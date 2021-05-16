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
		std::queue<int> recycleIndex;
	public:
		component& GetData(int index)
		{
			return componentDatatable[index];
		}

		int AddData()
		{
			int index = 0;
			if (recycleIndex.empty())
			{
				component data;
				componentDatatable.push_back(data);
				index = componentDatatable.size() - 1;
			}
			else
			{
				index = recycleIndex.front();
				recycleIndex.pop();
			}

			return index;
		}

		void RemoveData(int index)
		{
			recycleIndex.push(index);
			componentDatatable[index].SetValue();
		}

		std::vector<component> GetAllData(){return componentDatatable;}

		std::queue<int> GetRecycleQ(){return recycleIndex;}

		void SetAllData(std::vector<component> &components)
		{
			int comp_size = components.size(), curr_size = componentDatatable.size();
			for (int i = 0; i < comp_size; i++)
			{
				if (i < curr_size)
				{
					componentDatatable[i] = components[i];
				}
				else
				{
					componentDatatable.push_back(components[i]);
				}

			}
		}

		void SetRecycleQ(std::queue<int> &q)
		{
			while (!recycleIndex.empty())
			{
				recycleIndex.pop();
			}
			while (!q.empty())
			{
				recycleIndex.push(q.front());
				q.pop();
			}
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
		void RemoveDataOfComponent(const char* type, int index)
		{
			if (componentDatas.find(type) != componentDatas.end())
			{
				AccessComponent<component>(componentDatas[type])->RemoveData(index);
			}
		}

		template<typename component>
		component& GetDataOfComponent(const const char*& type, const int& index)
		{
			return AccessComponent<component>(componentDatas[type])->GetData(index);
		}

		template<typename component>
		std::pair<std::vector<component>, std::queue<int>> GetAllDataOfComponent()
		{
			const char* componentType = typeid(component).name() + 15;
			std::vector<component> components= AccessComponent<component>(componentDatas[componentType])->GetAllData();
			std::queue<int> recycleIndexQ = AccessComponent<component>(componentDatas[componentType])->GetRecycleQ();

			std::pair<std::vector<component>, std::queue<int>> compNname = std::make_pair(components, recycleIndexQ);

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