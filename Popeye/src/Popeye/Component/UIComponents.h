#pragma once

namespace Popeye

class Point
{
	private:
		int xpos;
		int ypos;

	public:
 		void TopLeft(int x, int y);
		void BottomRight(int x, int y);
		void UIFrame(int UI1, int UI2);
};

void Point::TopLeft(int x, int y)
{
	xpos = x;
	ypos = y;
}

void Point::BottomRight(int x, int y)
{
	xpos = x;
	ypos = y;
}

void Point::UIFrame(int UI1, int UI2)
{
	
}

//-------------------------------------------
//TODO :: Text
//-------------------------------------------

void Point::Text(int UI1, int UI2)
{
	
}

//-------------------------------------------
//TODO :: Button
//-------------------------------------------

int main()
{
	return 0;
}

