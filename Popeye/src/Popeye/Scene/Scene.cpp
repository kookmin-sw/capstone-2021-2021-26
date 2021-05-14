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
		gameObjects[0]->SetName("Main Camera");
		gameObjects[1]->SetName("Light");
	}
	
	Scene::~Scene(){ for (int i = 0; i < gameObjects.size(); i++) { delete gameObjects[i];} }

	void Scene::CreateGameObject(std::string name)
	{
		int newID = 0;
		if (!reusableIDs.empty()){
			newID = reusableIDs.front();
			reusableIDs.pop();
		}
		else{
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

	void Scene::DeleteGameObject(int _id)
	{
		reusableIDs.push(_id);
	}

	void Scene::SetName(char* name)
	{
		sceneName = name;
	}

	std::string Scene::GetName(){return sceneName;}
	int Scene::GetNextID(){return gameObjectID;}
	std::queue<int> Scene::GetReusableQueue(){return reusableIDs;}
	std::vector<std::vector<Accessor>> Scene::GetAccessors(){return keysToAccessComponent;}

	void Scene::ResetAccessor(int id)
	{
		for (int i = 0; i < keysToAccessComponent[id].size(); i++)
		{
			keysToAccessComponent[id][i].Reset();
		}
	}
	
	std::vector<const char*> Scene::GetAllComponents(int _id)
	{
		std::vector<const char*> components;

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

	void Scene::SaveScene()
	{
		std::cout << "Scene name : " << sceneName << " \n";
		std::cout << "gameobjects : \n";
		for (int i = 0; i < gameObjects.size(); i++)
		{
			std::cout << gameObjects[i]->GetID() << ", " << gameObjects[i]->GetName() << " ";
		}
		std::cout << " \n";

		std::vector<int> reuseables;
		while (!reusableIDs.empty())
		{
			reuseables.push_back(reusableIDs.front());
			reusableIDs.pop();
		}

		std::cout << "accessor : \n";
		for (int i = 0; i < keysToAccessComponent.size(); i++)
		{
			for (int j = 0; j < keysToAccessComponent[i].size(); j++)
			{
				std::cout << keysToAccessComponent[i][j].componentType << ", ";
				std::cout << keysToAccessComponent[i][j].dataIndex << " ";
			}
			std::cout << ' \n';
		}
	}
}