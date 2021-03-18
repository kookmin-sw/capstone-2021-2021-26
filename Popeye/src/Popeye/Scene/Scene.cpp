#include "Scene.h"
#include "GameObject.h"


namespace Popeye {
	//Scene
	Scene::Scene() 
	{
	}
	
	Scene::~Scene()
	{
	}

	void Scene::CreateGameObject()
	{
		int newID = 0;
		if (!reusableIDs.empty()){
			newID = reusableIDs.front();
			reusableIDs.pop();
		}
		else{
			newID = gameObjectID;
			gameObjectID++;
		}

		GameObject* gameObject = new GameObject(newID);
		gameObjects.push_back(gameObject);

		if (newID + 1 <= keysToAccessComponent.size())
		{
			ResetAccessor(newID);
		}
		else
		{
			std::vector<Accessor> newAccessor;
			keysToAccessComponent.push_back(newAccessor);
		}

	}

	void Scene::DeleteGameObject(int _id)
	{
		/*do some cleaning work*/
		reusableIDs.push(_id);
	}

	void Scene::SetName(char* name)
	{
		sceneName = name;
	}

	char* Scene::GetName()
	{
		return sceneName;
	}

	void Scene::ResetAccessor(int id)
	{
		for (int i = 0; i < keysToAccessComponent[id].size(); i++)
		{
			keysToAccessComponent[id][i].Reset();
		}
	}
	
	std::vector<const char*> Scene::GetAllComponents(int _id)
	{
		std::vector<const char*> components;

		for (int i = 0; i < keysToAccessComponent[_id].size(); i++)
		{
			components.push_back(keysToAccessComponent[_id][i].componentType);
		}
		return components;
	}

	std::vector<Accessor> Scene::GetAllAddressOfID(int _id)
	{
		return keysToAccessComponent[_id];
	}
}