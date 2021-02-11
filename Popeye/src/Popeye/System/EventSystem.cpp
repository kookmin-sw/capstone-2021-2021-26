#include "pch.h"
#include "EventSystem.h"
namespace Popeye
{
	void EventSystem::SetEventCallbacks(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		auto key_press =	[](GLFWwindow* w, int _key, int _scancode, int _action, int _mods)	{static_cast<EventSystem*>(glfwGetWindowUserPointer(w))->KeyPressCallback(_key, _scancode, _action, _mods); };
		//auto mouse_cursor = [](GLFWwindow* w, double _xPos, double _yPos)						{static_cast<EventSystem*>(glfwGetWindowUserPointer(w))->MouseCursorCallback(_xPos, _yPos); };
		auto mouse_button = [](GLFWwindow* w, int _button, int _action, int _mods)				{static_cast<EventSystem*>(glfwGetWindowUserPointer(w))->MouseButtonCallback(_button, _action, _mods); };
		auto mouse_scroll = [](GLFWwindow* w, double _xOffset, double _yOffset)					{static_cast<EventSystem*>(glfwGetWindowUserPointer(w))->MouseScrollCallback(_xOffset, _yOffset); };

		glfwSetKeyCallback(window, key_press);
		//glfwSetCursorPosCallback(window, mouse_cursor);
		glfwSetMouseButtonCallback(window, mouse_button);
		glfwSetScrollCallback(window, mouse_scroll);
	}

	void EventSystem::SystemRunning()
	{
		if (eventstate != EventManager::GetInstance()->GetState())
		{
			eventstate = EventManager::GetInstance()->GetState();
		}

		glfwPollEvents();
		//execute events
		switch (eventstate)
		{
		case EventMod::NONE:
			break;
		case EventMod::ONGUI:
			ExecuteGUIEvent();
			break;
		case EventMod::SCENE:
			ExecuteSceneEvent();
			break;
		case EventMod::GAME:
			ExecuteGameInput();
			break;
		}

	}

	void EventSystem::ExecuteGUIEvent()
	{
		static Event* _event;
		static ImGuiIO& io = ImGui::GetIO();
		while (!eventqueue.empty())
		{
			_event = eventqueue.front();
			if (_event->eventtype == Eventtype::MOUSEBUTTON)
			{
			}
			if (_event->eventtype == Eventtype::MOUSESCROLL)
			{
				io.MouseWheelH	+= (float)_event->GetMouseScroll()[0];
				io.MouseWheel	+= (float)_event->GetMouseScroll()[1];
			}
			if (_event->eventtype == Eventtype::KEYBOARD)
			{

			}
			if (_event->eventtype == Eventtype::CHARINPUT)
			{

			}
			eventqueue.pop();

			if (eventqueue.size() == 0)
			{
				POPEYE_CORE_INFO("done");
			}
		}
	}

	void EventSystem::ExecuteSceneEvent()
	{
		while (!eventqueue.empty())
		{
			eventqueue.pop();
		}
	}

	void EventSystem::ExecuteGameInput()
	{
		while (!eventqueue.empty())
		{
			eventqueue.pop();
		}
	}

	void EventSystem::KeyPressCallback(int key, int scancode, int action, int mods)
	{
		KeyboardButton _event;
		_event.eventtype = Eventtype::KEYBOARD;
	}

	void EventSystem::MouseCursorCallback(double& _xPos, double& _yPos)
	{
		MouseCursor _event;
		_event.eventtype = Eventtype::MOUSECURSOR;
		_event.xPos = _xPos;
		_event.yPos = _yPos;
		eventqueue.push(&_event);
	}

	void EventSystem::MouseButtonCallback(int& button, int& action, int& mods)
	{
		MouseButton _event;
		_event.eventtype = Eventtype::MOUSEBUTTON;
		_event.button = button;
		_event.action = action;
		_event.mods = mods;
		eventqueue.push(&_event);
	}

	void EventSystem::MouseScrollCallback(double& xoffset, double& yoffset)
	{
		POPEYE_CORE_INFO("x: {0}, y: {1} ", xoffset, yoffset);
		MouseScroll _event;
		_event.eventtype = Eventtype::MOUSESCROLL;
		_event.xoffset = xoffset;
		_event.yoffset = yoffset;
		eventqueue.push(&_event);
	}
}
