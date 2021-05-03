#pragma once

namespace Popeye

typedef struct {
	double x;
	double y;
} Point;

typedef struct {
	Point leftTop;
	Point rightBot;
} UIFrame;

//-------------------------------------------
//TODO :: Text
//-------------------------------------------

typedef struct {
	Point leftTop;
	Point rightBot;
} Text;

//-------------------------------------------
//TODO :: Button
//-------------------------------------------


typedef struct {
	Point leftTop;
	Point rightBot;
} Button;
