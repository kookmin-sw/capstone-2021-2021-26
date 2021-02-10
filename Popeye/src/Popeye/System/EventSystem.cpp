#include "pch.h"
#include "EventSystem.h"
#include "../Events/MouseEvent.h"
#include "../Events/KeyboardEvent.h"

namespace Popeye
{

	void EventSystem::SystemRunning()
	{
		//POPEYE_CORE_INFO("{0}", EventManager::GetInstance()->GetState());
		if (eventstate != EventManager::GetInstance()->GetState())
		{
			eventstate = EventManager::GetInstance()->GetState();
			//POPEYE_CORE_INFO(eventstate);
		}

		//switch (eventstate)
		//{
		//case EventMod::NONE:
		//	break;
		//case EventMod::EDIT:
		//	//printf("edit\n");
		//	break;
		//case EventMod::INPUT:
		//	//printf("input\n");
		//	break;
		//}
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
