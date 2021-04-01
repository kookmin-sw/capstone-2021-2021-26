#pragma once
#include "../Resource/Texture.h"
#include "../Resource/Mesh.h"
namespace Popeye {
	
	struct Msh;
	struct Textre;
	struct Material;

	class ResourceManager
	{
	public:
		std::vector<Mesh> meshes;
		std::vector<Textre> textures;
		std::vector<Material> materials;

		void SetResources();
		void SetTextureResources();
		void Set3DModelResources();
	};
}

