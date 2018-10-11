#ifndef _____ACTION_H_____
#define _____ACTION_H_____

#include <string>
#include <vector>

class Action {
public:
	Action();
	Action(std::string name);
	Action(std::string name, std::moveType, double damage);
	boolean setType(std::string type);
	/*
	return 0 if change cannot be done (i.e. type not legal type)
	return 1 if sucessful
	*/
	boolean setDamage(double damage);
	/*
	return 0 if change not completed (i.e. damage illegal value)
	return 1 if sucessful
	*/
	boolean setName(std::string name);
	/*
	return 0 if change not completed (i.e. name could not be read as string)
	return 1 if sucessful
	*/

private:
	std::string name;
	std::string moveType;
	double damage;
};

#endif