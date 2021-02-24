#include "pch.h"
#include "GameObject.h"

namespace Popeye {
	
	GameObject::GameObject()
	{
		ID = (int)&*this;
	}

	void GameObject::AddChild(GameObject* child)
	{
		childs.push_back(child);
	}

	GameObject::~GameObject() 
	{
	}

	char* GameObject::GetName()
	{
		return this->name;
	}
	void GameObject::SetName(char* _name)
	{
		this->name = _name;
	}

	int GameObject::GetID()
	{
		return this->ID;
	}
}
