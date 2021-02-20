#pragma once
#include "RenderingComponents.h"

namespace Popeye {
	struct Address
	{
		int gameObjectID , dataIndex;
		Address(int _id, int _index) { gameObjectID = _id; dataIndex = _index; }
	};

	class BaseComponentDatatable
	{
	public:
		int componentID;
		virtual ~BaseComponentDatatable() = default;
	};

	template<class component>
	class ComponentDatatable : public BaseComponentDatatable
	{
	private:
		std::vector<component> componentDatatable;
	public:
		component GetData(int key)
		{
			return componentDatatable[key];
		}

		std::vector<int> AddData()
		{
			component data;
			componentDatatable.push_back(data);

			std::vector<int> tempvector;
			tempvector.push_back(componentID);
			tempvector.push_back(componentDatatable.size() - 1);

			/*POPEYE_CORE_INFO("{0}", typeid(component).name());
			for (int i = 0; i < componentDatatable.size(); i++)
			{
				POPEYE_CORE_INFO(i);
			}*/

			return tempvector;
		}
		
	};

	class ComponentManager
	{
	private:
		std::unordered_map<const char*, BaseComponentDatatable*> componentDatas;
		std::vector<std::vector<Address>> address;
	private:
		template<typename component>
		ComponentDatatable<component>* AccessComponent(BaseComponentDatatable* _basedata)
		{
			ComponentDatatable<component>* compenentData = static_cast<ComponentDatatable<component>*> (_basedata);
			return compenentData;
		}

	public:
		template<typename component>
		void RegistComponent()
		{
			static int id = 0;
			const char* componentType = typeid(component).name();

			if (componentDatas.find(componentType) == componentDatas.end())
			{
				componentDatas[componentType] = new ComponentDatatable<component>();
				componentDatas[componentType]->componentID = id;
				id++;

				std::vector<Address> componentAddress;
				address.push_back(componentAddress);
			}
		}
		
		template<typename component>
		void AddDataToComponent(int id)
		{
			static std::vector<int> compNindex;
			const char* componentType = typeid(component).name();
			if (componentDatas.find(componentType) != componentDatas.end())
			{
				compNindex = AccessComponent<component>(componentDatas[componentType])->AddData();
				Address instaddress(compNindex[1], id);
				address[compNindex[0]].push_back(instaddress);
			}
			else
			{
				std::cout << "there is no such component" << std::endl;
			}
		}

		template<typename component>
		component GetDataToComponent(int id)
		{
			component temp;
			const char* componentType = typeid(component).name();
			ComponentDatatable<component>* castedcomponent = AccessComponent<component>(componentDatas[componentType]);
			int componentID = castedcomponent->componentID;
			for (int i = 0; i < address[componentID].size(); i++)
			{
				if (address[componentID][i].gameObjectID == id)
				{
					component temp = castedcomponent->GetData(address[componentID][i].dataIndex);
					return temp;
				}
			}
			return temp;
		}

		void InitComponents()
		{
			RegistComponent<Camera>();
			RegistComponent<MeshRenderer>();
		}
	};
}