#include <ft2build.h>
#include FT_FREETYPE_H
#include "font.h"

namespace xten { namespace xgraphics { namespace xfont {

	GLboolean Font::hasKerning(FT_Face face)
	{
		GLboolean res = false;

		if (face)
		{
			res = static_cast<GLboolean>(FT_HAS_KERNING(face));
		}

		return res;
	}

	Font::Font(const std::string &name, FT_Face face, GLuint textureID): m_Name(name), m_TextureID(0)
	{
		m_Face = face;
		m_TextureID = textureID;
		m_Kerning = hasKerning(face);
	}

	Font::~Font()
	{
		if (m_Face)
		{
			FT_Done_Face(m_Face);
			m_Face = nullptr;
		}
	}

	void Font::addChar(GLuint charchode, FontChar * fontChar)
	{
		m_FontCharList[charchode] = fontChar;
	}

	void * Font::getChar(GLuint c)
	{
		unordered_map<GLuint, FontChar*>::const_iterator it = m_FontCharList.find(c);

		if (it != m_FontCharList.end())
		{
			return it->second;
		}

		return nullptr;
	}

	GLint Font::getKerning(GLuint glyphPrev, GLuint glyph)
	{
		FT_Vector kerningVec;
		GLint kerningX = 0;

		if (glyphPrev && glyph)
		{
			FT_Get_Kerning(m_Face, glyphPrev, glyph, FT_KERNING_DEFAULT, &kerningVec);
			kerningX += kerningVec.x >> 6;
		}

		return kerningX;
	}

	FontGeometry* Font::calcVertices(GLint x, GLint y, const std::string & text, GLint color, GLfloat alpha)
	{
		GLulong quads = text.length();
		Textured2DVertex * vertices = XNEW Textured2DVertex[VERTICES_PER_QUAD * quads];

		unordered_map<GLuint, FontChar*>::const_iterator it;

		GLint curX = x;
		GLuint glyph;
		GLuint glyphprev = 0;

		for (GLint i = 0; text[i] != 0; i++)
		{
			GLuint c = static_cast<GLuint>(text[i]);
			it = m_FontCharList.find(c);

			if (it != m_FontCharList.end())
			{
				FontChar* fontChar = it->second;

				if (m_Kerning)
				{
					glyph = FT_Get_Char_Index(m_Face, c);
					curX += getKerning(glyphprev, glyph);
					glyphprev = glyph;
				}

				if (!fontChar->isEmpty())
				{
					fontChar->calcVertices(curX, y, &vertices[i*VERTICES_PER_QUAD]);
				}

				curX += fontChar->getXAdvance();
			}
		}

		return XNEW FontGeometry(m_TextureID, color, alpha, quads, vertices);
	}

} } }