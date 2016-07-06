#include "texture2d.h"
#include "../xTenStd.h"

namespace xten { namespace xgraphics {

	Texture2D::Texture2D(GLenum textureTarget, const std::string & name) :
		m_Name(name), m_WrapS(GL_REPEAT), m_WrapT(GL_REPEAT),
		m_FilterMin(GL_LINEAR), m_FilterMax(GL_LINEAR),
		m_Image_Format(GL_RGB), m_Internal_Format(GL_RGB),
		m_TextureTarget(textureTarget)
	{
		GLCall( glGenTextures(1, &m_TID) );
	}

	Texture2D::~Texture2D()
	{
		GLCall( glDeleteTextures(1, &m_TID) );
	}

	void Texture2D::bind(GLenum TextureUnit) const
	{
		GLCall( glActiveTexture(TextureUnit) );
		GLCall( glBindTexture(m_TextureTarget, m_TID) );
	}

	void Texture2D::unbind() const
	{
		GLCall( glBindTexture(m_TextureTarget, 0) );
	}

	void Texture2D::generate(GLint width, GLint height, GLuint bits, unsigned char* image)
	{
		m_Width = width;
		m_Height = height;

		if (bits != 24 && bits != 32)
		{
			printf("ERROR [Texture2D]. Unsupported image bit-depth! (%d)\n", bits);
		}

		m_Internal_Format = bits == 32 ? GL_RGBA : GL_RGB;
		m_Image_Format = bits == 32 ? GL_BGRA : GL_BGR;

		GLCall( glBindTexture(m_TextureTarget, m_TID) );
		GLCall( glTexImage2D(m_TextureTarget, 0, m_Internal_Format, width, height, 0, m_Image_Format, GL_UNSIGNED_BYTE, image) );
		// Set the texture parameters
		GLCall( glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_S, m_WrapS) );
		GLCall( glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_T, m_WrapT) );
		GLCall( glTexParameteri(m_TextureTarget, GL_TEXTURE_MIN_FILTER, m_FilterMin) );
		GLCall( glTexParameteri(m_TextureTarget, GL_TEXTURE_MAG_FILTER, m_FilterMax) );
		GLCall( glGenerateMipmap(m_TextureTarget) );
		GLCall( glBindTexture(m_TextureTarget, 0) );
	}

} }