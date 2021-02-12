#pragma once
#include "../Events/EventManager.h"
#include "../Events/Events.h"

namespace Popeye {
	class EventSystem
	{
	private:
		EventMod eventstate;
		std::queue<Event*> eventqueue;
		GLFWwindow* window;
	private:
		void KeyPressCallback(int, int, int, int);	//key scancode action mods
		void MouseCursorCallback(double, double); //xpos ypos
		void MouseButtonCallback(int, int, int); //button action mods
		void MouseScrollCallback(double, double); //xoffset yoffset

		void ExecuteGUIEvent();
		void ExecuteSceneEvent();
		void ExecuteGameInput();
	public:
		void SetEventCallbacks(GLFWwindow*);
		void SystemRunning();
	};

}