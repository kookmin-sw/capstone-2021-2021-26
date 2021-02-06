#include "pch.h"
#include "EventSystem.h"
#include "../Events/MouseEvent.h"

namespace Popeye
{
	void EventSystem::SetInputCallbacks(GLFWwindow* window)
	{
		static MouseEvent* mouse = new MouseEvent();

		//glfwSetCursorPosCallback(window, mouse->mouse_cursor_callback);
	}

	void EventSystem::SystemRunning()
	{

	}

	void EventSystem::Init()
	{

	}

	void EventSystem::OnUpdate()
	{

	}

	void EventSystem::OnExit()
	{

	}
}
