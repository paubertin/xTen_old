#pragma once

#include <ft2build.h>
#include FT_GLYPH_H
#include FT_FREETYPE_H

#include <GL\glew.h>
#include "../../utils/utils.h"

namespace xten { namespace xgraphics { namespace xfont {

	const int VERTICES_PER_QUAD = 4;
	const int INDICES_PER_QUAD = 6;
	const int COMP_VERT_POS = 2;
	
	struct Textured2DVertex
	{
		GLfloat x, y;
		GLfloat u, v;
	};

	class FontChar
	{
	private:
		GLuint m_Charchode;
		
		xmaths::vec2i m_Pos;
		xmaths::vec2i m_Size;
		xmaths::vec2i m_Offset;
		xmaths::vec2i m_Advance;

		GLfloat m_TexCoords[VERTICES_PER_QUAD * COMP_VERT_POS];

		FT_Glyph m_Glyph;
	public:
		FontChar(GLuint charcode, GLint width, GLint height, GLint xOffset, GLint yOffset, GLint xAdvance, GLint yAdvance, FT_Glyph glyph);
		~FontChar();
	public:
		inline GLint getXAdvance() const { return m_Advance.x; }
		inline GLint getYAdvance() const { return m_Advance.y; }
		inline void releaseGlyph() { if (m_Glyph) { FT_Done_Glyph(m_Glyph); } }
		void drawToBitmap(unsigned char* data, int texWidth, int texHeight);
		inline void setXY(int x, int y) { m_Pos.x = x; m_Pos.y = y; }
		inline GLint getWidth() const { return m_Size.x; }
		inline GLint getHeight() const { return m_Size.y; }
		inline bool isEmpty() const { return ((m_Size.x == 0) || (m_Size.y == 0)); }
		inline int getNumPixels() const { return m_Size.x * m_Size.y; }
		void calcVertices(GLint x, GLint y, Textured2DVertex* vertices) const;
	private:
		void initTexCoords(GLint texW, GLint texH);
	};

} } }