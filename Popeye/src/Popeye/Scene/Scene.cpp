#include "pch.h"
#include "Scene.h"

namespace Popeye {

	//SceneManager
	SceneManager::SceneManager()
	{}

	SceneManager::~SceneManager()
	{}

	SceneManager* SceneManager::instance = nullptr;

	SceneManager* SceneManager::GetInstance()
	{
		if (!instance)
		{
			instance = new SceneManager;
		}
		return instance;
	}

	void SceneManager::DestroyInstance()
	{
		if (!instance)
		{
			return;
		}
		delete instance;
		instance = nullptr;
	}

	int SceneManager::SceneCreated(Scene& scene)
	{
		//this->scene_IDs.push_back((int)&scene);
		return (int)&scene;
	}

	//Scene
	Scene::Scene()
	{
		this->scene_id = SceneManager::GetInstance()->SceneCreated(*this);
		SceneManager::GetInstance()->currentScene = this;
	}

	Scene::~Scene()
	{
	}

	void Scene::GameObjectDeleted(GameObject& gameobject)
	{
	}
}