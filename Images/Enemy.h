
#ifndef _____ENEMY_H_____
#define _____ENEMY_H_____
#include <string>
#include <vector>

#include "Inventory.h"
#include "Character.h"
#include "Item.h"
#include "Player.h"

class Enemy : public Character {
public:
	Enemy();
	Enemy(std::string n);
	Enemy(std::string n, int s, int i, int d, int c, int f);
	Enemy(std::string n, std::vector<Attribute> attr);
	Enemy(std::string name, std::string description, double range, int x0, int y0, int ai);
	/*
	compute overworld movement

	will alter this.xPosition and this.yPosition
	input: player x and y values
	*/
	void makeDecision(int x, int y);
	double getRange() { return maxRange; }
	int getXSpawn() { return xSpawn; }
	int getYSpawn() { return ySpawn; }

	operator std::string();

protected:
	std::string name;
	std::string description;

	//will not agro player if player outside of range
	double maxRange; 
	int xSpawn, ySpawn;

	/* 
	0: (agressive) npc will move toward player (if player currently w/i range)
		when player out of range, NPC moves back toward spawn

	1: (passive) npc will run away from player (if player gets w/i range)
		when player out of range, NPC sets new spawn
	*/
	int ai;
};
#endif
