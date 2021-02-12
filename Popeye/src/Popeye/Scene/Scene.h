#pragma once
#include "SceneManger.h"

namespace Popeye {
	
	class GameObject;
	
	class Scene //got gameobjects ID lists, if gameobject create, regist id. if gameobject deleted, 
	{
	private:
		int scene_id;
	public:
		glm::vec3 position;
	public:
		Scene();
		~Scene();
		std::vector<int> gameobject_IDs;
		void GameObjectCreated(int);
		void GameObjectDeleted(int);
	};
}
