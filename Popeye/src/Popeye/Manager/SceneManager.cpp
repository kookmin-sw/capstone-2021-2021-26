#include "SceneManager.h"
#include "../Scene/Scene.h"

namespace Popeye {

	//SceneManager
	SceneManager::SceneManager()
	{
		currentScene = nullptr;
	}

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

	void SceneManager::CreateScene()
	{
		if (currentScene == nullptr)
			currentScene = new Scene("Scene");
	}

	void SceneManager::UnloadScene()
	{
		delete(currentScene);
	}
}