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
					std::string err = "[ShaderManager]. Shader " + name + " already exists.";
					XTEN_WARN(err);
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
				std::string err = "[ShaderManager]. Failed to read vertex shader file: " + std::string(vertexpath);
				XTEN_ERROR(err);
			}
			try
			{
				fragsrc = read_file(fragpath);
			}
			catch (std::exception e)
			{
				std::string err = "[ShaderManager]. Failed to read fragment shader file: " + std::string(fragpath);
				XTEN_ERROR(err);
			}
			if (geompath != nullptr)
			{
				try
				{
					geomsrc = read_file(geompath);
				}
				catch (std::exception e)
				{
					std::string err = "[ShaderManager]. Failed to read geometry shader file: " + std::string(geompath);
					XTEN_ERROR(err);
				}
			}
			const GLchar *vShader = vertsrc.c_str();
			const GLchar *fShader = fragsrc.c_str();
			const GLchar *gShader = geomsrc.c_str();

			// 2. Now create shader object from source code
			Shader* shader = XNEW Shader(name);
			if (!(shader->compile(vShader, fShader, geompath != nullptr ? gShader : nullptr)))
			{
				XTEN_ERROR("[ShaderManager]. Failed creating shader object.");
				XDEL(shader);
				return nullptr;
			}
			else
			{
				m_Shaders.push_back(shader);
				std::string info = "[ShaderManager]. Created shader object: " + name;
				XTEN_INFO(info);
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
			XDEL_VEC(m_Shaders);
		}


	}
}