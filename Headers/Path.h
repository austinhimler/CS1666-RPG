#ifndef _____PATH_H_____
#define _____PATH_H_____
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include "Point.h"
#include "Tile.h"
#include <random>

class Path
{
public:
	Path(Tile*[MAX_HORIZONTAL_TILES][MAX_VERTICAL_TILES]);
	std::vector<Point*> makePath(int startX, int startY, int endX, int endY);
	bool isValid(int x, int y);
	bool isDestination(int x, int y, int endX, int endY);
	int calculateDist(int x, int y, int endX, int endY);
	int map[MAX_HORIZONTAL_TILES][MAX_VERTICAL_TILES];
};

#endif