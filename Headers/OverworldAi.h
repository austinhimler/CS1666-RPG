#ifndef _____OVERWORLDAI_H_____
#define _____OVERWORLDAI_H_____

#include <string>
#include <vector>
#include "Ai.h"

class OverworldAi : public Ai{
public:
	OverworldAi();
	OverworldAi(std:string name);
	boolean changeType(boolean a) { return false;  }
private:
	std::string name;
	boolean type = 0;
};

#endif