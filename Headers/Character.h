#ifndef _____CHARACTER_H_____
#define _____CHARACTER_H_____

#include "Attribute.h"
#include <string>
#include <vector>
#include "Attributes/Strength.h"
#include "Attributes/Intelligence.h"
#include "Attributes/Dexterity.h"
#include "Attributes/Constitution.h"
#include "Attributes/Faith.h"

class Character {

public:
	int hpMax;
	int hpCurrent;
	int energyMax;
	int energyCurrent;
	int mpMax;
	int mpCurrent;
	std::string imageResource;
	Strength strength;
	Intelligence intelligence;
	Dexterity dexterity;
	Constitution constitution;
	Faith faith;
	std::vector<Ability> abilities;

	Character();
	Character(Strength strength, Intelligence intelligence, Dexterity dexterity, Constitution constitution, Faith faith);
	int determineHPMax();
	int determineMPMax();
	int determineEnergyMax();
private:
};



/*Resistance evasion;
Resistance physical;
Resistance fire;
Resistance earth;
Resistance water;
Resistance air;
Resistance light;
Resistance dark;*/
#endif