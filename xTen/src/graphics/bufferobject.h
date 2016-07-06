#pragma once

#include <GL\glew.h>
#include "../xTenStd.h"

namespace xten { namespace xgraphics {

	class BufferObject
	{
	private:
		const GLuint m_Type;
		const GLuint m_Size;
		GLuint m_ID;
	public:
		static BufferObject* createVertexBuffer(const GLuint size)
		{
			return XNEW BufferObject(GL_ARRAY_BUFFER, size);
		}
		static BufferObject* createIndexBuffer(const GLuint size)
		{
			return XNEW BufferObject(GL_ELEMENT_ARRAY_BUFFER, size);
		}
	private:
		BufferObject(const GLuint type, const GLuint size) :
			m_Type(type), m_Size(size)
		{
			GLCall( glGenBuffers(1, &m_ID) );
			GLCall( glBindBuffer(m_Type, m_ID) );
			GLCall( glBufferData(m_Type, m_Size, NULL, GL_DYNAMIC_DRAW) );
		}
	public:
		~BufferObject() { GLCall( glDeleteBuffers(1, &m_ID) ); }
		inline void bind() const { GLCall( glBindBuffer(m_Type, m_ID) ); }
		inline void unbind() const { GLCall( glBindBuffer(m_Type, 0) ); }
		inline const GLuint getSize() const { return m_Size; }
		void fill(const GLuint offset, GLuint size, const void* data) const
		{
			GLCall( glBufferSubData(m_Type, offset, size, data) );
		}
	};

} }