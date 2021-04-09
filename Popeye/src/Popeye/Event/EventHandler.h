#pragma once
#include "Events.h"

namespace Popeye {
	
	class EventHandler
	{
	private:
		int				eventstate;
		KeyboardEvent	keyevent;
		MouseEvent		mouseevent;
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
		void HandleEvent();
	};

}