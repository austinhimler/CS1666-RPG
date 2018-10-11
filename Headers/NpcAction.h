#ifndef _____NPCACTION_H_____
#define _____NPCACTION_H_____

#include <string>
#include <vector>
#include "Action.h"

class NpcAction : public Action {
public:
	NpcAction();
	NpcAction(std::string name);
	NpcAction(std::string name, std::string moveType, double damage);
private:
	std::string name;
	std::string moveType;
	double damage;
};

#endif