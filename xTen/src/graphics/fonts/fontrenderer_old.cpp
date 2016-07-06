#include "fontrenderer.h"
#include "fontmanager.h"

namespace xten { namespace xgraphics { namespace xfont {

	const int VERTEX_STRIDE = 4;

	FontRenderer::FontRenderer(): m_CacheSize(256)
	{ }

	FontRenderer::~FontRenderer()
	{
		m_CacheSize = 0;
		shutdown();
	}

	void FontRenderer::shutdown()
	{
		XDEL(m_IBO);
		XDEL(m_VBO);
	}

	void FontRenderer::init()
	{
			m_VBO = BufferObject::createVertexBuffer((GLuint const)(m_CacheSize * VERTICES_PER_QUAD * VERTEX_STRIDE * sizeof(GLfloat)));
			m_IBO = BufferObject::createIndexBuffer((GLuint const)(m_CacheSize * INDICES_PER_QUAD * sizeof(GLubyte)));

			// Indices
			GLubyte * indices = XNEW GLubyte[(GLuint)(m_CacheSize*INDICES_PER_QUAD)];
			for (GLint i = 0; i < m_CacheSize; i++)
			{
				indices[i * INDICES_PER_QUAD + 0] = (GLubyte)(i * VERTICES_PER_QUAD + 0);
				indices[i * INDICES_PER_QUAD + 1] = (GLubyte)(i * VERTICES_PER_QUAD + 1);
				indices[i * INDICES_PER_QUAD + 2] = (GLubyte)(i * VERTICES_PER_QUAD + 2);
				indices[i * INDICES_PER_QUAD + 3] = (GLubyte)(i * VERTICES_PER_QUAD + 2);
				indices[i * INDICES_PER_QUAD + 4] = (GLubyte)(i * VERTICES_PER_QUAD + 3);
				indices[i * INDICES_PER_QUAD + 5] = (GLubyte)(i * VERTICES_PER_QUAD + 0);
			}
			m_IBO->fill(0, (GLuint)(m_CacheSize * INDICES_PER_QUAD * sizeof(GLubyte)), indices);
			XDEL_ARRAY(indices);

			m_Shader = ShaderManager::add("font", "shaders/font.vert", "shaders/font.frag");

			glBindAttribLocation(m_Shader->getID(), 0, "position");
			glBindAttribLocation(m_Shader->getID(), 1, "texCoord");
		
	}

	void FontRenderer::render(FontGeometry* fontGeometry)
	{
		GLboolean IsCullEnabled = glIsEnabled(GL_CULL_FACE);
		GLboolean IsDepthEnabled = glIsEnabled(GL_DEPTH_TEST);

		const GLuint & textureId = fontGeometry->getTextureID();
		const GLint & color = fontGeometry->getColor();
		const GLfloat & alpha = fontGeometry->getAlpha();
		const GLulong & quads = fontGeometry->getQuads();
		Textured2DVertex * vertices = fontGeometry->getVertices();

		m_VBO->bind();
		m_VBO->fill(0, quads * VERTICES_PER_QUAD * VERTEX_STRIDE * sizeof(GLfloat), &vertices[0]);

		if (quads == 0) return;

		assert(quads * INDICES_PER_QUAD < 0xffff);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		m_Shader->enable();
		
		xmaths::mat4 proj = xmaths::mat4::orthographic(0.f, 800.f, 0.f, 600.f, -1.f, 1.f);
		m_Shader->setMat4f("projection", proj);

		// Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Set texture
		glBindTexture(GL_TEXTURE_2D, textureId);
		GLint textureSamplerLoc = glGetUniformLocation(m_Shader->getID(), "tex");
		glUniform1i(textureSamplerLoc, 0);

		//m_Shader->setInteger("tex", 0);

		// Set color
		const GLfloat inv256 = 1.0f / 256.0f;

		GLfloat red = ((color >> 16)&0xff) * inv256;
		GLfloat green = ((color >> 8)&0xff) * inv256;
		GLfloat blue = (color&0xff) * inv256;

		xmaths::vec4 colorV = xmaths::vec4(red, green, blue, alpha);
		m_Shader->setVector4f("textColor", colorV);

		// vertex data
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * VERTEX_STRIDE, (const void*)0);
		glEnableVertexAttribArray(0);

		// texture data
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * VERTEX_STRIDE, (const void*)(2 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glDrawElements(GL_TRIANGLES, (GLsizei)(quads * INDICES_PER_QUAD), GL_UNSIGNED_BYTE, 0);

		glDisable(GL_BLEND);
		if (IsCullEnabled) {
			glEnable(GL_CULL_FACE);
		}
		if (IsDepthEnabled) {
			glEnable(GL_DEPTH_TEST);
		}
	}

} } }