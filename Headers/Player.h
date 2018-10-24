#pragma once

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
	operator std::string();
private:
	int currentExprience;
	Inventory inventory;
	
			
};