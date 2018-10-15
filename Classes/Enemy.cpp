#include "../Headers/Character.h"
#include "../Headers/Item.h"
#include <string>
#include <vector>
class Enemy : public Character {
public:
	std::string description;
	std::vector<Item> loot;
	int experienceGiven;
};