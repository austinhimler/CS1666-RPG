#ifndef _____COMBATAI_H_____
#define _____COMBATAI_H_____

#include <string>
#include <vector>
#include "Ai.h"

class CombatAi : public Ai {
public:
	CombatAi();
	CombatAi(std::string name);
	boolean changeType(boolean temp) { return false; }
private:
	std::string name;
	boolean type = 1;
};

#endif