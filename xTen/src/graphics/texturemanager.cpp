#include <iostream>

#include "../utils/utils.h"
#include "texturemanager.h"

namespace xten { namespace xgraphics {

	// Instantiate static variables
	std::vector<Texture2D*>	TexManager::m_Textures;

	Texture2D * TexManager::add(GLenum textureTarget, const std::string & name, const std::string & filename, GLboolean alpha)
	{
		for (Texture2D* texture : m_Textures)
		{
			if (texture->getName() == name)
			{
				std::string err = "[TexManager]. Texture " + name + " already exists.";
				XTEN_WARN(err);
				return nullptr;
			}
		}

		Texture2D* texture = XNEW Texture2D(textureTarget, name);

		if (alpha)
		{
			texture->m_Internal_Format = GL_RGBA;
			texture->m_Image_Format = GL_RGBA;
		}

		GLsizei width, height;
		GLuint bits;
		//unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, texture->m_Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
		//bits = 32;
		BYTE* image = load_image(filename.c_str(), &width, &height, &bits);


		//texture->m_TID = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (!image)
		{
			std::string err = "[TexManager]. SOIL loading image error: " + filename + SOIL_last_result();
			XTEN_ERROR(err);
			XDEL(texture);
			return nullptr;
		}

		texture->generate(width, height, bits, image);

		m_Textures.push_back(texture);

		std::string info = "[TexManager]. Added new texture:" + name + " (" + filename + ")";
		XTEN_INFO(info);

		XDEL_ARRAY(image);

		return texture;
	}

	Texture2D * TexManager::get(const std::string & name)
	{
		for (Texture2D* texture : m_Textures)
		{
			if (texture->getName() == name)
			{
				return texture;
			}
		}
		return nullptr;
	}

	void TexManager::clean()
	{
		XDEL_VEC(m_Textures);
	}

} }