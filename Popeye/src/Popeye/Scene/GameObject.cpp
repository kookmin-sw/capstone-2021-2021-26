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
	}

	std::string GameObject::GetName()
	{
		return name;
	}
	void GameObject::SetName(std::string _name)
	{
		this->name = _name;
	}

	int GameObject::GetID()
	{
		return this->id;
	}
}
