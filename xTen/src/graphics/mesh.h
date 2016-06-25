#pragma once

#include <string>
#include <vector>

#include <GL\glew.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "vertex.h"
#include "texture2d.h"

namespace xten { namespace xgraphics {

	class Mesh
	{
	public:
		Mesh();
		~Mesh();
		
		GLboolean loadMesh(const std::string & filename);
		void render();

	private:
		GLboolean initFromScene(const aiScene* scene, const std::string & filename);
		void initMesh(GLuint index, const aiMesh* aimesh);
		GLboolean initMaterials(const aiScene* scene, const std::string & filename);
		void clear();

		struct MeshEntry
		{
			MeshEntry();
			~MeshEntry();

			void init(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

			GLuint m_VAO, m_VBO, m_IBO;
			GLuint numIndices, materialIndex;
		};

		std::vector<MeshEntry> m_Entries;
		std::vector<Texture2D*> m_Textures;

	};

} }