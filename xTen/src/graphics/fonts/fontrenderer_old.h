#pragma once

#include "../../utils/utils.h"

#include "../bufferobject.h"
#include "../shaders/shadermanager.h"
#include "fontatlas.h"

namespace xten { namespace xgraphics { namespace xfont {

	class FontRenderer
	{
	private:
		// current amount of quads that can fit in the arrays
		GLint m_CacheSize;
		Shader* m_Shader;
		BufferObject* m_VBO;
		BufferObject* m_IBO;
		//static GLuint m_VAO;
		//static GLuint m_VBO;
		void shutdown();
	public:
		FontRenderer();
		~FontRenderer();
		// Initializes FontRenderer, creates font shader, VBO and IBO
		void init();
		// Renders text
		void render(FontGeometry* fontGeometry);
		//static void renderText(std::string fontname, std::string text, GLfloat x, GLfloat y, GLfloat scale, xmaths::vec4 color);
		//static void renderAtlasText(std::string fontname, std::string text, GLfloat x, GLfloat y, GLfloat scale, xmaths::vec4 color);
	};

} } }