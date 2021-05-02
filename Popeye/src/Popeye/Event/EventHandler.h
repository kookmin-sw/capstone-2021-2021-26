#pragma once
#include "Events.h"

namespace Popeye {
	
	class EventHandler
	{
	private:
		int				eventstate;
		KeyboardEvent	keyevent;
		MouseEvent		mouseevent;
	public:
		//screenCam pos, dir
		glm::vec3 sceneViewPos;
		glm::vec3 sceneViewDir;
		//ray
		bool sendEditRay;
		glm::vec3 screenToMousePos;
		glm::vec3 screenToMouseDir;
	private:
		void KeyPressCallback(int, int, int, int);	//key scancode action mods
		void MouseCursorCallback(double, double); //xpos ypos
		void MouseButtonCallback(int, int, int); //button action mods
		void MouseScrollCallback(double, double); //xoffset yoffset

		void ExecuteGUIEvent();
		void ExecuteSceneEvent();
		void ExecuteGameInput();
	public:
		EventHandler();
		~EventHandler();
		void SetEventCallbacks(GLFWwindow*);
		void HandleEvent();
	};

}