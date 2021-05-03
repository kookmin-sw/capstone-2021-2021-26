#pragma once

namespace Popeye

struct Point{
	double x;
	double y;
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
