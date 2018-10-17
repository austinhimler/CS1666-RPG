#pragma once

#include <string>
#include <vector>

#include "Character.h"
#include "Item.h"

class Enemy : public Character {
public:
	Enemy();
	Enemy(std::string n, int s, int i, int d, int c, int f);
protected:
	std::string description;
	std::vector<Item> lost;
	int experienceGiven;
};
