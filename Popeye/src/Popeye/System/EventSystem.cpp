#include "pch.h"
#include "EventSystem.h"

namespace Popeye
{
	extern glm::vec3 g_sceneViewPosition;
	extern glm::vec3 g_sceneViewDirection;

	void EventSystem::SetEventCallbacks(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		auto key_press	  =	[](GLFWwindow* w, int _key, int _scancode, int _action, int _mods)	{static_cast<EventSystem*>(glfwGetWindowUserPointer(w))->KeyPressCallback(_key, _scancode, _action, _mods); };
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
		static ImGuiIO& io = ImGui::GetIO();
	}

	void EventSystem::ExecuteSceneEvent() // On game scene edit
	{
		static bool leftM	= false;
		static bool middleM = false;
		static bool rightM	= false;
		static glm::vec2 lastPos = glm::vec2(0.0f, 0.0f);
		static float yaw = -90.0f;
		static float pitch = 0.0f;

		//mouse
		if (mouseevent.IsMousePressed(Mouse::ButtonLeft))//drag and drop
		{

		}
		if (mouseevent.IsMousePressed(Mouse::ButtonMiddle))
		{

		}
		if (mouseevent.IsMousePressed(Mouse::ButtonRight))	
		{
			if (!rightM)
			{
				rightM = true;
				lastPos.x = (float)mouseevent.xPos;
				lastPos.y = (float)mouseevent.yPos;
			}
			if (rightM)
			{
				float xoffset = lastPos.x - mouseevent.xPos;
				float yoffset = lastPos.y - mouseevent.yPos;
				lastPos.x = mouseevent.xPos;
				lastPos.y = mouseevent.yPos;

				float sensitivity = 0.25f;
				xoffset *= sensitivity;
				yoffset *= sensitivity;

				yaw -= xoffset;
				pitch += yoffset;

				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;
				
				glm::vec3 front;
				front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
				front.y = sin(glm::radians(pitch));
				front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
				g_sceneViewDirection = glm::normalize(front);
			}
		}
		else
		{
			rightM = false;
		}

		
		//keyboard
		if (keyevent.IsKeyPressed(Keyboard::W)) { g_sceneViewPosition += 0.1f * g_sceneViewDirection; }
		if (keyevent.IsKeyPressed(Keyboard::S)) { g_sceneViewPosition -= 0.1f * g_sceneViewDirection; }
		if (keyevent.IsKeyPressed(Keyboard::A)) { g_sceneViewPosition -= 0.1f * glm::normalize(glm::cross(g_sceneViewDirection, glm::vec3(0.0f, 1.0f, 0.0f))); }
		if (keyevent.IsKeyPressed(Keyboard::D)) { g_sceneViewPosition += 0.1f * glm::normalize(glm::cross(g_sceneViewDirection, glm::vec3(0.0f, 1.0f, 0.0f))); }
		if (keyevent.IsKeyPressed(Keyboard::Q)) { g_sceneViewPosition -= glm::vec3(0.0f, 0.1f, 0.0f); }
		if (keyevent.IsKeyPressed(Keyboard::E)) { g_sceneViewPosition += glm::vec3(0.0f, 0.1f, 0.0f); }
		
	}

	void EventSystem::ExecuteGameInput() // input class handle on play
	{

	}

	void EventSystem::MouseCursorCallback(double _xPos, double _yPos)
	{
		mouseevent.xPos = _xPos;
		mouseevent.yPos = _yPos;
	}

	void EventSystem::MouseButtonCallback(int _button, int _action, int _mods)
	{
		if (_action == GLFW_PRESS)		{ mouseevent.mousePressed[_button] = true; }
		if (_action == GLFW_RELEASE)	{ mouseevent.mousePressed[_button] = false; }
	}

	void EventSystem::MouseScrollCallback(double _xoffset, double _yoffset)
	{
		mouseevent.xoffset = _xoffset;
		mouseevent.yoffset = _yoffset;
	}

	void EventSystem::KeyPressCallback(int key, int scancode, int action, int mods)
	{
		if (key != -1)
		{
			if (action == GLFW_PRESS)	{ keyevent.keyPressed[key] = true;	}
			if (action == GLFW_RELEASE) { keyevent.keyPressed[key] = false;	}
		}
	}
}
