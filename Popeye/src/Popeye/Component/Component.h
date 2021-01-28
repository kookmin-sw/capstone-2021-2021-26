#pragma once
namespace Popeye {
	class Transform
	{
	public:
		Transform();
		static std::unordered_map<int, glm::vec3> position; // id, id's postion ...
		static std::unordered_map<int, glm::vec3> rotation;
		static std::unordered_map<int, glm::vec3> scale;

		static void componentAdded(int);
		static void get_info(int);

	};

	struct MeshRenderer
	{
		//std::vector<size_t, > gameObjectIDs;
	};
}

