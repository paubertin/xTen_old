#pragma once

#include <vector>

#include "texture2d.h"

namespace xten { namespace xgraphics {

	class TexManager
	{
	private:
		static std::vector<Texture2D*> m_Textures;
	public:
		// Loads (and generates) a texture from file
		static Texture2D* add(GLenum TextureTarget, const std::string & name, const std::string & filename, GLboolean alpha);
		// Retrieves a stored texture
		static Texture2D* get(const std::string &name);
		// Properly de-allocates all loaded resources
		static void clean();
	private:
		TexManager() { }
		~TexManager() { clean(); }
	};

} }