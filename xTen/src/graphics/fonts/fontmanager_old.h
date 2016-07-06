#pragma once

#include "../../utils/utils.h"

#include "fontatlas.h"
#include "fontrenderer.h"
#include "../shaders/shadermanager.h"

namespace xten { namespace xgraphics { namespace xfont {

	class FontManager
	{
	private:
		static FontAtlas* m_Atlas;
		static FontRenderer* m_Renderer;
		static StreamFactory* m_StreamFactory;
		static FontManager* s_Instance;
	public:
		static FontManager* get() { return s_Instance; }
		// Initialization
		static void init(const std::string & dirpath);
		static void clean();
		// Loads (and generates) a font from file
		static void add(const std::string & name, const std::string & filename, GLuint size);
		// Renders text
		static void renderText(const std::string & fontname, const std::string &text, GLfloat x, GLfloat y, GLfloat scale, xmaths::vec4 color);
	private:
		FontManager(const std::string & dirpath);
		~FontManager() { }
	};

} } }