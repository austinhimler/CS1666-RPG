#ifndef _____OverworldPathing_H_____
#define _____OverworldPathing_H_____
#include <string>
#include <vector>
#include <math.h>
#include <stack>
#include <array>
#include <exception>
#include "Tile.h"
#include "Cluster.h"
#include "Player.h"
#include "Globals.h"

struct Node {
	int x;
	int y;
	int parentX;
	int parentY;
	float gCost;
	float hCost;
	float fCost;
};

inline bool operator < (const Node& a, const Node& b) {
	return a.fCost < b.fCost;
}

class OverworldPathing {
public:
	OverworldPathing();
	OverworldPathing(Tile* map[900]);

	void test(Cluster* c, Player* p);
	void pathSequence(Cluster* c, Player* p);
	void updatePlayerLocation(Player* p);
	int getDirection(Cluster* c, Player* p);
private:
	int mapping[900];
	int playerTile = -1;
	/*
	int currentTile = (int)(player1.xPosition + (player1.rectangle.w / 2)) / TILE_WIDTH;
	currentTile += (int)((player1.yPosition + player1.rectangle.h) / TILE_HEIGHT) * 30;
	*/
	bool isValid(int x, int y);
	bool isDestination(int x, int y, Node dest);
	double calculateH(int x, int y, Node dest);
	std::vector<Node> aStar(Node p, Node dest);
	std::vector<Node> makePath(std::array<std::array<Node, 30>, 30> map, Node dest);
};
#endif
