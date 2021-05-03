#pragma once

namespace Popeye

class Point
{
	private:
		int xpos;
		int ypos;
    		int zpos;
		int wpos;

	public:
 		void PointSet(int x, int y);
		void UIFrame(int UI1, int UI2);
};

void Point::PointSet(int x, int y)
{
	xpos = x;
	ypos = y;
	Point UI1;
	UI2.PointSet = (x, y);
	zpos = x;
	wpos = y;
	Point UI2;
	UI2.PointSet = (x, y);
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

