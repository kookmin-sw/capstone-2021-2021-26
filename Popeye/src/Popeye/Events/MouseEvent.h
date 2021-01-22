#pragma once
#include "Popeye/Core.h"

namespace Popeye
{
	class POPEYE_API MouseEvent
	{
	public:
		void mouse_sensor();
		void mouse_cursor_callback(struct GLFWwindow* window, double x, double y);
		void mouse_button_callback(struct GLFWwindow* window, int button, int action, int mods);
	private:
		double xPos, yPos;
	};
}
