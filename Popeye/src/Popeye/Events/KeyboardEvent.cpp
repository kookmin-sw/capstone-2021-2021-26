#include "pch.h"
#include "KeyboardEvent.h"
namespace Popeye{

	void KeyboardEvent::setCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);

		auto key_press = [](GLFWwindow* w, int _key, int _scancode, int _action, int _mods) { static_cast<KeyboardEvent*>(glfwGetWindowUserPointer(w))->keyPress(_key, _scancode, _action, _mods);};
		
		glfwSetKeyCallback(window, key_press);
	}

	void KeyboardEvent::keyPress(int key, int scancode, int action, int mods)
	{
		POPEYE_CORE_INFO("key: {0}, scancode: {1}, action: {2}, mods: {3}", key, scancode, action, mods);
	}

}
