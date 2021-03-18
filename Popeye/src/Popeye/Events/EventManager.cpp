#include "EventManager.h"

namespace Popeye {
	EventManager::EventManager(){}
	EventManager::~EventManager(){}

	EventManager* EventManager::instance = nullptr;

	EventManager* EventManager::GetInstance()
	{
		if (!instance)
		{
			instance = new EventManager;
		}
		return instance;
	}

	void EventManager::DestroyInstance()
	{
		if (!instance)
		{
			return;
		}
		delete instance;
		instance = nullptr;
	}

	EventMod EventManager::GetState() const
	{
		return instance->eventstate;
	}

	void EventManager::SetState(EventMod state)
	{
		instance->eventstate = state;
	}
}