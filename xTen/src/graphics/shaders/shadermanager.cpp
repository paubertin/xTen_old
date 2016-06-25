#include "shadermanager.h"
#include "../../utils/utils.h"

namespace xten {
	namespace xgraphics {

		// Instantiate static variables
		std::vector<Shader*>	ShaderManager::m_Shaders;

		Shader * ShaderManager::add(std::string name, const GLchar * vertexpath, const GLchar * fragpath, const GLchar * geompath)
		{
			for (Shader* shader : m_Shaders)
			{
				if (shader->getName() == name)
				{
					std::cout << "ERROR [ShaderManager]. Shader " << name << " already exists." << std::endl
						<< "-- --------------------------------- --" << std::endl;
					return nullptr;
				}
			}

			// 1. Retrieve the vertex/fragment source code from filePath
			std::string vertsrc, fragsrc, geomsrc;
			try
			{
				vertsrc = read_file(vertexpath);
			}
			catch (std::exception e)
			{
				std::cout << "ERROR [ShaderManager]. Failed to read vertex shader file: " << vertexpath << std::endl
					<< "-- --------------------------------- --" << std::endl;
			}
			try
			{
				fragsrc = read_file(fragpath);
			}
			catch (std::exception e)
			{
				std::cout << "ERROR [ShaderManager]. Failed to read fragment shader file: " << fragpath << std::endl
					<< "-- --------------------------------- --" << std::endl;
			}
			if (geompath != nullptr)
			{
				try
				{
					geomsrc = read_file(geompath);
				}
				catch (std::exception e)
				{
					std::cout << "ERROR [ShaderManager]. Failed to read geometry shader file: " << geompath << std::endl
						<< "-- --------------------------------- --" << std::endl;
				}
			}
			const GLchar *vShader = vertsrc.c_str();
			const GLchar *fShader = fragsrc.c_str();
			const GLchar *gShader = geomsrc.c_str();

			// 2. Now create shader object from source code
			Shader* shader = XNEW Shader(name);
			if (!(shader->compile(vShader, fShader, geompath != nullptr ? gShader : nullptr)))
			{
				std::cout << "ERROR [ShaderManager]. Failed creating shader object. " << std::endl
					<< "-- --------------------------------- --" << std::endl;
				XDEL(shader);
				return nullptr;
			}
			else
			{
				m_Shaders.push_back(shader);
				std::cout << "[ShaderManager]. Created shader object: " << name << std::endl
					<< "-- --------------------------------- --" << std::endl;
				return shader;
			}
		}

		Shader * ShaderManager::get(const std::string & name)
		{
			for (Shader* shader : m_Shaders)
			{
				if (shader->getName() == name)
				{
					return shader;
				}
			}
			return nullptr;
		}

		void ShaderManager::clean()
		{
			for (GLuint i = 0; i < m_Shaders.size(); i++)
			{
				XDEL(m_Shaders[i]);
			}
		}


	}
}