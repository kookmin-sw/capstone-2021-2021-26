#include "pch.h"
#include "Scene.h"

namespace Popeye {
	//Scene
	Scene::Scene()
	{
		this->scene_id = SceneManager::GetInstance()->SceneCreated(*this);
		SceneManager::GetInstance()->currentScene = this;
	}

	Scene::~Scene()
	{
	}

	void Scene::GameObjectCreated(int id)
	{
		gameobject_IDs.push_back(id);
	}

	void Scene::GameObjectDeleted(int gameobject)
	{
	}
}