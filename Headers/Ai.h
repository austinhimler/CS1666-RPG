#ifndef _____AI_H_____
#define _____AI_H_____

#include <string>
#include <vector>

class Ai {
public:
	Ai();
	Ai(std::string name);
	Ai(boolean type);
	boolean changeType(boolean temp);
	/*
	return true if change successful
	return false if change unsuccessful (i.e. temp == this.type)
	*/
private:
	std::string name;
	boolean type;
	/*
	0: overworld
	1: combat
	*/
};

#endif