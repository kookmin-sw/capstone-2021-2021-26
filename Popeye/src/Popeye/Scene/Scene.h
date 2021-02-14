#pragma once

namespace Popeye {
	
	class GameObject;
	
	class Scene //got gameobjects ID lists, if gameobject create, regist id. if gameobject deleted, 
	{
	private:
		int sceneID;
		char* sceneName;
	public:
		std::vector<GameObject*> gameObjects; // root gameobjects at scene
	public:
		Scene();
		~Scene();
		void CreateGameObject();
		void DeleteGameObject(int);

		char* GetName();
		void SetName(char* );
	};
}
