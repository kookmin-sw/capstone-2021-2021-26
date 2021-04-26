#pragma once

namespace Popeye {
	class ComponentManager;
	class GameObject;

	struct Accessor 
	{
		const char* componentType;
		int dataIndex;
		
		Accessor() : componentType(nullptr), dataIndex(-1) {};
		void Reset() { componentType = nullptr; dataIndex = -1; }
	};
	
	class Scene //got gameobjects ID lists, if gameobject create, regist id. if gameobject deleted, 
	{
	private:
		int	sceneID;
		std::string sceneName;
		int gameObjectID;
		std::queue	<int> reusableIDs;
		std::vector <std::vector<Accessor>> keysToAccessComponent;
		std::vector <int> gameObjectIDs;
	
	public:
		int mainCameraID;
		std::vector <GameObject*> gameObjects;		// root gameobjects at scene

	private:
		void ResetAccessor(int id);
	
	public:
		Scene(std::string name = "Scene");
		~Scene();
		
		void CreateGameObject(std::string name = "GameObject");
		void DeleteGameObject(int id);

		template<class component>
		void AddData(int _id)
		{
			Accessor accessor;
			ComponentManager::GetInstance()->AddDataOfComponent<component>(accessor.componentType, accessor.dataIndex);
			keysToAccessComponent[_id].push_back(accessor);
		}

		void AddDataByName(int _id, const char* component);

		
		template<class component>
		component& GetData(int _id) // gameobject's data
		{
			const char* componentType = typeid(component).name();
			for (int i = 0; i < keysToAccessComponent[_id].size(); i++)
			{
				if (keysToAccessComponent[_id][i].componentType == componentType)
				{
					return ComponentManager::GetInstance()->GetDataOfComponent<component>
						(keysToAccessComponent[_id][i].componentType, keysToAccessComponent[_id][i].dataIndex);
				}
			}
			component& temp = component();
			return temp;
		}

		template<class component>
		bool CheckIfThereIsData(int _id)
		{
			const char* componentType = typeid(component).name();
			for (int i = 0; i < keysToAccessComponent[_id].size(); i++)
			{
				if (keysToAccessComponent[_id][i].componentType == componentType)
				{
					return true;
				}
			}
			return false;
		}


		std::vector<const char*> GetAllComponents(int _id);

		std::vector<Accessor> GetAllAddressOfID(int _id);

		std::string GetName();
		void SetName(char* );

	};
}
