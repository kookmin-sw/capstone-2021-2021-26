#pragma once
#include "KeyCode.h"
#include "MouseCode.h"

namespace Popeye {

	struct KeyboardEvent
	{
		bool keyPressed[350] = {false};
		bool IsKeyPressed(KeyCode keycode) { return keyPressed[keycode]; }
	};

	struct MouseEvent 
	{
		bool mousePressed[11] = {false};
		double xPos, yPos;
		double xoffset, yoffset;
		bool IsMousePressed(MouseCode mousecode) { return mousePressed[mousecode]; }
	};
}

