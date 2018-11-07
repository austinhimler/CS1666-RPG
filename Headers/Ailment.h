
#ifndef _____AILMENT_H_____
#define _____AILMENT_H_____
#include <string>
#include <Vector>
#include <Random>
#include "Globals.h"
#include "Resistance.h"

/**
*	Currently Assuming only 5 degrees of each ailments, if more/less degress needed, the constructor needs to be reworked
**/

class Ailment {
public:
	Ailment();
	Ailment(int n, double c, int v, int tn, int sa, int d);
	Ailment(int n, double c);
	Ailment(int n, int f1, int f2, int val);	// val is the return value of abil.getVal(), where abil is the ability that creates this ailment
	operator std::string();
	bool operator==(const Ailment& ailm);
	bool operator!=(const Ailment& ailm);
	Ailment operator=(const Ailment& ailm);

	string getName();
	int getVal();
	int getChance();
	int getTN();
	int getSA();
	int getDegree();
	int getType();
	std::vector<Resistance> getREs();

	bool combineAilment(Ailment& ailm);
	void update();

protected:
	int name;
	int chance;	// chance of hitting the target in percent
	int value;	//per turn, negative means adding this value to character, positive means decreasing, can also be 0 (i.e. silence, which doesn't affect mp/hp/energy, just the target can't cast spell)
	int turnN;	// the turns for which the ailment will affect the player's hp/mp/energy, the length of the player can't take actions, if any, is stored by stopAction, not turnN
	int stopAction; // 0 if the ailment doesn't prevet target from taking actions, can NOT be negative?
	int degree;	
	int type;	// determine either HP, MP, energy, any two of these, or all of them are affected
	std::vector<Resistance> resistances; // can be null if no resistance matched

	const int DEGREE_FACTOR = 10;
	const int BASE_CHANCE = 10;
	const int ABIL_FACTOR = 3;
};
#endif
