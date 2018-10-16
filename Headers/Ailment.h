#pragma once
#include <string>

enum AILMENTS {
	SILENCE,
	POISON,
};

class Ailment {
public:
	Ailment(std::string n, double c);
protected:
	int name;
	double chance;
};
