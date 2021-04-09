#pragma once
#include "../Resource/Mesh.h"
#include "../Resource/Material.h"
#include "../Resource/Texture.h"

namespace Popeye {

	class ResourceManager
	{
	public:
		std::vector<Mesh> meshes;
		std::vector<Texture> textures;
		std::vector<Material> materials;

		ResourceManager();
		~ResourceManager();

		void SetResources();
		void SetTextureResources();
		void Set3DModelResources();

	};
}

