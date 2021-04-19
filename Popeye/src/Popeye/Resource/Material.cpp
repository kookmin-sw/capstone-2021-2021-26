#include "Material.h"

namespace Popeye{
	Material::Material()
	{
		color = glm::vec3(1.0f);
		textureID = -1;
		amb_diff_spec[0] = 0.8f; //ambient
		amb_diff_spec[1] = 0.4f; //diffuse
		amb_diff_spec[2] = 0.1f; //specular
		shininess = 32.0f;
	}

}