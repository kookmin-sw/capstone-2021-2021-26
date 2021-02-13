#include "pch.h"
#include "Scene.h"
#include "SceneManger.h"
#include "GameObject.h"

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

	void Scene::CreateGameObject()
	{
		GameObject* gameObject = new GameObject();
		gameObjects.push_back(gameObject);
	}

}