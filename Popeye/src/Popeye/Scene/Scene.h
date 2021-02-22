#pragma once

namespace Popeye {
	class ComponentManager;
	class GameObject;

	struct Accessor 
	{
		const char* componentType = nullptr;
		int dataIndex = -1;
		
		Accessor() : componentType(nullptr), dataIndex(-1) {};
		void Reset() { componentType = nullptr; dataIndex = -1; }
	};
	
	class Scene //got gameobjects ID lists, if gameobject create, regist id. if gameobject deleted, 
	{
	private:
		int	sceneID;
		char* sceneName;
		int gameObjectID = 0;
		std::queue<int> reusableIDs;
		std::vector<std::vector<Accessor>> keysToAccessComponent;
		std::vector <int> gameObjectIDs;
	public:
		int mainCameraID;
		std::vector <GameObject*> gameObjects;		// root gameobjects at scene
	
	private:
		void ResetAccessor(int id);
	public:
		Scene();
		~Scene();
		
		void CreateGameObject();
		void DeleteGameObject(int);

		template<class component>
		void AddData(int _id)
		{
			Accessor accessor;
			ComponentManager::GetInstance()->AddDataOfComponent<component>(accessor.componentType, accessor.dataIndex);
			keysToAccessComponent[_id].push_back(accessor);
		}
		
		template<class component>
		component GetData(int _id) // gameobject's data
		{
			const char* componentType = typeid(component).name();
			component temp;
			for (int i = 0; i < keysToAccessComponent[_id].size(); i++)
			{
				if (keysToAccessComponent[_id][i].componentType == componentType)
				{
					temp = ComponentManager::GetInstance()->GetDataOfComponent<component>
						(keysToAccessComponent[_id][i].componentType, keysToAccessComponent[_id][i].dataIndex);
				}
			}
			return temp;
		}

		template<class component>
		bool CheckIfThereIsData(int _id)
		{
			const char* componentType = typeid(component).name() + 15;
			for (int i = 0; i < keysToAccessComponent[_id].size(); i++)
			{
				if (keysToAccessComponent[_id][i].componentType == componentType)
				{
					return true;
				}
			}
			return false;
		}


		void GetAllComponents(int _id)
		{
			for (int i = 0; i < keysToAccessComponent[_id].size(); i++)
			{
				if (keysToAccessComponent[_id][i].componentType != nullptr)
				{
					POPEYE_CORE_INFO(keysToAccessComponent[_id][i].componentType);
				}
			}
		}

		char* GetName();
		void SetName(char* );

	};
}
