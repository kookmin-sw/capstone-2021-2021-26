#include "pch.h"
#include "GameObject.h"

namespace Popeye {
	
	GameObject::GameObject() 
	{}

	GameObject::~GameObject() 
	{}

	char* GameObject::getName()
	{
		return this->name;
	}
}
