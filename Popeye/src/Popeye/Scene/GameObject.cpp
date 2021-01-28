#include "pch.h"
#include "GameObject.h"
#include "Scene.h"

namespace Popeye {
	
	GameObject::GameObject(int id) 
	{
		this->ID = id;
	}
	GameObject::~GameObject() 
	{

	}

	char* GameObject::getName()
	{
		return this->name;
	}
}
