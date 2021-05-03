#include "UIComponents.h"
#include <iostream>

using namespace std;
{
  private:
    int xpos;
    int ypos;

public:
  void PointSet(int x, int y);
  void ShowPosition();
};

void Point::PointSet(int x, int y)
{
  xpos=x;
  ypos=y;
}
