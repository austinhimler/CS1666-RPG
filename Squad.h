#ifndef _____SQUAD_H_____
#define _____SQUAD_H_____

#include <string>
#include <vector>
#include "Headers/Player.h"

class Squad
{
public:
	Squad(std::string name);

	bool addPlayer(Player p);

	int squadSize();
	bool isEmpty();
	bool isFull();
private:
	std::string name;
	std::vector<Player> squad;
	int size;
};

#endif 