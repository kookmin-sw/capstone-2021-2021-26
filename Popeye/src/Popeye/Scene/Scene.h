#pragma once

namespace Popeye {
	class GUIManager;
	class ComponentManager;
	class GameObject;
	struct Transform;

	struct Accessor 
	{
		std::string componentType;
		int dataIndex;
		
		Accessor() : componentType(""), dataIndex(-1) {};
		void Reset() { componentType = ""; dataIndex = -1; }
	};
	
	class Scene //got gameobjects ID lists, if gameobject create, regist id. if gameobject deleted, 
	{
	private:
		int	sceneID;
		std::string sceneName;
		int gameObjectID;
		std::queue	<int> idRecycleQ;
		std::vector <std::vector<Accessor>> keysToAccessComponent;
	
	public:
		int focusedCamID;
		std::vector <GameObject*> gameObjects;		// root gameobjects at scene

	private:
		void ResetAccessor(int id);
	
	public:
		Scene(std::string name = "Scene");
		~Scene();
		
		void CreateGameObject(std::string name = "GameObject");
		void CreateGameObject(int id, std::string name, Transform transform);
		void DeleteGameObject(int _id);

		template<class component>
		void AddData(int _id)
		{
			Accessor accessor;
			ComponentManager::GetInstance()->AddDataOfComponent<component>(accessor.componentType, accessor.dataIndex);
			keysToAccessComponent[_id].push_back(accessor);
		}

		template<class component>
		void RemoveData(int _id)
		{
			std::string componentType = ComponentManager::GetInstance()->ComponentTypeName<component>();
			int address_size = keysToAccessComponent[_id].size();
			for (int i = 0; i < address_size; i++)
			{
				if (keysToAccessComponent[_id][i].componentType != "" && keysToAccessComponent[_id][i].componentType[0] == componentType[0])
				{
					ComponentManager::GetInstance()->RemoveDataOfComponent<component>(componentType, keysToAccessComponent[_id][i].dataIndex);
					keysToAccessComponent[_id][i].Reset();
					return;
				}
			}
		}
		
		template<class component>
		component& GetData(int _id) // gameobject's data
		{
			std::string componentType = ComponentManager::GetInstance()->ComponentTypeName<component>();
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
			std::string componentType = ComponentManager::GetInstance()->ComponentTypeName<component>();

			for (int i = 0; i < keysToAccessComponent[_id].size(); i++)
			{
				if (keysToAccessComponent[_id][i].componentType == componentType)
				{
					return true;
				}
			}
			return false;
		}

		void AddDataByName(int _id, const char* component);

		std::vector<std::string> GetAllComponents(int _id);

		std::vector<Accessor> GetAllAddressOfID(int _id);

		std::string GetName();
		int GetNextID();
		std::queue<int> GetRecycleQueue();
		std::vector<std::vector<Accessor>> GetAccessors();

		void SetName(std::string _name);
		void SetNextID(int _nextID);
		void SetRecycleQueue(std::queue<int>& _idRecycleQ);
		void SetAccessors(const std::vector<std::vector<Accessor>>& _accessors);
		void SetGameObjects(std::vector <GameObject>& _gameObjects);
	};
}
