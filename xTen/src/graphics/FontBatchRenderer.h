#pragma once

#include "renderer2d.h"
#include "bufferobject.h"

namespace xten { namespace xgraphics {

#define RENDERER_MAX_SPRITES	60000
#define RENDERER_VERTEX_SIZE	sizeof(VertexData)
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURES	32

#define SHADER_VERTEX_INDEX		0
#define SHADER_UV_INDEX			1
#define SHADER_MASK_UV_INDEX	2
#define SHADER_TID_INDEX		3
#define SHADER_MID_INDEX		4
#define SHADER_COLOR_INDEX		5

	class FontBatchRenderer : public Renderer2d
	{
	private:
		GLuint m_VAO;
		BufferObject* m_VBO;
		BufferObject* m_IBO;
		GLsizei m_IndexCount;
		VertexData* m_Buffer;
		std::vector<GLuint> m_TextureSlots;

	public:
		xBatchRenderer2d();
		~xBatchRenderer2d();
		void begin() override;
		void submit(const xRenderable2d * renderable) override;
		void drawString(const std::string & text, const xMaths::vec3 & position, const xFont &font, GLuint color) override;
		void end() override;
		void flush() override;
	private:
		void init();
		float submitTexture(GLuint textureID);
		float submitTexture(const xTexture* texture);
	};


} }