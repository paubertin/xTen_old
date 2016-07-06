#include "mesh.h"
#include "../utils/utils.h"
#include "../maths/xmaths.h"
#include "texturemanager.h"

namespace xten { namespace xgraphics {

	Mesh::MeshEntry::MeshEntry() :
		m_VAO(NULL), m_VBO(NULL), m_IBO(NULL), numIndices(NULL), materialIndex(NULL)
	{
	}

	Mesh::MeshEntry::~MeshEntry()
	{
		GLCall( glDeleteBuffers(1, &m_VBO) );
		GLCall( glDeleteBuffers(1, &m_IBO) );
	}

	void Mesh::MeshEntry::init(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
	{
		numIndices = indices.size();

		GLCall( glGenVertexArrays(1, &m_VAO) );
		GLCall( glGenBuffers(1, &m_VBO) );
		GLCall( glGenBuffers(1, &m_IBO) );

		GLCall( glBindVertexArray(m_VAO) );

		GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_VBO) );
		GLCall( glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW) );

		GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO) );
		GLCall( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numIndices, &indices[0], GL_STATIC_DRAW) );

		//positions
		GLCall( glEnableVertexAttribArray(0) );
		GLCall( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(0)) );
		//texcoord
		GLCall( glEnableVertexAttribArray(1) );
		GLCall( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, Vertex::uv))) );
		//normal
		GLCall( glEnableVertexAttribArray(2) );
		GLCall( glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, Vertex::normal))) );
		
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		GLCall( glBindVertexArray(0) );
	}

	Mesh::Mesh()
	{}

	Mesh::~Mesh()
	{
		clear();
	}

	void Mesh::clear()
	{
		for (GLuint i = 0; i < m_Textures.size(); ++i)
		{
			// XDEL(m_Textures[i]);
		}
	}

	GLboolean Mesh::loadMesh(const std::string & filename)
	{
		clear();

		GLboolean res = GL_FALSE;

		Assimp::Importer import;
		
		const aiScene* scene = import.ReadFile(filename.c_str(), 
									aiProcess_FlipUVs |
									aiProcess_GenSmoothNormals | 
									aiProcess_Triangulate);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::string err = "[ASSIMP]. Could not load file: " + filename + "\n" + import.GetErrorString();
			XTEN_ERROR(err);
			return res;
		}
		else
		{
			res = initFromScene(scene, filename);
		}

		return res;
	}

	void Mesh::render()
	{
		for (GLuint i = 0; i < m_Entries.size(); i++)
		{
			const GLuint MaterialIndex = m_Entries[i].materialIndex;

			if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex])
			{
				m_Textures[MaterialIndex]->bind(GL_TEXTURE0);
			}

			GLCall( glBindVertexArray(m_Entries[i].m_VAO) );
			GLCall( glDrawElements(GL_TRIANGLES, m_Entries[i].numIndices, GL_UNSIGNED_INT, NULL) );
			GLCall( glBindVertexArray(0) );

		}
	}

	GLboolean Mesh::initFromScene(const aiScene* scene, const std::string & filename)
	{
		m_Entries.resize(scene->mNumMeshes);
		m_Textures.resize(scene->mNumMaterials);

		for (GLuint i = 0; i < m_Entries.size(); ++i)
		{
			const aiMesh* aimesh = scene->mMeshes[i];
			initMesh(i, aimesh);
		}

		return initMaterials(scene, filename);
	}

	void Mesh::initMesh(GLuint index, const aiMesh* aimesh)
	{
		using namespace xmaths;

		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		for (GLuint i = 0; i < aimesh->mNumVertices; i++)
		{
			vec3 position(aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z);
			vec3 normal(aimesh->mNormals[i].x, aimesh->mNormals[i].y, aimesh->mNormals[i].z);
			vec2 texcoord = aimesh->HasTextureCoords(0) ? vec2(aimesh->mTextureCoords[0][i].x, aimesh->mTextureCoords[0][i].y) : vec2(0.f);
		
			vertices.push_back(Vertex(vec3(aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z),
									  aimesh->HasTextureCoords(0) ? vec2(aimesh->mTextureCoords[0][i].x, aimesh->mTextureCoords[0][i].y) : vec2(0.f),
								      vec3(aimesh->mNormals[i].x, aimesh->mNormals[i].y, aimesh->mNormals[i].z)));
		}

		for (GLuint i = 0; i < aimesh->mNumFaces; i++)
		{
			const aiFace& face = aimesh->mFaces[i];
			XASSERT(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		m_Entries[index].materialIndex = aimesh->mMaterialIndex;
		m_Entries[index].init(vertices, indices);
	}

	GLboolean Mesh::initMaterials(const aiScene* scene, const std::string & filename)
	{
		// Extract the directory part from the file name
		std::string::size_type SlashIndex = filename.find_last_of("/");
		std::string Dir;

		if (SlashIndex == std::string::npos) {
			Dir = ".";
		}
		else if (SlashIndex == 0) {
			Dir = "/";
		}
		else {
			Dir = filename.substr(0, SlashIndex);
		}

		GLboolean res = GL_TRUE;

		for (GLuint i = 0; i < scene->mNumMaterials; i++)
		{
			const aiMaterial * mat = scene->mMaterials[i];

			m_Textures[i] = nullptr;

			if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString path;

				if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					std::string fullpath = Dir + "/" + path.data;
					m_Textures[i] = TexManager::add(GL_TEXTURE_2D, path.data, fullpath, GL_TRUE);
				}
			}

			if (!m_Textures[i])
			{
				m_Textures[i] = TexManager::get("default_white");
			}

			if (!m_Textures[i])
			{
				res = GL_FALSE;
			}
		}

		return res;
	}

} }