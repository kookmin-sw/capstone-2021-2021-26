#include "pch.h"
#include "GameObject.h"
#include "Scene.h"

namespace Popeye {
	
	GameObject::GameObject() 
	{
		this->ID = (int)&*this;
		std::cout << ID << std::endl;
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
