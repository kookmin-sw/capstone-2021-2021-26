#include "GameObject.h"

namespace Popeye {
	
	GameObject::GameObject(int _id)
	{
		id = _id;
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
		return name;
	}
	void GameObject::SetName(char* _name)
	{
		this->name = _name;
	}

	int GameObject::GetID()
	{
		return this->id;
	}
}
