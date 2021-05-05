#include "EventHandler.h"

namespace Popeye
{
	int g_eventMod;
	glm::vec2 g_scenePosition;
	glm::vec2 g_sceneSize;
	
	bool g_sendRay = false;
	glm::vec2 g_MousePos = glm::vec2(1.0f);

	extern glm::vec3 g_sceneViewPosition;
	extern glm::vec3 g_sceneViewDirection;

	EventHandler::EventHandler() {};
	EventHandler::~EventHandler() {};

	void EventHandler::SetEventCallbacks(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		auto key_press	  =	[](GLFWwindow* w, int _key, int _scancode, int _action, int _mods)	{static_cast<EventHandler*>(glfwGetWindowUserPointer(w))->KeyPressCallback(_key, _scancode, _action, _mods); };
		auto mouse_cursor = [](GLFWwindow* w, double _xPos, double _yPos)						{static_cast<EventHandler*>(glfwGetWindowUserPointer(w))->MouseCursorCallback(_xPos, _yPos); };
		auto mouse_button = [](GLFWwindow* w, int _button, int _action, int _mods)				{static_cast<EventHandler*>(glfwGetWindowUserPointer(w))->MouseButtonCallback(_button, _action, _mods); };
		auto mouse_scroll = [](GLFWwindow* w, double _xOffset, double _yOffset)					{static_cast<EventHandler*>(glfwGetWindowUserPointer(w))->MouseScrollCallback(_xOffset, _yOffset); };

		glfwSetKeyCallback(window, key_press);
		glfwSetCursorPosCallback(window, mouse_cursor);
		glfwSetMouseButtonCallback(window, mouse_button);
		glfwSetScrollCallback(window, mouse_scroll);
	}

	void EventHandler::HandleEvent()
	{
		if (eventstate != g_eventMod)
		{
			eventstate = g_eventMod;
		}

		//execute events
		switch (eventstate)
		{
		case 0:
			break;
		case 1:
			ExecuteGUIEvent();
			break;
		case 2:
			ExecuteSceneEvent();
			break;
		case 3:
			ExecuteGameInput();
			break;
		}

		glfwPollEvents();

	}

	void EventHandler::ExecuteGUIEvent() //For Dear imGUI events
	{
		static ImGuiIO& io = ImGui::GetIO();
	}

	void EventHandler::ExecuteSceneEvent() // On game scene edit
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
			if (!leftM)
			{
				leftM = true;
				glm::vec2 screepos = g_scenePosition;
				glm::vec2 screensize = g_sceneSize;
				g_MousePos.x = (float)mouseevent.xPos;
				g_MousePos.y = (float)mouseevent.yPos;

				if (!g_sendRay)
					g_sendRay = true;
			}
			if (leftM)
			{
			}

		}
		else
		{
			leftM = false;
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
				sceneViewDir[0] = glm::normalize(front);
			}
		}
		else
		{
			rightM = false;
		}

		
		//keyboard
		if (keyevent.IsKeyPressed(Keyboard::W)) { sceneViewPos[0] += 0.1f * sceneViewDir[0]; }
		if (keyevent.IsKeyPressed(Keyboard::S)) { sceneViewPos[0] -= 0.1f * sceneViewDir[0]; }
		if (keyevent.IsKeyPressed(Keyboard::D)) { sceneViewPos[0] += 0.1f * glm::normalize(glm::cross(sceneViewDir[0], glm::vec3(0.0f, 1.0f, 0.0f))); }
		if (keyevent.IsKeyPressed(Keyboard::A)) { sceneViewPos[0] -= 0.1f * glm::normalize(glm::cross(sceneViewDir[0], glm::vec3(0.0f, 1.0f, 0.0f))); }
		if (keyevent.IsKeyPressed(Keyboard::E)) { sceneViewPos[0] += glm::vec3(0.0f, 0.1f, 0.0f); }
		if (keyevent.IsKeyPressed(Keyboard::Q)) { sceneViewPos[0] -= glm::vec3(0.0f, 0.1f, 0.0f); }
		
	}

	void EventHandler::ExecuteGameInput() // input class handle on play
	{

	}

	void EventHandler::MouseCursorCallback(double _xPos, double _yPos)
	{
		mouseevent.xPos = _xPos;
		mouseevent.yPos = _yPos;
	}

	void EventHandler::MouseButtonCallback(int _button, int _action, int _mods)
	{
		if (_action == GLFW_PRESS)		{ mouseevent.mousePressed[_button] = true; }
		if (_action == GLFW_RELEASE)	{ mouseevent.mousePressed[_button] = false; }
	}

	void EventHandler::MouseScrollCallback(double _xoffset, double _yoffset)
	{
		mouseevent.xoffset = _xoffset;
		mouseevent.yoffset = _yoffset;
	}

	void EventHandler::KeyPressCallback(int key, int scancode, int action, int mods)
	{
		if (key != -1)
		{
			if (action == GLFW_PRESS)	{ keyevent.keyPressed[key] = true;	}
			if (action == GLFW_RELEASE) { keyevent.keyPressed[key] = false;	}
		}
	}
}
