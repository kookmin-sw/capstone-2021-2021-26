#pragma once

namespace Popeye {
	class ComponentManager;
	class GameObject;
	
	class Scene //got gameobjects ID lists, if gameobject create, regist id. if gameobject deleted, 
	{
	private:
		int	sceneID;
		char* sceneName;
		ComponentManager* componentManager;
	private:
		int gameObjectID = 0;
		std::queue<int> reusableIDs;
		std::vector <int> gameObjectIDs;
	public:
		int mainCameraID;
		std::vector <GameObject*> gameObjects;		// root gameobjects at scene
	public:
		Scene();
		~Scene();
		
		void CreateGameObject();
		void DeleteGameObject(int);

		template<class component>
		void AddData(int _id)
		{
			componentManager->AddDataToComponent<component>(_id);
		}
		
		template<class component>
		component GetData(int _id) // gameobject's data
		{
			return componentManager->GetDataToComponent<component>(_id);
		}

		char* GetName();
		void SetName(char* );
	};
}
