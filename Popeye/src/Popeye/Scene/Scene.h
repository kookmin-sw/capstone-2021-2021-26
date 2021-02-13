#pragma once

namespace Popeye {
	
	class GameObject;
	
	class Scene //got gameobjects ID lists, if gameobject create, regist id. if gameobject deleted, 
	{
	private:
		int scene_id;
	public:
		std::vector<GameObject*> gameObjects; // root gameobjects at scene
	public:
		Scene();
		~Scene();
		void CreateGameObject();
		void DeleteGameObject(int);
	};
}
