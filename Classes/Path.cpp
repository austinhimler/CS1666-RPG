#include "../Headers/Path.h"
#include <stdio.h>
Path::Path(Tile* input[MAX_HORIZONTAL_TILES][MAX_VERTICAL_TILES])
{
	
		for (int y = 0; y < MAX_VERTICAL_TILES; y++)
		{
			for (int x = 0; x < MAX_HORIZONTAL_TILES; x++)
			{
				map[x][y] = input[x][y]->getType();
			}
		}
}
std::vector<Point*> Path::makeRandomPath(int currentX, int currentY)
{
	std::vector<Point*> myPath;
	int direction = rand() % 4;
	switch (direction)
	{
	case 0:
		for (int x = currentX+1; x < MAX_HORIZONTAL_TILES; x++)
		{
			if (isValid(x, currentY))
				myPath.push_back(new Point(x, currentY, x - 1, currentY));
			else break;
		}
		break;
	case 1:
		for (int x = currentX - 1; x > 0; x--)
		{
			if (isValid(x, currentY))
				myPath.push_back(new Point(x, currentY, x + 1, currentY));
			else break;
		}
		break;
	case 2:
		for (int y = currentY + 1; y < MAX_VERTICAL_TILES; y++)
		{
			if (isValid(currentX, y))
				myPath.push_back(new Point(currentX, y, currentX, y-1));
			else break;
		}
		break;
	case 3:
		for (int y = currentY - 1; y > 0; y--)
		{
			if (isValid(currentX, y))
				myPath.push_back(new Point(currentX, y, currentX, y + 1));
			else break;
		}
		break;
	default:
		std::cout << "WHOOPS" << std::endl;
	}
	return myPath;
}
std::vector<Point*> Path::makePath(int currentX, int currentY, int endX, int endY)
{
	Point* allPoints[MAX_HORIZONTAL_TILES][MAX_VERTICAL_TILES];
	
	for (int x = 0; x < MAX_HORIZONTAL_TILES; x++)
	{
		for (int y = 0; y < MAX_VERTICAL_TILES; y++)
		{
			allPoints [x][y] = nullptr;
		}
	}
	Point* current = new Point(currentX, currentY, 0, 0, 0, 0, 0, true, false);
	allPoints[currentX][currentY] = current;
	while(true)
	{ 
		if (isDestination(current->x,current->y,endX, endY))
			break;
		int temporary_x;
		int temporary_y;
		int temporary_h;
		if (current->x > 0)
		{
			temporary_x = current->x - 1;
			temporary_y = current->y;
			if (isValid(temporary_x, temporary_y))
			{
				if (allPoints[temporary_x][temporary_y] == nullptr)
				{
					temporary_h = calculateDist(temporary_x, temporary_y, endX, endY);
					allPoints[temporary_x][temporary_y] = new Point(temporary_x, temporary_y, current->x, current->y, current->moves + 1, temporary_h, temporary_h + current->moves + 1, false, true);
				}
				else if (allPoints[temporary_x][temporary_y]->closed == false)
				{
						if (allPoints[temporary_x][temporary_y]->moves > current->moves + 1)
						{
							temporary_h = calculateDist(temporary_x, temporary_y, endX, endY);
							allPoints[temporary_x][temporary_y] = new Point(temporary_x, temporary_y, current->x, current->y, current->moves + 1, temporary_h, temporary_h + current->moves + 1, false, true);
						}
				}
			}

		}
		
		if (current->x < MAX_X_POSITION)
		{
			temporary_x = current->x + 1;
			temporary_y = current->y;
			if (isValid(temporary_x, temporary_y))
			{
				if (allPoints[temporary_x][temporary_y] == nullptr)
				{
					temporary_h = calculateDist(temporary_x, temporary_y, endX, endY);
					allPoints[temporary_x][temporary_y] = new Point(temporary_x, temporary_y, current->x, current->y, current->moves + 1, temporary_h, temporary_h + current->moves + 1, false, true);
				}
				else if (allPoints[temporary_x][temporary_y]->closed == false)
				{
					if (allPoints[temporary_x][temporary_y]->moves > current->moves + 1)
					{
						temporary_h = calculateDist(temporary_x, temporary_y, endX, endY);
						allPoints[temporary_x][temporary_y] = new Point(temporary_x, temporary_y, current->x, current->y, current->moves + 1, temporary_h, temporary_h + current->moves + 1, false, true);
					}
				}
			}
		}
		if (current->y > 0)
		{
			temporary_x = current->x;
			temporary_y = current->y - 1;
			if (isValid(temporary_x, temporary_y))
			{
				if (allPoints[temporary_x][temporary_y] == nullptr)
				{
					temporary_h = calculateDist(temporary_x, temporary_y, endX, endY);
					allPoints[temporary_x][temporary_y] = new Point(temporary_x, temporary_y, current->x, current->y, current->moves + 1, temporary_h, temporary_h + current->moves + 1, false, true);
				}
				else if (allPoints[temporary_x][temporary_y]->closed == false)
				{
					if (allPoints[temporary_x][temporary_y]->moves > current->moves + 1)
					{
						temporary_h = calculateDist(temporary_x, temporary_y, endX, endY);
						allPoints[temporary_x][temporary_y] = new Point(temporary_x, temporary_y, current->x, current->y, current->moves + 1, temporary_h, temporary_h + current->moves + 1, false, true);
					}
				}
			}
		}
		if (current->y < MAX_Y_POSITION)
		{
			temporary_x = current->x;
			temporary_y = current->y + 1;
			if (isValid(temporary_x, temporary_y))
			{
				if (allPoints[temporary_x][temporary_y] == nullptr)
				{
					temporary_h = calculateDist(temporary_x, temporary_y, endX, endY);
					allPoints[temporary_x][temporary_y] = new Point(temporary_x, temporary_y, current->x, current->y, current->moves + 1, temporary_h, temporary_h + current->moves + 1, false, true);
				}
				else if (allPoints[temporary_x][temporary_y]->closed == false)
				{
					if (allPoints[temporary_x][temporary_y]->moves > current->moves + 1)
					{
						temporary_h = calculateDist(temporary_x, temporary_y, endX, endY);
						allPoints[temporary_x][temporary_y] = new Point(temporary_x, temporary_y, current->x, current->y, current->moves + 1, temporary_h, temporary_h + current->moves + 1, false, true);
					}
				}
			}
		}

		Point* low = nullptr;
		int lowScore = INT_MAX;
		for (int x = 0; x < MAX_HORIZONTAL_TILES; x++)
		{
			for (int y = 0; y < MAX_VERTICAL_TILES; y++)
			{
				if (allPoints[x][y] != nullptr)
				{
					if (allPoints[x][y]->open == true)
					{
						if (allPoints[x][y]->total < lowScore)
						{
							low = allPoints[x][y];
							lowScore = low->total;
						}
					}
				}
			}
		}
		if (low == nullptr)
		{
			std::vector<Point*> noPath;
			return noPath;
		}
		current = low;
		current->closed = true;
		current->open = false;
	}
	std::vector<Point*> fullPath = std::vector<Point*>(current->moves);
	for (int temp = current->moves - 1; temp >= 0; temp--)
	{
		fullPath.at(temp) = current;
		current = allPoints[current->parentX][current->parentY];
	}
	return fullPath;
}
bool Path::isValid(int x, int y)
{
	return map[x][y] == 0;
}

bool Path::isDestination(int x, int y, int endX, int endY)
{
	return (x == endX && y == endY);
}
int Path::calculateDist(int x, int y, int endX, int endY)
{
	return pow(x - endX ,2) + pow(y - endY,2);
}


