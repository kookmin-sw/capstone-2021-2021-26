#include "PhysicsSystem.h"

#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/ComponentManager.h"

#include "../Scene/Scene.h"
#include "../Scene/GameObject.h"

#include "../Component/PhysicsComponents.h"

namespace Popeye
{
	void PhysicsSystem::SystemInit(bool* _onPlay)
	{
		onPlay = _onPlay;
	}
	
	void PhysicsSystem::SystemRunning() 
	{
		// test
		//if (onPlay[0])
		//{
		//	for (GameObject* gameObject : SceneManager::GetInstance()->currentScene->gameObjects)
		//	{
		//		int id = gameObject->GetID();
		//		if (SceneManager::GetInstance()->currentScene->CheckIfThereIsData<Rigidbody>(id))
		//		{
		//			Rigidbody rigid = SceneManager::GetInstance()->currentScene->GetData<Rigidbody>(id);
		//			if (rigid.gravity)
		//			{
		//				gameObject->transform.position += glm::vec3(0.0f , -0.1f , 0.0f);
		//			}
		//		}
		//	}
		//	//POPEYE_CORE_INFO("Popeye Physics");
		//}

	}
}