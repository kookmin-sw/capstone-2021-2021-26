#include "Scene.h"
#include "../Manager/ComponentManager.h"
#include "GameObject.h"


namespace Popeye {
	//Scene
	Scene::Scene(std::string name) 
	{
		sceneName		= name;
		gameObjectID	= 0;
		focusedCamID	= 0;

		CreateGameObject();
		AddDataByName(0, "Camera");

		CreateGameObject();
		AddDataByName(1, "Light");

		gameObjects[0]->transform.position = { 8.0f,8.0f, 8.0f };
		gameObjects[0]->transform.rotation = { -30.0f, 45.0f, 0.0f };
		gameObjects[0]->SetName("Main_Camera");
		gameObjects[1]->SetName("Light");
	}
	
	Scene::~Scene(){ for (int i = 0; i < gameObjects.size(); i++) { delete gameObjects[i];} }

	void Scene::CreateGameObject(std::string name)
	{
		int newID = 0;
		if (!idRecycleQ.empty()){
			
			newID = idRecycleQ.front();
			idRecycleQ.pop();
		}
		else
		{	
			newID = gameObjectID;
			gameObjectID++;
		}

		GameObject* gameobject = new GameObject(newID);
		
		std::string idstr = std::to_string(newID);
		name += idstr;
		gameobject->SetName(name);
		gameObjects.push_back(gameobject);

		if (newID + 1 <= keysToAccessComponent.size())
		{
			ResetAccessor(newID);
		}
		else
		{
			std::vector<Accessor> newAccessor;
			keysToAccessComponent.push_back(newAccessor);
		}

	}

	void Scene::CreateGameObject(int id, std::string name, Transform transform)
	{
		GameObject* gameobject = new GameObject(id, name, transform);
		gameObjects.push_back(gameobject);

		if (id + 1 <= keysToAccessComponent.size())
		{
			ResetAccessor(id);
		}
		else
		{
			std::vector<Accessor> newAccessor;
			keysToAccessComponent.push_back(newAccessor);
		}
	}

	void Scene::DeleteGameObject(int _id)
	{
		int id = _id;
		idRecycleQ.push(id);
		ResetAccessor(id);
		int gameobject_size = gameObjects.size();
		for (int i = 0; i < gameobject_size; i++)
		{
			if (id == gameObjects[i]->GetID())
			{
				GameObject* targetObject = gameObjects[i];
				gameObjects[i] = nullptr;
				delete(targetObject);
				gameObjects.erase(gameObjects.begin() + i);
				return;
			}
		}
	}

	void Scene::ResetAccessor(int id)
	{
		for (int i = 0; i < keysToAccessComponent[id].size(); i++)
		{
			keysToAccessComponent[id][i].Reset();
		}
	}
	
	std::vector<std::string> Scene::GetAllComponents(int _id)
	{
		std::vector<std::string> components;

		for (int i = 0; i < keysToAccessComponent[_id].size(); i++)
		{
			components.push_back(keysToAccessComponent[_id][i].componentType);
		}
		return components;
	}

	std::vector<Accessor> Scene::GetAllAddressOfID(int _id)
	{
		return keysToAccessComponent[_id];
	}

	void Scene::AddDataByName(int _id, const char* component)
	{
		Accessor accessor;
		ComponentManager::GetInstance()->AddDataOfComponentByName(component, accessor.componentType, accessor.dataIndex);
		keysToAccessComponent[_id].push_back(accessor);
	}


	int Scene::GetNextID() { return gameObjectID; }
	std::string Scene::GetName() { return sceneName; }
	std::queue<int> Scene::GetRecycleQueue() { return idRecycleQ; }
	std::vector<std::vector<Accessor>> Scene::GetAccessors()
	{
		std::vector<std::vector<Accessor>> keysTodata;

		int size = keysToAccessComponent.size();
		for (int i = 0; i < size; i++)
		{
			std::vector < Accessor> datas;

			int arr_size = keysToAccessComponent[i].size();
			for (int j = 0; j < arr_size; j++)
			{
				if (keysToAccessComponent[i][j].componentType != "")
				{
					datas.push_back(keysToAccessComponent[i][j]);
				}
			}

			keysTodata.push_back(datas);
		}

		return keysTodata;
	}

	void Scene::SetName(std::string _name)
	{
		sceneName = _name;
	}
	
	void Scene::SetNextID(int _nextID)
	{
		gameObjectID = _nextID;
	}
	
	void Scene::SetRecycleQueue(std::queue<int>& _idRecycleQ)
	{
		while (!idRecycleQ.empty())
		{
			idRecycleQ.pop();
		}
		
		while (!_idRecycleQ.empty())
		{
			idRecycleQ.push(_idRecycleQ.front());
			_idRecycleQ.pop();
		}
	}

	void Scene::SetAccessors(const std::vector<std::vector<Accessor>>& accessors)
	{
		//keysToAccessComponent = accessors;
		int size = keysToAccessComponent.size();
		for (int i = 0; i < size; i++)
		{
			std::vector<Accessor>().swap(keysToAccessComponent[i]);
		}
		std::vector<std::vector<Accessor>>().swap(keysToAccessComponent);

		size = accessors.size();
		for (int i = 0; i < size; i++)
		{
			int sszize = accessors[i].size();
			std::vector<Accessor> acc;
			for (int j = 0; j < sszize; j++)
			{
				acc.push_back(accessors[i][j]);
			}
			keysToAccessComponent.push_back(acc);
		}

	}

	void Scene::SetGameObjects(std::vector <GameObject>& _gameObjects)
	{
		int curr_size	= gameObjects.size();
		int load_size	= _gameObjects.size();
		int delstart	= 0;
		
		if (curr_size > load_size)
		{
			for (int i = load_size; i < curr_size; i++)
			{
				GameObject* targetObject = gameObjects[i];
				gameObjects[i] = nullptr;
				delete(targetObject);
			}
			gameObjects.erase(gameObjects.begin() + load_size, gameObjects.end());
			gameObjects.shrink_to_fit();
		}

		for (int i = 0; i < load_size; i++)
		{
			if (i < curr_size)
			{
				gameObjects[i]->SetValue(_gameObjects[i].GetID(), _gameObjects[i].GetName(), _gameObjects[i].transform);
			}
			else
			{
				// crash when there is gameObject with same ID if size > ssize
				GameObject* gameobject = new GameObject(_gameObjects[i].GetID(), _gameObjects[i].GetName(), _gameObjects[i].transform);
				gameObjects.push_back(gameobject);
			}
		}

	}

}