#pragma once

namespace Popeye

struct Point{
	float x;
	float y;
};

struct UIFrame{
	Point leftTop;
	Point rightBot;
};

struct Text{
	Point leftTop;
	Point rightBot;
};

struct Button{
	Point leftTop;
	Point rightBot;
};
