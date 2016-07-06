#include <vector>
#include "shader.h"
#include "../../xTenStd.h"

namespace xten {
	namespace xgraphics {

		Shader::Shader(const std::string & name) :
			m_Name(name)
		{

		}

		Shader::~Shader()
		{
			GLCall( glDeleteProgram(m_ID) );
		}

		GLboolean Shader::compile(const GLchar * vertexpath, const GLchar * fragpath, const GLchar * geompath)
		{
			GLuint vertexID = GLCall( glCreateShader(GL_VERTEX_SHADER) );
			GLCall( glShaderSource(vertexID, 1, &vertexpath, NULL) );
			GLCall( glCompileShader(vertexID) );
			if (!checkErrors(vertexID, "VERTEX"))
			{
				GLCall( glDeleteShader(vertexID) );
				return GL_FALSE;
			}

			GLuint fragID = GLCall( glCreateShader(GL_FRAGMENT_SHADER) );
			GLCall( glShaderSource(fragID, 1, &fragpath, NULL) );
			GLCall( glCompileShader(fragID) );
			if (!checkErrors(fragID, "FRAGMENT"))
			{
				GLCall( glDeleteShader(vertexID) );
				GLCall( glDeleteShader(fragID) );
				return GL_FALSE;
			}

			GLuint geomID;
			if (geompath != nullptr)
			{
				geomID = GLCall( glCreateShader(GL_GEOMETRY_SHADER) );
				GLCall( glShaderSource(geomID, 1, &geompath, NULL) );
				GLCall( glCompileShader(geomID) );
				if (!checkErrors(geomID, "GEOMETRY"))
				{
					GLCall( glDeleteShader(vertexID) );
					GLCall( glDeleteShader(fragID) );
					GLCall( glDeleteShader(geomID) );
					return GL_FALSE;
				}
			}

			m_ID = GLCall( glCreateProgram() );
			GLCall( glAttachShader(m_ID, vertexID) );
			GLCall( glAttachShader(m_ID, fragID) );
			if (geompath != nullptr)
			{
				GLCall( glAttachShader(m_ID, geomID) );
			}

			GLCall( glLinkProgram(m_ID) );
			if (!checkProgramErrors(m_ID, "PROGRAM", "Link"))
			{
				GLCall( glDetachShader(m_ID, vertexID) );
				GLCall( glDeleteShader(vertexID) );
				GLCall( glDetachShader(m_ID, fragID) );
				GLCall( glDeleteShader(fragID) );
				if (geompath != nullptr)
				{
					GLCall( glDetachShader(m_ID, geomID) );
					GLCall( glDeleteShader(geomID) );
				}
				return GL_FALSE;
			}

			GLCall( glValidateProgram(m_ID) );
			if (!checkProgramErrors(m_ID, "PROGRAM", "Validate"))
			{
				GLCall( glDetachShader(m_ID, vertexID) );
				GLCall( glDeleteShader(vertexID) );
				GLCall( glDetachShader(m_ID, fragID) );
				GLCall( glDeleteShader(fragID) );
				if (geompath != nullptr)
				{
					GLCall( glDetachShader(m_ID, geomID) );
					GLCall( glDeleteShader(geomID) );
				}
				return GL_FALSE;
			}

			GLCall( glDetachShader(m_ID, vertexID) );
			GLCall( glDeleteShader(vertexID) );
			GLCall( glDetachShader(m_ID, fragID) );
			GLCall( glDeleteShader(fragID) );
			if (geompath != nullptr)
			{
				GLCall( glDetachShader(m_ID, geomID) );
				GLCall( glDeleteShader(geomID) );
			}

			return GL_TRUE;

		}

		void Shader::enable() const
		{
			GLCall( glUseProgram(m_ID) );
		}

		void Shader::disable() const
		{
			GLCall( glUseProgram(0) );
		}

