#pragma once

#include <string>
#include <SOIL2.h>
#include <GL\glew.h>


namespace xten { namespace xgraphics {

	class Texture2D
	{
		friend class TexManager;
	private:
		GLuint m_TID;
		GLenum m_TextureTarget;
		GLuint m_Width;
		GLuint m_Height;
		std::string m_Name, m_FileName;
		GLuint m_WrapS; // Wrapping mode on S axis
		GLuint m_WrapT; // Wrapping mode on T axis
		GLuint m_FilterMin; // Filtering mode if texture pixels < screen pixels
		GLuint m_FilterMax; // Filtering mode if texture pixels > screen pixels
		GLuint m_Internal_Format; // Format of texture object
		GLuint m_Image_Format; // Format of loaded image
	private:
	public:
		Texture2D(GLenum TextureTarget, const std::string & name);
		~Texture2D();
		void generate(GLint width, GLint height, GLuint bits, unsigned char* image);

		inline const std::string& getName() const { return m_Name; }
		inline const GLuint getID() const { return m_TID; }
		inline const GLuint getWidth() const { return m_Width; }
		inline const GLuint getHeight() const { return m_Height; }

		void bind(GLenum TextureUnit) const;
		void unbind() const;
	};

} }