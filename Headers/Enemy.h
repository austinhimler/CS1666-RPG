#pragma once
#include "Character.h"
#include "Item.h"
#include <string>
#include <vector>
class Enemy : public Character {
	Enemy();
	std::string description;
	std::vector<Item> lost;
	int experienceGiven;
};
