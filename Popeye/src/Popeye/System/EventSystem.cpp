#include "pch.h"
#include "EventSystem.h"

namespace Popeye
{
	extern glm::vec3 g_sceneViewPosition;

	void EventSystem::SetEventCallbacks(GLFWwindow* window)
	{
		this->window = window;

		glfwSetWindowUserPointer(window, this);
		auto key_press =	[](GLFWwindow* w, int _key, int _scancode, int _action, int _mods)	{static_cast<EventSystem*>(glfwGetWindowUserPointer(w))->KeyPressCallback(_key, _scancode, _action, _mods); };
		auto mouse_cursor = [](GLFWwindow* w, double _xPos, double _yPos)						{static_cast<EventSystem*>(glfwGetWindowUserPointer(w))->MouseCursorCallback(_xPos, _yPos); };
		auto mouse_button = [](GLFWwindow* w, int _button, int _action, int _mods)				{static_cast<EventSystem*>(glfwGetWindowUserPointer(w))->MouseButtonCallback(_button, _action, _mods); };
		auto mouse_scroll = [](GLFWwindow* w, double _xOffset, double _yOffset)					{static_cast<EventSystem*>(glfwGetWindowUserPointer(w))->MouseScrollCallback(_xOffset, _yOffset); };

		glfwSetKeyCallback(window, key_press);
		glfwSetCursorPosCallback(window, mouse_cursor);
		glfwSetMouseButtonCallback(window, mouse_button);
		glfwSetScrollCallback(window, mouse_scroll);
	}

	void EventSystem::SystemRunning()
	{
		if (eventstate != EventManager::GetInstance()->GetState())
		{
			eventstate = EventManager::GetInstance()->GetState();
		}

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

		glfwPollEvents();


	}

	void EventSystem::ExecuteGUIEvent() //For Dear imGUI events
	{
		static Event* _event;
		static ImGuiIO& io = ImGui::GetIO();
		while (!eventqueue.empty())
		{
			_event = eventqueue.front();
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
		}
	}

	void EventSystem::ExecuteSceneEvent() // on game scene edit
	{
		static Event* _event;
		static bool isMouseRightButtonPressed = false;
		static std::stack<int> keystack;

		int button	= -1;
		int action	= -1;
		while (!eventqueue.empty())
		{
			_event = eventqueue.front();
			if (_event->eventtype == Eventtype::MOUSECURSOR)
			{
				if (isMouseRightButtonPressed)
				{
					POPEYE_CORE_INFO("scene view moving");
				}
			}
			if (_event->eventtype == Eventtype::MOUSEBUTTON)
			{
				button = _event->GetMouseButton()[0];
				action = _event->GetMouseButton()[1];
				if (action == GLFW_PRESS && button == Mouse::ButtonRight)	{ isMouseRightButtonPressed = true; }
				if (action == GLFW_RELEASE && button == Mouse::ButtonRight)	{ isMouseRightButtonPressed = false; }
			}
			if (_event->eventtype == Eventtype::MOUSESCROLL)
			{
				//std::cout << (float)_event->GetMouseScroll()[0] << std::endl;
				//std::cout << _event->GetMouseScroll()[1] << std::endl;
			}
			if (_event->eventtype == Eventtype::KEYBOARD)
			{
				button	=	_event->GetKeyButton()[0];
				action	=	_event->GetKeyButton()[2];

				if (action == GLFW_PRESS || action == GLFW_REPEAT)
				{
					if (button == Keyboard::W) 
					{
						g_sceneViewPosition += glm::vec3(0.0f, 0.0f, 0.2f);
					}
					if (button == Keyboard::A) 
					{
						g_sceneViewPosition += glm::vec3(0.2f, 0.0f, 0.0f);
					}
					if (button == Keyboard::S) 
					{
						g_sceneViewPosition -= glm::vec3(0.0f, 0.0f, 0.2f);
					}
					if (button == Keyboard::D) 
					{
						g_sceneViewPosition -= glm::vec3(0.2f, 0.0f, 0.0f);
					}
				}
			}

			eventqueue.pop();
		}
	}

	void EventSystem::ExecuteGameInput() // input class handle on play
	{
		while (!eventqueue.empty())
		{
			eventqueue.pop();
		}
	}

	void EventSystem::MouseCursorCallback(double _xPos, double _yPos)
	{
		MouseCursor _event;
		_event.eventtype	= Eventtype::MOUSECURSOR;
		_event.xPos			= _xPos;
		_event.yPos			= _yPos;

		eventqueue.push(&_event);
	}

	void EventSystem::MouseButtonCallback(int button, int action, int mods)
	{
		MouseButton _event;
		_event.eventtype	= Eventtype::MOUSEBUTTON;
		_event.button		= button;
		_event.action		= action;
		_event.mods			= mods;
		//POPEYE_CORE_INFO("button : {0}, action : {1}, mods : {2}", button, action, mods);
		eventqueue.push(&_event);
	}

	void EventSystem::MouseScrollCallback(double xoffset, double yoffset)
	{
		MouseScroll _event;
		_event.eventtype	= Eventtype::MOUSESCROLL;
		_event.xoffset		= xoffset;
		_event.yoffset		= yoffset;
		
		eventqueue.push(&_event);
	}

	void EventSystem::KeyPressCallback(int key, int scancode, int action, int mods)
	{
		KeyboardButton _event;
		_event.eventtype	= Eventtype::KEYBOARD;
		_event.button		= key;
		_event.scancode		= scancode;
		_event.action		= action;
		_event.mods			= mods;
		POPEYE_CORE_INFO("button : {0}, action : {1}, mods : {2}", key, action, mods);
		eventqueue.push(&_event);
	}
}
