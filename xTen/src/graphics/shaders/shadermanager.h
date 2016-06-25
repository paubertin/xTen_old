#pragma once

#include <vector>
#include "shader.h"

namespace xten { namespace xgraphics {

	class ShaderManager
	{
	private:
		static std::vector<Shader*> m_Shaders;
	public:
		// Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. 
		//If geompath is not nullptr, it also loads a geometry shader
		static Shader* add(std::string name, const GLchar* vertexpath, const GLchar* fragpath, const GLchar *geompath = nullptr);
		// Retrieves a stored sader
		static Shader* get(const std::string &name);
		// Properly de-allocates all loaded resources
		static void clean();
	private:
		ShaderManager() { }
		~ShaderManager() { clean(); }
	};

} }