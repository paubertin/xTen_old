#include "fontchar.h"

namespace xten { namespace xgraphics { namespace xfont {

	FontChar::FontChar(GLuint charcode, GLint width, GLint height, GLint xOffset, GLint yOffset,
		GLint xAdvance, GLint yAdvance, FT_Glyph glyph):
		m_Charchode(charcode), m_Pos(xmaths::vec2i(0,0)), m_Size(xmaths::vec2i(width,height)),
		m_Offset(xmaths::vec2i(xOffset,yOffset)), m_Advance(xmaths::vec2i(xAdvance,yAdvance)), m_Glyph(glyph)
	{ }

	FontChar::~FontChar()
	{ }

	void FontChar::calcVertices(GLint x, GLint y, Textured2DVertex * vertices) const
	{
		GLint x0 = x + m_Offset.x;
		GLint y0 = y + m_Offset.y;

		vertices[0].x = (GLfloat)(x0);
		vertices[0].y = (GLfloat)(y0);
		vertices[0].u = m_TexCoords[0];
		vertices[0].v = m_TexCoords[1];

		vertices[1].x = (GLfloat)(x0);
		vertices[1].y = (GLfloat)(y0 - m_Size.y);
		vertices[1].u = m_TexCoords[2];
		vertices[1].v = m_TexCoords[3];

		vertices[2].x = (GLfloat)(x0 + m_Size.x);
		vertices[2].y = (GLfloat)(y0 - m_Size.y);
		vertices[2].u = m_TexCoords[4];
		vertices[2].v = m_TexCoords[5];

		vertices[3].x = (GLfloat)(x0 + m_Size.x);
		vertices[3].y = (GLfloat)(y0);
		vertices[3].u = m_TexCoords[6];
		vertices[3].v = m_TexCoords[7];
	}

	void FontChar::initTexCoords(GLint texW, GLint texH)
	{
		GLfloat x1 = (GLfloat)(m_Pos.x) / (GLfloat)(texW);
		GLfloat y1 = (GLfloat)(m_Pos.y) / (GLfloat)(texH);
		GLfloat x2 = (GLfloat)(m_Pos.x + m_Size.x) / (GLfloat)(texW);
		GLfloat y2 = (GLfloat)(m_Pos.y + m_Size.y) / (GLfloat)(texH);

		m_TexCoords[0] = x1;
		m_TexCoords[1] = y1;
		
		m_TexCoords[2] = x1;
		m_TexCoords[3] = y2;
		
		m_TexCoords[4] = x2;
		m_TexCoords[5] = y2;
		
		m_TexCoords[6] = x2;
		m_TexCoords[7] = y1;
	}

	void FontChar::drawToBitmap(unsigned char* data, int texWidth, int texHeight)
	{
		if (isEmpty())
		{
			return;
		}

		initTexCoords(texWidth, texHeight);

		// Convert The Glyph To A Bitmap.
		FT_Glyph_To_Bitmap(&m_Glyph, FT_RENDER_MODE_NORMAL, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)(m_Glyph);

		// This Reference Will Make Accessing The Bitmap Easier.
		FT_Bitmap& bitmap = bitmap_glyph->bitmap;

		assert(bitmap.width == m_Size.x);
		assert(bitmap.rows == m_Size.y);

		int x, y = 0;
		int index;

		for (y = 0; y < bitmap.rows; y++)
		{
			for (x = 0; x < bitmap.width; x++)
			{
				index = (m_Pos.y + y) * texWidth + (m_Pos.x + x);
				data[index] = bitmap.buffer[y * bitmap.width + x];
			}
		}
	}


} } }