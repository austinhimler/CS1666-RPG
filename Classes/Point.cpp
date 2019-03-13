#include "../Headers/Point.h"

Point::Point()
{
}
Point::Point(int newX, int newY, int pX, int pY, int m, int d, int t, bool isClosed, bool isOpen)
{
	x = newX;
	y = newY;
	parentX = pX;
	parentY = pY;
	moves = m;
	dist = d;
	total = t;
	closed = isClosed;
	open = isOpen;
}