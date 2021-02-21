#include "pch.h"
#include "ComponentManager.h"


namespace Popeye
{
	void ComponentManager::InitComponents()
	{
		RegistComponent<Camera>();
		RegistComponent<MeshRenderer>();
	}

	ComponentManager::ComponentManager()
	{}

	ComponentManager::~ComponentManager()
	{}

	ComponentManager* ComponentManager::instance = nullptr;

	ComponentManager* ComponentManager::GetInstance()
	{
		if (!instance)
		{
			instance = new ComponentManager;
		}
		return instance;
	}

	void ComponentManager::DestroyInstance()
	{
		if (!instance)
		{
			return;
		}
		delete instance;
		instance = nullptr;
	}
}