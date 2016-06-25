#pragma once

#include <GL\glew.h>
#include <string>

#include "../../maths/xmaths.h"

namespace xten { namespace xgraphics {

	class Shader
	{
	private:
		GLuint m_ID;
		std::string m_Name;
	public:
		Shader(const std::string & name);
		~Shader();
		GLboolean compile(const GLchar * vertexpath, const GLchar * fragpath, const GLchar * geompath = nullptr);
		void enable() const;
		void disable() const;
		inline const std::string& getName() const { return m_Name; }
		inline const GLuint getID() const { return m_ID; }

		void setFloat(const GLchar* name, float value);
		void setInteger(const GLchar* name, int value);
		void setUniform1iv(const GLchar * name, int count, int * value);

		void setVector2f(const GLchar* name, const xmaths::vec2& vector);
		void setVector2f(const GLchar* name, GLfloat x, GLfloat y);

		void setVector3f(const GLchar* name, const xmaths::vec3& vector);
		void setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);

		void setVector4f(const GLchar* name, const xmaths::vec4& vector);
		void setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

		void setMat4f(const GLchar* name, const xmaths::mat4& matrix);
	private:
		GLint getUniformLocation(const GLchar *name);
		GLboolean checkErrors(GLuint shaderID, const std::string shader_type);
		GLboolean checkProgramErrors(GLuint shaderID, const std::string shader_type, const std::string type);
	};

} }