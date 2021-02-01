#include "pch.h"
#include "GameObject.h"
#include "Scene.h"

namespace Popeye {
	
	GameObject::GameObject() 
	{
		this->ID = (int)&*this;
		SceneManager::GetInstance()->currentScene->GameObjectCreated(this->ID);
	}
	GameObject::~GameObject() 
	{

	}

	char* GameObject::GetName()
	{
		return this->name;
	}

	int GameObject::GetID()
	{
		return this->ID;
	}
}
