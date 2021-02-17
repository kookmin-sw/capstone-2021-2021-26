#pragma once

class ComponentManager
{
public:
	template<class component>
	void RegistComponent(component);
};