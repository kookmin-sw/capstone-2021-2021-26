#pragma once
#include "../KeyCode.h"
#include "../MouseCode.h"

namespace Popeye {
	enum class Eventtype
	{
		MOUSECURSOR = 0,
		MOUSEBUTTON = 1,
		MOUSESCROLL = 2,
		KEYBOARD = 3,
		CHARINPUT = 4
	};

	struct Event
	{
		Eventtype eventtype;
		virtual std::vector<int> GetMouseButton()		{ return{ 0, 0, 0, 0 };}
		virtual std::vector<double> GetMouseCursor()	{ return{ 0.0, 0.0 }; }
		virtual std::vector<double> GetMouseScroll()	{ return{ 0.0, 0.0 }; }
		virtual std::vector<int> GetKeyButton()			{ return{ 0, 0, 0, 0 }; };
	};

	struct MouseCursor : public Event
	{
		double xPos, yPos;
		virtual std::vector<double> GetMouseCursor() { return { xPos, yPos }; }
	};
	
	struct MouseButton : public Event
	{
		MouseCode currentMouse;
		int button, action, mods;
		virtual std::vector<int> GetMouseButton() { return { currentMouse, button, action, mods }; }
	};

	struct MouseScroll : public Event
	{
		double xoffset, yoffset;
		virtual std::vector<double> GetMouseScroll() { return { xoffset, yoffset }; }
	};

	struct KeyboardButton : public Event
	{
		KeyCode pressedkey;
		virtual std::vector<int> GetKeyButton();
	};
}

