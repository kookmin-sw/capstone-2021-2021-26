#include "ComponentManager.h"
#include "../Component/RenderingComponents.h"

namespace Popeye
{
	void ComponentManager::InitComponents()
	{
		//Rendering Components
		RegistComponent<Camera>();
		RegistComponent<MeshRenderer>();
		RegistComponent<Light>();

		//Physics Components
		//RegistComponent<Collider>();
		//RegistComponent<Rigidbody>();

		//UI Components
		//RegistComponent<Frame>();
		//RegistComponent<Button>();
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

	// terrible way to find component fix it later
	void ComponentManager::AddDataOfComponentByName(const char* component, const char*& type, int& index)
	{
		if (component[0] == (typeid(Camera).name() + 15)[0])
		{
			type = typeid(Camera).name();
			index = AccessComponent<Camera>(componentDatas[type])->AddData();
		}
		else if (component[0] == (typeid(MeshRenderer).name() + 15)[0])
		{
			type = typeid(MeshRenderer).name();
			index = AccessComponent<MeshRenderer>(componentDatas[type])->AddData();
		}
		else if (component[0] == (typeid(Light).name() + 15)[0])
		{
			type = typeid(Light).name();
			index = AccessComponent<Light>(componentDatas[type])->AddData();
		}
	}
}