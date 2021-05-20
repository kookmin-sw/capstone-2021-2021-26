#pragma once

namespace Popeye{
	struct Material
	{
		std::string id;
		unsigned int textureID;
		glm::vec3 color;
		glm::vec3 amb_diff_spec;
		float shininess;

		Material();
		Material(std::string _id);
	};
}