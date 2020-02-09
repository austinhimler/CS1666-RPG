
#ifndef _____CLUSTER_H_____
#define _____CLUSTER_H_____
#include <string>
#include <vector>
#include <queue>
#include "Enemy.h"
#include "Tile.h"
#include "Path.h"
#include <random>
#include "Helper.h"
class Cluster : public Character
{
public:
	Cluster();
	Cluster(int q);
	Cluster(std::string n);
	Cluster(std::string n, int q);
	vector<Character*> characterGroup;
	Character* targetPlayer;
	int targetTileX;
	int targetTileY;
	int pursuitRange;
	int isAlive();
	int clusterSize;
	bool combatReady;
	int readyTimeLeft;
	std::vector<Point*> currentPath;
	std::string ptoString();
	void fromString(std::string in);
	void setTarget(Character*);
	void clearTarget();
	void findRandom(Tile*[MAX_HORIZONTAL_TILES][MAX_VERTICAL_TILES]);
	void findPath(Tile*[MAX_HORIZONTAL_TILES][MAX_VERTICAL_TILES]);
	void moveSteps(double);
	int pathOffset;
	double partialSteps;

	int lastDirection = -1;
	std::queue<int> seqX;
	std::queue<int> seqY;
	double aRange = 1750.0;
};
#endif