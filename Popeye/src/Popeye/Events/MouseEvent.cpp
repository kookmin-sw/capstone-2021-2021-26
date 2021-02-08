#include "pch.h"
#include "MouseEvent.h"

namespace Popeye {
	void MouseEvent::mouse_cursor_callback(struct GLFWwindow* window, double& x, double& y) 
	{
		this->xPos = x;
		this->yPos = y;
	}
	
	void MouseEvent::mouse_button_callback(struct GLFWwindow* window, int button, int action, int mods) 
	{
		
	}
}