		GLboolean Shader::checkProgramErrors(GLuint shaderID, const std::string shader_type, const std::string type)
		{
			GLint success;
			if (type == "Link")
			{
				GLCall( glGetProgramiv(shaderID, GL_LINK_STATUS, &success) );
				if (!success)
				{
					GLint length;
					GLCall( glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length) );
					std::vector<char> error(length);
					GLCall( glGetShaderInfoLog(shaderID, length, &length, &error[0]) );
					std::string errorMessage("[Shader]. ");
					errorMessage += type;
					errorMessage += "-time error: Type: ";
					errorMessage += shader_type;
					errorMessage += "\n";
					errorMessage += &error[0];
					XTEN_ERROR(errorMessage);
					//std::cout << "ERROR [Shader]. " << type << "-time error: Type: " << shader_type
					//	<< std::endl << errorMessage << std::endl
					//	<< "-- --------------------------------- --" << std::endl;
					return GL_FALSE;
				}
				else
				{
					return GL_TRUE;
				}
			}
			else if (type == "Validate")
			{
				GLCall( glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &success) );
				if (!success)
				{
					GLint length;
					GLCall( glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length) );
					std::vector<char> error(length);
					GLCall( glGetShaderInfoLog(shaderID, length, &length, &error[0]) );
					std::string errorMessage("[Shader]. ");
					errorMessage += type;
					errorMessage += "-time error: Type: ";
					errorMessage += shader_type;
					errorMessage += "\n";
					errorMessage += &error[0];
					XTEN_ERROR(errorMessage);
					//std::cout << "ERROR [Shader]. " << type << "-time error: Type: " << shader_type
					//	<< std::endl << errorMessage << std::endl
					//	<< "-- --------------------------------- --" << std::endl;
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
			GLCall( glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success) );
			if (!success)
			{
				GLint length;
				GLCall( glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length) );
				std::vector<char> error(length);
				GLCall( glGetShaderInfoLog(shaderID, length, &length, &error[0]) );
				std::string errorMessage("[Shader]. Compile-time error: Type: ");
				errorMessage += shader_type;
				errorMessage += "\n";
				errorMessage += &error[0];
				XTEN_ERROR(errorMessage);
				//std::cout << "ERROR [Shader]. Compile-time error: Type: " << shader_type
				//	<< std::endl << errorMessage << std::endl
				//	<< "-- --------------------------------- --" << std::endl;
				return GL_FALSE;
			}
			else
			{
				return GL_TRUE;
			}
		}

		GLint Shader::getUniformLocation(const GLchar *name)
		{
			GLint result = GLCall( glGetUniformLocation(m_ID, name) );

			if (result == -1)
			{
				std::string err = "[Shader]. Could not find uniform " + std::string(name) + " in shader.";
				XTEN_ERROR(err);
			}
			return result;
		}

		void Shader::setFloat(const GLchar* name, float value)
		{
			GLCall( glUniform1f(getUniformLocation(name), value) );
		}

		void Shader::setInteger(const GLchar * name, int value)
		{
			GLCall( glUniform1i(getUniformLocation(name), value) );

		}

		void Shader::setUniform1iv(const GLchar * name, int count, int * value)
		{
			GLCall( glUniform1iv(getUniformLocation(name), count, value) );
		}

		void Shader::setVector2f(const GLchar * name, const xmaths::vec2 & vector)
		{
			GLCall( glUniform2f(getUniformLocation(name), vector.x, vector.y) );
		}

		void Shader::setVector2f(const GLchar * name, GLfloat x, GLfloat y)
		{
			GLCall( glUniform2f(getUniformLocation(name), x, y) );
		}

		void Shader::setVector3f(const GLchar * name, const xmaths::vec3 & vector)
		{
			GLCall( glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z) );
		}

		void Shader::setVector3f(const GLchar * name, GLfloat x, GLfloat y, GLfloat z)
		{
			GLCall( glUniform3f(getUniformLocation(name), x, y, z) );
		}

		void Shader::setVector4f(const GLchar * name, const xmaths::vec4 & vector)
		{
			GLCall( glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w) );
		}

		void Shader::setVector4f(const GLchar * name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
		{
			GLCall( glUniform4f(getUniformLocation(name), x, y, z, w) );
		}

		void Shader::setMat4f(const GLchar * name, const xmaths::mat4 & matrix)
		{
			GLCall( glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.data) );
		}

	}
}