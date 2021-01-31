#pragma once
namespace Popeye {
	struct Transform
	{
		Transform();

		static std::unordered_map<int, glm::vec3> position; // id, id's postion ...
		static std::unordered_map<int, glm::vec3> rotation;
		static std::unordered_map<int, glm::vec3> scale;

		static void ComponentAdded(int);
		static void Get_info(int);
		static void Set_pos(int, glm::vec3);
		static void Set_rotation(int, glm::vec3);
		static void Set_scale(int, glm::vec3);
	};
}

