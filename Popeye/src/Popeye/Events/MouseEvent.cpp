#include "pch.h"
#include "MouseEvent.h"

namespace Popeye {
	void mouse_cursor_callback(struct GLFWwindow* window, double x, double y) 
	{
		std::cout << x << " ," << y << std::endl;
	}
	
	void mouse_button_callback(struct GLFWwindow* window, int button, int action, int mods) {
	
	}
}