#ifndef _____POINT_H_____
#define _____POINT_H_____

class Point
{
public:
	Point(int, int, int, int, int, int, int, bool, bool);
	Point(int, int, int, int);
	Point();
	int y;
	int x;
	int parentX;
	int parentY;
	bool closed;
	bool open;
	int moves;
	int dist;
	int total;
};
#endif