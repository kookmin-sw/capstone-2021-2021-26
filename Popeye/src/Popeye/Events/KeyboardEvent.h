#pragma once
#include "../KeyCode.h"
namespace Popeye {
	class KeyboardEvent
	{
	private:
		KeyCode pressedkey;
	private:
		void keyPress(int, int, int, int);
	public:
		void setCallback(GLFWwindow*);
	};
}

