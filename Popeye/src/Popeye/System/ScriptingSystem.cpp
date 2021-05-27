#include "ScriptingSystem.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ComponentManager.h"

#include "../Scene/Scene.h"
#include "../Scene/GameObject.h"

#include "../Component/ScriptComponents.h"

namespace Popeye 
{
	void ScriptingSystem::SystemInit(bool* _onPlay) { onPlay = _onPlay;  }

	void ScriptingSystem::SystemRunning()
	{
		// just test
		//Init at play
		//static int count = 0;

		//if (onPlay[0])
		//{
		//	//While playing
		//	for (GameObject* gameObject : SceneManager::GetInstance()->currentScene->gameObjects)
		//	{
		//		int id = gameObject->GetID();
		//		if (id == 10)
		//		{
		//			count++;
		//			if (count < 20)
		//			{
		//				gameObject->transform.position.y += 0.1f;


		//			}
		//			else if (count >= 20 && count < 40)
		//			{
		//				gameObject->transform.position.y -= 0.1f;
		//			}
		//			else
		//			{
		//				count = 0;
		//			}
		//		}

		//}
		//	/*if (SceneManager::GetInstance()->currentScene->CheckIfThereIsData<ScriptComponent>(id))
		//	{
		//		gameObject->GetComponent<ScriptComponent>().Run();
		//	}*/
		//}

		//On End
	}
}
