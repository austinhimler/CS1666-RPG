#ifndef _____ATTRIBUTE_H_____
#define _____ATTRIBUTE_H_____

#include <string>
#include <vector>

class Attribute {
public:
	Attribute();
	Attribute(std::string n);
	Attribute(std::string name, int stat);
	std::string toString();
protected:
	int base;
	int current;
	std::string name;

	friend class Character;
};

#endif