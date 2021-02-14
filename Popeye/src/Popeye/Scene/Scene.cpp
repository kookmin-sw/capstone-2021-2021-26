#include "pch.h"
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
		GameObject* gameObject = new GameObject();
		gameObjects.push_back(gameObject);
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