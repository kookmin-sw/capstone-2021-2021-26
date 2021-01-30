#include "pch.h"
#include "Transform.h"
namespace Popeye {
	std::unordered_map<int, glm::vec3> Transform::position;
	std::unordered_map<int, glm::vec3> Transform::rotation;
	std::unordered_map<int, glm::vec3> Transform::scale;
	
	Transform::Transform() {}

	void Transform::componentAdded(int id)
	{
		if (position.find(id) == position.end())
		{
			position[id] = { 0.0f, 0.0f, 0.0f };
			rotation[id] = { 0.0f, 0.0f, 0.0f };
			scale[id] = { 1.0f, 1.0f, 1.0f };
		}
		else
		{
			printf("already exist !!");
		}
	}

	void Transform::get_info(int id)
	{
		std::cout << position[id].x << " " << position[id].y << " " << position[id].z << std::endl;
		std::cout << rotation[id].x << " " << rotation[id].y << " " << rotation[id].z << std::endl;
		std::cout << scale[id].x << " " << scale[id].y << " " << scale[id].z << std::endl;
	}
}
