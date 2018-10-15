#ifndef _____ABILITY_H_____
#define _____ABILITY_H_____

#include <string>
#include <vector>
#include "Attribute.h"

//#include "../Headers/Attribute.h"
//#include "../Headers/Resistance.h"

class Ability {
public:
	Ability();
	//Ability(std::string n, std::string d, int eC, int cD, bool isD, bool isH, bool isDf, std::vector<Resistance> rE, Attribute aAT);
private:
	std::string name;
	std::string description;
	int energyCost;
	int cooldown; //might not add

	//maybe determine these with another subclass of each type? e.g. class for DamageAbility, HealingAbility, DefenseAbility
	//or we might not even need something like this
	bool isDamage;
	bool isHealing;
	bool isDefense;
	
	// attributes affecting this ability, assuming maximum 3 attributes can affect it
	// stores the enum of the attributes
	int relatedAttr[3];

	//std::vector<Resistance> resistancesEffected;
	//Attribute abilityAttributeType;
};

#endif // !_____ABILITY_H_____
