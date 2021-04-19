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
<<<<<<< HEAD
=======
		childs.push_back(child);
>>>>>>> d3fea4472f4c492ecb6f3e9bd38da674be1267f9
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
