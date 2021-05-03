#pragma once

namespace Popeye

class Point
{
	private:
		int x1, y1;
    		int x2, y2;

	public:
 		void PointSet(int x, int y);
};

void Point::PointSet(int x, int y)
{
	x1 = x;
	y1 = y;
	Point UI1;
	UI1.UIFrame = (x, y);
	x2 = x;
	y2 = y;
	Point UI2;
	UI2.UIFrame = (x, y);
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

