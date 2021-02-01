#include "pch.h"
#include "Transform.h"

namespace Popeye {
	std::unordered_map<int, glm::vec3> Transform::position;
	std::unordered_map<int, glm::vec3> Transform::rotation;
	std::unordered_map<int, glm::vec3> Transform::scale;
	
	Transform::Transform() {}

	void Transform::ComponentAdded(int id)
	{
		if (position.find(id) == position.end())
		{
			position[id] = { 0.0f, 0.0f, 0.0f };
			rotation[id] = { 0.0f, 0.0f, 0.0f };
			scale[id] = { 1.0f, 1.0f, 1.0f };
		}
	}

	void Transform::Get_info(int id)
	{
		std::cout << position[id].x << " " << position[id].y << " " << position[id].z << std::endl;
		std::cout << rotation[id].x << " " << rotation[id].y << " " << rotation[id].z << std::endl;
		std::cout << scale[id].x << " " << scale[id].y << " " << scale[id].z << std::endl;
	}

	void Transform::Set_pos(int id, glm::vec3 _position)
	{
		if (position.find(id) != position.end())
		{
			position[id] = _position;
		}
	}
	void Transform::Set_rotation(int id, glm::vec3 _rotation)
	{
		if (position.find(id) != position.end())
		{
			rotation[id] = _rotation;
		}
	}
	void Transform::Set_scale(int id, glm::vec3 _scale)
	{
		if (position.find(id) != position.end())
		{
			scale[id] = _scale;
		}
	}

}
