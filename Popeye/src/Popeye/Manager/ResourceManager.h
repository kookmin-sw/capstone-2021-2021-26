#pragma once

#include "../Resource/Mesh.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"

struct aiScene;
struct aiNode;
struct aiMesh;

namespace Popeye {

	class ResourceManager
	{
	public:
		std::vector<Model> models;
		std::vector<Mesh> meshes;
		std::vector<Texture> textures;
		std::vector<Material> materials;
		std::vector<unsigned int> offsets;

		ResourceManager();
		~ResourceManager();

		void SetResources();
		void SetTexture(unsigned char* buffer, unsigned int offset, unsigned int length);
		void SetModel(unsigned char* buffer, unsigned int offset, unsigned int length);
		void SetModelRecursively(aiNode* node, const aiScene* scene, Model& model);
		void SetMesh(const aiScene* scene);
		void SetMesh(aiMesh* mesh, Model& model);
		bool CheckMesh(float* id, int& index);
		//bool IsMeshHere(aiMesh* mesh);
		void SetMesh(unsigned char* buffer, unsigned int offset, unsigned int length);
		void SetMaterial(unsigned char* buffer, unsigned int offset, unsigned int length);

		void AddMaterial();
		//void Set3DModel();

	};
}

