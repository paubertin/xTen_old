#include <vector>
#include "shader.h"

namespace xten {
	namespace xgraphics {

		Shader::Shader(const std::string & name) :
			m_Name(name)
		{

		}

		Shader::~Shader()
		{
			glDeleteProgram(m_ID);
		}

		GLboolean Shader::compile(const GLchar * vertexpath, const GLchar * fragpath, const GLchar * geompath)
		{
			GLuint vertexID = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexID, 1, &vertexpath, NULL);
			glCompileShader(vertexID);
			if (!checkErrors(vertexID, "VERTEX"))
			{
				glDeleteShader(vertexID);
				return GL_FALSE;
			}

			GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragID, 1, &fragpath, NULL);
			glCompileShader(fragID);
			if (!checkErrors(fragID, "FRAGMENT"))
			{
				glDeleteShader(vertexID);
				glDeleteShader(fragID);
				return GL_FALSE;
			}

			GLuint geomID;
			if (geompath != nullptr)
			{
				geomID = glCreateShader(GL_GEOMETRY_SHADER);
				glShaderSource(geomID, 1, &geompath, NULL);
				glCompileShader(geomID);
				if (!checkErrors(geomID, "GEOMETRY"))
				{
					glDeleteShader(vertexID);
					glDeleteShader(fragID);
					glDeleteShader(geomID);
					return GL_FALSE;
				}
			}

			m_ID = glCreateProgram();
			glAttachShader(m_ID, vertexID);
			glAttachShader(m_ID, fragID);
			if (geompath != nullptr)
			{
				glAttachShader(m_ID, geomID);
			}

			glLinkProgram(m_ID);
			if (!checkProgramErrors(m_ID, "PROGRAM", "Link"))
			{
				glDetachShader(m_ID, vertexID);
				glDeleteShader(vertexID);
				glDetachShader(m_ID, fragID);
				glDeleteShader(fragID);
				if (geompath != nullptr)
				{
					glDetachShader(m_ID, geomID);
					glDeleteShader(geomID);
				}
				return GL_FALSE;
			}

			glValidateProgram(m_ID);
			if (!checkProgramErrors(m_ID, "PROGRAM", "Validate"))
			{
				glDetachShader(m_ID, vertexID);
				glDeleteShader(vertexID);
				glDetachShader(m_ID, fragID);
				glDeleteShader(fragID);
				if (geompath != nullptr)
				{
					glDetachShader(m_ID, geomID);
					glDeleteShader(geomID);
				}
				return GL_FALSE;
			}

			glDetachShader(m_ID, vertexID);
			glDeleteShader(vertexID);
			glDetachShader(m_ID, fragID);
			glDeleteShader(fragID);
			if (geompath != nullptr)
			{
				glDetachShader(m_ID, geomID);
				glDeleteShader(geomID);
			}

			return GL_TRUE;

		}

		void Shader::enable() const
		{
			glUseProgram(m_ID);
		}

		void Shader::disable() const
		{
			glUseProgram(0);
		}

		GLboolean Shader::checkProgramErrors(GLuint shaderID, const std::string shader_type, const std::string type)
		{
			GLint success;
			if (type == "Link")
			{
				glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
				if (!success)
				{
					GLint length;
					glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
					std::vector<char> error(length);
					glGetShaderInfoLog(shaderID, length, &length, &error[0]);
					std::string errorMessage(&error[0]);
					std::cout << "ERROR [Shader]. " << type << "-time error: Type: " << shader_type
						<< std::endl << errorMessage << std::endl
						<< "-- --------------------------------- --" << std::endl;
					return GL_FALSE;
				}
				else
				{
					return GL_TRUE;
				}
			}
			else if (type == "Validate")
			{
				glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &success);
				if (!success)
				{
					GLint length;
					glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
					std::vector<char> error(length);
					glGetShaderInfoLog(shaderID, length, &length, &error[0]);
					std::string errorMessage(&error[0]);
					std::cout << "ERROR [Shader]. " << type << "-time error: Type: " << shader_type
						<< std::endl << errorMessage << std::endl
						<< "-- --------------------------------- --" << std::endl;
					return GL_FALSE;
				}
				else
				{
					return GL_TRUE;
				}
			}
			else
			{
				return GL_FALSE;
			}
		}


		GLboolean Shader::checkErrors(GLuint shaderID, const std::string shader_type)
		{
			GLint success;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				GLint length;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(shaderID, length, &length, &error[0]);
				std::string errorMessage(&error[0]);
				std::cout << "ERROR [Shader]. Compile-time error: Type: " << shader_type
					<< std::endl << errorMessage << std::endl
					<< "-- --------------------------------- --" << std::endl;
				return GL_FALSE;
			}
			else
			{
				return GL_TRUE;
			}
		}

		GLint Shader::getUniformLocation(const GLchar *name)
		{
			GLint result = glGetUniformLocation(m_ID, name);

			if (result == -1)
				std::cout << "ERROR [Shader]. Could not find uniform " << name << " in shader" << std::endl;

			return result;
		}

		void Shader::setFloat(const GLchar* name, float value)
		{
			glUniform1f(getUniformLocation(name), value);
		}

		void Shader::setInteger(const GLchar * name, int value)
		{
			glUniform1i(getUniformLocation(name), value);

		}

		void Shader::setUniform1iv(const GLchar * name, int count, int * value)
		{
			glUniform1iv(getUniformLocation(name), count, value);
		}

		void Shader::setVector2f(const GLchar * name, const xmaths::vec2 & vector)
		{
			glUniform2f(getUniformLocation(name), vector.x, vector.y);
		}

		void Shader::setVector2f(const GLchar * name, GLfloat x, GLfloat y)
		{
			glUniform2f(getUniformLocation(name), x, y);
		}

		void Shader::setVector3f(const GLchar * name, const xmaths::vec3 & vector)
		{
			glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
		}

		void Shader::setVector3f(const GLchar * name, GLfloat x, GLfloat y, GLfloat z)
		{
			glUniform3f(getUniformLocation(name), x, y, z);
		}

		void Shader::setVector4f(const GLchar * name, const xmaths::vec4 & vector)
		{
			glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
		}

		void Shader::setVector4f(const GLchar * name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
		{
			glUniform4f(getUniformLocation(name), x, y, z, w);
		}

		void Shader::setMat4f(const GLchar * name, const xmaths::mat4 & matrix)
		{
			glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.data);
		}

	}
}