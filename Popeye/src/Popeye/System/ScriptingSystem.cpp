#include "ScriptingSystem.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ComponentManager.h"

#include "../Scene/Scene.h"
#include "../Scene/GameObject.h"

#include "../Component/ScriptComponents.h"

namespace Popeye 
{
	void ScriptingSystem::SystemInit(){}

	void ScriptingSystem::SystemRunning()
	{
		//Init at play



		//While playing
		for (GameObject* gameObject : SceneManager::GetInstance()->currentScene->gameObjects)
		{
			int id = gameObject->GetID();
			if (SceneManager::GetInstance()->currentScene->CheckIfThereIsData<ScriptComponent>(id))
			{
				gameObject->GetComponent<ScriptComponent>().Run();
			}
		}

		//On End
	}
}
