#include "GameObject.h"
#include "../Manager/SceneManager.h"
#include "Scene.h"

namespace Popeye {
	
	GameObject::GameObject(int _id)
	{
		SetValue(_id);
	}

	GameObject::GameObject(int _id, std::string _name, Transform _transform)
	{
		SetValue(_id, _name, _transform);
	}

	GameObject::~GameObject(){}

	/*void GameObject::AddChild(GameObject* child)
	{
	}*/

	void GameObject::SetValue(int _id)
	{
		id = _id;
		transform = Transform();
	}

	void GameObject::SetValue(int _id, std::string _name, Transform _transform)
	{
		id			= _id;
		name		= _name;
		transform	= _transform;
	}

	std::string GameObject::GetName()
	{
		return name;
	}
	void GameObject::SetName(std::string _name)
	{
		std::swap(name, _name);
	}

	int GameObject::GetID()
	{
		return this->id;
	}

	void GameObject::AddComponentByName(const char* component)
	{
		SceneManager::GetInstance()->currentScene->AddDataByName(id, component);
	}
}
