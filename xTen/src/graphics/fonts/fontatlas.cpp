#include <ft2build.h>
#include FT_GLYPH_H
#include FT_FREETYPE_H

#include "binpacker.h"
#include "fontatlas.h"

namespace xten { namespace xgraphics { namespace xfont {

	FontAtlas::FontAtlas(StreamFactory* streamFactory, GLint width, GLint height):
		m_Width(width), m_Height(height), m_StreamFactory(streamFactory)
	{
		FT_Error error;

		error = FT_Init_FreeType(&m_Library);
		if (error)
		{
			std::cout << "ERROR [Freetype]. Could not init FreeType Library. Error: " << error << std::endl
				<< "-- --------------------------------- --" << std::endl;
		}

		glGenTextures(1, &m_TextureID);
	}

	FontAtlas::~FontAtlas()
	{
		m_FontFiles.clear();
		m_FontList.clear();
		XDEL_VEC(m_FontCharList);
		m_FontCharList.clear();
		if (m_Library)
		{
			FT_Done_FreeType(m_Library);
			m_Library = nullptr;
		}
	}

	Font* FontAtlas::addFont(const std::string &name, const std::string & filename, GLuint size)
	{
		for (Font* font : m_FontList)
		{
			if (font->getName() == name)
			{
				std::cout << "ERROR [FontAtlas]. Font " << name << " already exists." << std::endl
					<< "-- --------------------------------- --" << std::endl;
				return nullptr;
			}
		}

		FT_Error error;

		FT_Face face;

		FileStream* fontFile = m_StreamFactory->open(filename);
		m_FontFiles.push_back(fontFile);

		char* fontData = XNEW char[fontFile->size()];
		fontFile->read(fontData, fontFile->size());

		error = FT_New_Memory_Face(m_Library, (FT_Byte*)fontData, fontFile->size(), 0, &face);
		if (error)
		{
			std::cout << "ERROR " << error << " [FontAtlas]. Failed to load font file: " << filename.c_str() << std::endl
				<< "-- --------------------------------- --" << std::endl;
			return nullptr;
		}

		// FreeType Measures Font Size In Terms Of 1/64ths Of Pixels.
		FT_Set_Char_Size(face, size * 64, size * 64, 72, 72);

		Font* font = XNEW Font(name, face, m_TextureID);
		m_FontList.push_back(font);

		FT_Glyph pGlyph;
		GLuint ixGlyph;

		for (GLuint c = 32; c < 128; c++)
		{
			if (font->getChar(c) != nullptr)
			{
				return nullptr;
			}

			ixGlyph = FT_Get_Char_Index(face, c);
			if (ixGlyph == 0)
			{
				std::cout << "ERROR [FontAtlas]. Character: " << c << " does not exist in font " << filename.c_str() << std::endl
					<< "-- --------------------------------- --" << std::endl;
				//return nullptr;
				continue;
			}

			error = FT_Load_Glyph(face, ixGlyph, FT_LOAD_RENDER);
			if (error)
			{
				std::cout << "ERROR " << error << " [FontAtlas]. Failed to load glyph for char: " << c << std::endl
					<< "-- --------------------------------- --" << std::endl;
				return nullptr;
			}

			// Move The Face's Glyph Into A Glyph Object.
			error = FT_Get_Glyph(face->glyph, &pGlyph);
			if (error)
			{
				std::cout << "ERROR " << error << " [FontAtlas]. Failed to load glyph object for char: " << c << std::endl
					<< "-- --------------------------------- --" << std::endl;
				return nullptr;
			}

			// all metrics dimensions are multiplied by 64, so we have to divide by 64
			GLint xoffset = (GLint)face->glyph->metrics.horiBearingX >> 6;
			GLint yoffset = (GLint)face->glyph->metrics.horiBearingY >> 6;
			GLint width = (GLint)face->glyph->metrics.width >> 6;
			GLint height = (GLint)face->glyph->metrics.height >> 6;
			GLint xadv = (GLint)face->glyph->metrics.horiAdvance >> 6;
			GLint yadv = (GLint)face->glyph->metrics.vertAdvance >> 6;

			// Create a new FontChar that represent one character in the font
			FontChar* fontChar = XNEW FontChar(c, width, height, xoffset, yoffset, xadv, yadv, pGlyph);
			m_FontCharList.push_back(fontChar);
			font->addChar(c, fontChar);
		}

		return font;
	}

	Font* FontAtlas::getFont(const std::string & name)
	{
		for (Font* font : m_FontList)
		{
			if (font->getName() == name)
			{
				return font;
			}
		}
		return nullptr;
	}

	inline GLboolean sizeComparator(FontChar* fontChar1, FontChar* fontChar2)
	{
		return fontChar1->getNumPixels() > fontChar2->getNumPixels();
	}

	void FontAtlas::create()
	{
		std::sort(m_FontCharList.begin(), m_FontCharList.end(), sizeComparator);

		BinPacker binPacker(xmaths::vec2i(m_Width, m_Height));
		if (!binPacker.pack(m_FontCharList))
		{
			std::cout << "ERROR [FontAtlas]. Failed to render glyphs to texture. Glyphs did not fit." << std::endl
				<< "-- --------------------------------- --" << std::endl;
		}

		unsigned char* data = XNEW unsigned char[m_Width*m_Height];

		for (int i = 0; i < (int)m_FontCharList.size(); i++)
		{
			m_FontCharList[i]->drawToBitmap(data, m_Width, m_Height);
			m_FontCharList[i]->releaseGlyph();
		}

		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		// Set the texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_Width, m_Height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		int err = glGetError();
		if (err != GL_NO_ERROR)
		{
			std::cout << "ERROR " << err << " [FontAtlas]. Error in glTexImage2D." << std::endl
				<< "-- --------------------------------- --" << std::endl;
		}

		// clean up memory
		XDEL_ARRAY(data);
	}


} } }