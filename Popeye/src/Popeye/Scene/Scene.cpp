#include "pch.h"
#include "Scene.h"
#include "../Component/ComponentManager.h"
#include "GameObject.h"


namespace Popeye {
	//Scene
	Scene::Scene() 
	{
		componentManager = new ComponentManager();

		componentManager->InitComponents();
	}
	
	Scene::~Scene()
	{
		delete(componentManager);
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
}