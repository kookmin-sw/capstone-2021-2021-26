#include "pch.h"
#include "MouseEvent.h"

namespace Popeye{

	void MouseEvent::setCallback(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);

		auto mouse_cursor = [](GLFWwindow* w, double _xPos, double _yPos)			{static_cast<MouseEvent*>(glfwGetWindowUserPointer(w))->mouseCursorCallback(_xPos, _yPos);};
		auto mouse_button = [](GLFWwindow* w, int _button, int _action, int _mods)	{static_cast<MouseEvent*>(glfwGetWindowUserPointer(w))->mouseButtonCallback(_button, _action, _mods);};
		auto mouse_scroll = [](GLFWwindow* w, double _xOffset, double _yOffset)		{static_cast<MouseEvent*>(glfwGetWindowUserPointer(w))->mouseScrollCallback(_xOffset, _yOffset);};

		glfwSetCursorPosCallback(window, mouse_cursor);
		glfwSetMouseButtonCallback(window, mouse_button);
		glfwSetScrollCallback(window, mouse_scroll);
	}

	void MouseEvent::mouseCursorCallback(double xPos, double yPos)
	{
		POPEYE_CORE_INFO("x: {0} , y: {1}", xPos, yPos);
	}

	void MouseEvent::mouseButtonCallback(int button, int action, int mods)
	{
		POPEYE_CORE_INFO("button: {0}, action: {1}, mods: {2}", button, action, mods);
	}

	void MouseEvent::mouseScrollCallback(double xoffset, double yoffset)
	{
		POPEYE_CORE_INFO("xoffset: {0}, yoffset: {1}", xoffset, yoffset);
	}

}
