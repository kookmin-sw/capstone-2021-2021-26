#include "GameObject.h"

namespace Popeye {
	
	GameObject::GameObject(int _id)
	{
		id = _id;
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::AddChild(GameObject* child)
	{
		childs.push_back(child);
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
