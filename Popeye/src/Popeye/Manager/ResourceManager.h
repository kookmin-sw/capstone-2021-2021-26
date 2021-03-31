#pragma once
#include "../Resource/Texture.h"
namespace Popeye {
	
	struct Mesh;
	struct Textre;
	struct Material;

	class ResourceManager
	{
	public:
		std::vector<Mesh> meshes;
		std::vector<Textre> textures;
		std::vector<Material> materials;

		void SetResources();
	};
}

