#pragma once

#include <string>
#include <vector>
#include "Attribute.h"
#include "Globals.h"

/*
*	Curretnly not supporting abilities involving ailments and Resistance
*/

class Ability {
public:
	Ability(int n, std::vector<int> re, std::vector<Attribute> attr);
	Ability(int n, std::string d, int eC, int cD, int v, int t, std::vector<int> rE);
	Ability();

	// compare function, return true if names are the same, otherwise false
	bool cmp(Ability a);
	bool cmp(int a);
	
	//getters
	int getName();
	int getEnergyCost();
	int getCD();
	int getVal();
	int getType();

private:
	int name;
	std::string description;
	int energyCost;
	int cooldown; //might not add
	int value;
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
