
#ifndef _____PLAYER_H_____
#define _____PLAYER_H_____
#include <string>
#include <vector>

#include "Inventory.h"
#include "Ability.h"
#include "Character.h"

class Player : public Character
{
public:
	//stores stats
	Player();
	Player(std::string n);
	Player(std::string n, int s, int i, int d, int c, int f);
	Player(std::string n, std::vector<Attribute> attr);
	void setAll(std::string n, int s, int i, int d, int c, int f);
	int getLevel();
	int getCurrentExperience();
	operator std::string();
private:
	int level;
	int currentExperience;
	Inventory inventory;
	
			
};
#endif