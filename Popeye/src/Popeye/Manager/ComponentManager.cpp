#include "ComponentManager.h"
#include "../Component/RenderingComponents.h"
#include "../Component/PhysicsComponents.h"
#include "../Component/UIComponents.h"
#include "../Component/ScriptComponents.h"

namespace Popeye
{
	void ComponentManager::InitComponents()
	{
		//Rendering Components
		RegistComponent<Camera>();
		RegistComponent<MeshRenderer>();
		RegistComponent<Light>();

		//Physics Components
		RegistComponent<BoxCollider>();
		RegistComponent<Rigidbody>();

		//UI Components
		RegistComponent<UIFrame>();
		RegistComponent<Text>();
		//RegistComponent<Button>();

		//Script Component
		RegistComponent<ScriptComponent>();
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
	void ComponentManager::AddDataOfComponentByName(std::string component, std::string& type, int& index)
	{
		if (componentDatas.find(component) != componentDatas.end())
		{
			if (component == "Camera")
			{
				type = component;
				index = AccessComponent<Camera>(componentDatas[type])->AddData();
			}
			else if (component == "MeshRenderer")
			{
				type = component;
				index = AccessComponent<MeshRenderer>(componentDatas[type])->AddData();
			}
			else if (component == "Light")
			{
				type = component;
				index = AccessComponent<Light>(componentDatas[type])->AddData();
			}
			else if (component == "BoxCollider")
			{
				type = component;
				index = AccessComponent<BoxCollider>(componentDatas[type])->AddData();
			}
			else if (component == "Rigidbody")
			{
				type = component;
				index = AccessComponent<Rigidbody>(componentDatas[type])->AddData();
			}
			else if (component == "UIFrame")
			{
				type = component;
				index = AccessComponent<UIFrame>(componentDatas[type])->AddData();
			}
			else if (component == "Text")
			{
				type = component;
				index = AccessComponent<Text>(componentDatas[type])->AddData();
			}
		}
	}
}