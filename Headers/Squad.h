#ifndef _____SQUAD_H_____
#define _____SQUAD_H_____

#include <string>
#include <vector>
#include "Player.h"

class Squad {
public:
	Squad();
	Squad(Player leader);
	addMember(Player p);
private:
	std::vector<Player> mySquad[3];
	int size;
	Player leader;
};

#endif