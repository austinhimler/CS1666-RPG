
#ifndef _____ABILITY_H_____
#define _____ABILITY_H_____
#include <string>
#include <vector>
#include "Attribute.h"
#include "Globals.h"
#include "Ailment.h"

/*
*	Curretnly not supporting abilities involving ailments and Resistance
*/

class Ability {
public:
	Ability(int n, std::vector<int> re, std::vector<Attribute> attr);
	Ability(int n, std::string d, int eC, int cD, int v, int t);
	Ability();

	// compare function, return true if names are the same, otherwise false
	bool cmp(Ability a);
	bool cmp(int a);
	
	//getters
	int getName();
	int getEnergyCost();
	int getMPCost();
	int getCD();
	int getVal();
	int getType();

	operator std::string();
private:
	int name;
	std::string description;
	int energyCost;
	int MPCost;
	int cooldown; //might not add
	int value;	// the possiblity of succes for escape and summon
	int type;
	//maybe determine these with another subclass of each type? e.g. class for DamageAbility, HealingAbility, DefenseAbility
	//or we might not even need something like this
	bool isDamage;
	bool isHealing;
	bool isDefense;

	// attributes affecting this ability, assuming maximum 3 attributes can affect it
	// stores the enum of the attributes
	std::vector<int> relatedAttr;

	//std::vector<Resistance> resistancesEffected;
	//Attribute abilityAttributeType;

};
#endif