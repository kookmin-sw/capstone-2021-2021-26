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
		void UIFrame(int x, int y);
		void Text(int x, int y);
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

void Point::UIFrame(int x, int y)
{
	
}

//-------------------------------------------
//TODO :: Text
//-------------------------------------------

void Point::Text(int x, int y)
{
	
}

//-------------------------------------------
//TODO :: Button
//-------------------------------------------

int main()
{
	return 0;
}

