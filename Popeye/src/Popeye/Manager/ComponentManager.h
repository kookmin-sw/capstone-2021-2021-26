#pragma once

namespace Popeye {
	
	class BaseComponentDatatable
	{
	public:
		virtual ~BaseComponentDatatable() = default;
	};

	template<class component>
	class ComponentDatatable : public BaseComponentDatatable
	{
	private:
		std::vector<component> componentDatatable;
	public:
		component& GetData(int key)
		{
			return componentDatatable[key];
		}

		int AddData()
		{
			component data;
			componentDatatable.push_back(data);

			return componentDatatable.size() - 1;
		}

		std::vector<component> GetAllData()
		{
			return componentDatatable;
		}
		
	};

	class ComponentManager
	{
	private:
		std::unordered_map<const char*, BaseComponentDatatable*> componentDatas;

	private:
		template<typename component>
		ComponentDatatable<component>* AccessComponent(BaseComponentDatatable* _basedata)
		{
			ComponentDatatable<component>* compenentData = static_cast<ComponentDatatable<component>*> (_basedata);
			return compenentData;
		}

	public:
		void InitComponents();
		void AddDataOfComponentByName(const char* component, const char*& type, int& index);

		template<typename component>
		void RegistComponent()
		{
			const char* componentType = typeid(component).name() + 15;
			if (componentDatas.find(componentType) == componentDatas.end())
			{
				componentDatas[componentType] = new ComponentDatatable<component>();
			}
		}
		
		template<typename component>
		void AddDataOfComponent(const char*& type, int& index)
		{
			const char* componentType = typeid(component).name() + 15;
			if (componentDatas.find(componentType) != componentDatas.end())
			{
				type = componentType;
				index = AccessComponent<component>(componentDatas[componentType])->AddData();
			}
		}

		template<typename component>
		component& GetDataOfComponent(const const char*& type, const int& index)
		{
			return AccessComponent<component>(componentDatas[type])->GetData(index);
		}

		template<typename component>
		std::pair<const char*, std::vector<component>> GetAllDataOfComponent()
		{
			const char* componentType = typeid(component).name() + 15;
			std::vector<component> components= AccessComponent<component>(componentDatas[componentType])->GetAllData();
			
			std::pair<const char*, std::vector<component>> compNname = std::make_pair(componentType, components);

			return compNname;
		}
	
	private:
		static ComponentManager* instance;
		ComponentManager();
		~ComponentManager();

	public:
		static ComponentManager* GetInstance();
		static void DestroyInstance();
	};
}