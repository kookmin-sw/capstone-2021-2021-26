#pragma once
#include "Popeye/Core.h"

namespace Popeye
{
	enum MouseButton 
	{
		RIGHTBUTTON,
		LEFTBUTTON,
		WHEEL
	};
	class MouseEvent
	{
	private:
		GLFWwindow* window;
		double xPos, yPos;
	private:
		void mouse_cursor_callback(GLFWwindow*, double&, double&);
		void mouse_button_callback(GLFWwindow*, int, int, int); //button, action, mods
	public:
		void mouse_set_Callback(GLFWwindow*);
	};
}
