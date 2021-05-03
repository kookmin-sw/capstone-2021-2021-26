#pragma once

namespace Popeye

class Point
{
	private:
		float xpos;
		float ypos;

	public:
 		void TopLeft(float x, float y);
		void BottomRight(float x, float y);
		void UIFrame(float x, float y);
		void Text(float x, float y);
};

void Point::TopLeft(float x, float y)
{
	xpos = x;
	ypos = y;
}

void Point::BottomRight(float x, float y)
{
	xpos = x;
	ypos = y;
}

void Point::UIFrame(float x, float y)
{
	
}

//-------------------------------------------
//TODO :: Text
//-------------------------------------------

void Point::Text(float x, float y)
{
	
}

//-------------------------------------------
//TODO :: Button
//-------------------------------------------

int main()
{
	return 0;
}
