#pragma once
#include "Popeye/Core.h"
#include "../MouseCode.h"

namespace Popeye
{
	class MouseEvent
	{
	private:
		MouseCode currentMouse;
		double xPos, yPos;
	private:
		void mouseCursorCallback(double, double);
		void mouseButtonCallback(int, int, int);
		void mouseScrollCallback(double, double);
	public:
		void setCallback(GLFWwindow*);
	};
}
