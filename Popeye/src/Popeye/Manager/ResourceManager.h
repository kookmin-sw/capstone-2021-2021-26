#pragma once

#include "../Resource/Mesh.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"
namespace Popeye {

	class ResourceManager
	{
	public:
		std::vector<Mesh> meshes;
		std::vector<Texture> textures;
		std::vector<Material> materials;
		std::vector<unsigned int> offsets;

		ResourceManager();
		~ResourceManager();

		void SetResources();
		void SetTexture(unsigned char* buffer, unsigned int offset, unsigned int length);
		void SetMesh(unsigned char* buffer, unsigned int offset, unsigned int length);
		void SetMaterial(unsigned char* buffer, unsigned int offset, unsigned int length);

		void AddMaterial();
		//void Set3DModel();

	};
}

