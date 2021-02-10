#include "pch.h"
#include "EventSystem.h"
#include "../Events/MouseEvent.h"
#include "../Events/EventManager.h"

namespace Popeye
{

	void EventSystem::SystemRunning()
	{
		//POPEYE_CORE_INFO("{0}", EventManager::GetInstance()->GetState());
		if (eventstate != EventManager::GetInstance()->GetState())
		{
			eventstate = EventManager::GetInstance()->GetState();
			POPEYE_CORE_INFO("{0}", EventManager::GetInstance()->GetState());
		}
	}

	void EventSystem::Init()
	{

	}

	void EventSystem::OnUpdate()
	{

	}

	void EventSystem::OnExit()
	{

	}
}
