#pragma once
#include "pch.h"
namespace Popeye {

	struct testComponent0 { int x = 0; int y = 1; int z = 3; };
	struct testComponent1 { bool blahblah = true; };
	struct testComponent2 {};
	struct testComponent3 {};
	struct testComponent4 {};

	class BaseDatatable
	{
	public:
		virtual ~BaseDatatable() = default;
	};

	template<class component>
	class ComponentDatatable : public BaseDatatable
	{
	private:
		std::vector<component> componentDatatable;
	public:
		component GetData(int key)
		{
			return componentDatatable[key];
		}

		component SetType()
		{
			return component;
		}
	};

	class ComponentManager
	{
	private:
		std::unordered_map<const char*, BaseDatatable*> componentDatas;
	private:
		template<typename component>
		ComponentDatatable<component>* accessComponent(BaseDatatable* basedata)
		{
			ComponentDatatable<component>* compenentData = dynamic_cast<ComponentDatatable<component>*> basedata;
			return componentData;
		}
	public:
		template<typename component>
		void RegistComponent()
		{
			const char* componentType = typeid(component).name();

			if (componentDatas.find(componentType) == componentDatas.end())
				componentDatas[componentType] = new ComponentDatatable<component>();
		}
		
		template<typename component>
		void AddDataToComponent(int id)
		{
			const char* componentType = typeid(component).name();
			if (componentDatas.find(componentType) != componentDatas.end())
			{
				std::cout << typeid(accessComponent<component>(componentDatas[componentType])->SetType()).name() << std::endl;
			}
			else
			{
				std::cout << "there is no such component" << std::endl;
			}
		}

		void InitComponents()
		{
			RegistComponent<testComponent0>();
			RegistComponent<testComponent1>();
			RegistComponent<testComponent2>();
			RegistComponent<testComponent3>();
		}
	};
}