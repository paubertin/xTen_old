#pragma once

#include <GL\glew.h>

#include "../../utils/utils.h"
#include "../../utils/streamfactory.h"
#include "font.h"

namespace xten { namespace xgraphics { namespace xfont {

	class FontAtlas
	{
	private:
		GLint m_Width;
		GLint m_Height;
		GLuint m_TextureID;
		FT_LibraryRec_* m_Library;
		std::vector<Font* > m_FontList;
		std::vector<FontChar*> m_FontCharList;
		std::vector<FileStream* > m_FontFiles;
		StreamFactory* m_StreamFactory;
	public:
		FontAtlas(StreamFactory* streamFactory, GLint width, GLint height);
		~FontAtlas();
	public:
		Font* addFont(const std::string &name, const std::string& fontName, GLuint size);
		Font* getFont(const std::string &name);
		void create();
	};

} } }