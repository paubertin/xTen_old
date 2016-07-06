#pragma once

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL\glew.h>
#include "../../utils/utils.h"
#include "fontchar.h"

namespace xten { namespace xgraphics { namespace xfont {

	using std::unordered_map;

	struct Character
	{
		GLuint texture_ID;
		xmaths::vec2i size;
		xmaths::vec2i bearing;
		xmaths::vec2i advance;
		GLfloat x_offset; // x offset of glyph in texture coordinates
	};
	
	struct Atlas
	{
		GLuint texture_ID;
		GLuint width;
		GLuint height;
	};

	class FontGeometry
	{
	private:
		GLuint m_TextureID;
		GLint m_Color;
		GLfloat m_Alpha;
		GLulong m_Quads;
		Textured2DVertex* m_Vertices;
	public:
		FontGeometry(GLuint textureID, GLint color, GLfloat alpha, GLulong quads, Textured2DVertex* vertices):
			m_TextureID(textureID), m_Color(color), m_Alpha(alpha), m_Quads(quads), m_Vertices(vertices)
		{ }

		~FontGeometry()
		{
			XDEL_ARRAY(m_Vertices);
		}

		const GLuint & getTextureID() { return m_TextureID; }
		const GLint &  getColor() { return m_Color; }
		const GLfloat & getAlpha() { return m_Alpha; }
		const GLulong & getQuads() { return m_Quads; }
		Textured2DVertex* getVertices() { return m_Vertices; }
	};
	
	class Font
	{
	private:
		std::string m_Name;
		GLuint m_TextureID;
		FT_Face m_Face;
		GLboolean m_Kerning;
		unordered_map<GLuint, FontChar*> m_FontCharList;
	public:
		Font(const std::string &name, FT_Face face, GLuint textureID);
		~Font();
	public:
		inline const std::string getName() const { return m_Name; }
		void addChar(GLuint charchode, FontChar * fontChar);
		void* getChar(GLuint c);
		GLint getKerning(GLuint glyphPrev, GLuint glyph);
		FontGeometry* calcVertices(GLint x, GLint y, const std::string & text, GLint color, GLfloat alpha);
	private:
		GLboolean hasKerning(FT_Face face);
	};

} } }