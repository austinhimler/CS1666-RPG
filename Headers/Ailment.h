#pragma once
#include <string>
#include <Vector>
#include "Player.h"
#include "Ability.h"
#include "Resistance.h"

class Ailment {
public:
	Ailment(int n, double c, int v, int tn, int sa, int d);
	Ailment(int n, double c);
	Ailment(int n, int f1, int f2, Ability a);
	operator std::string();
protected:
	int name;
	double chance;
	int value;	//per turn
	int turnN;
	int stopAction;
	int degree;
	std::vector<Resistance> resistances; // can be null if no resistance matched
};
