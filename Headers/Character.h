#pragma once
#include "Attribute.h"
#include <string>
#include <vector>
#include "Attributes/Strength.h"
#include "Attributes/Intelligence.h"
#include "Attributes/Dexterity.h"
#include "Attributes/Constitution.h"
#include "Attributes/Faith.h"
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
class Character {
	Character();
	Character(Strength strength, Intelligence intelligence, Dexterity dexterity, Constitution constitution, Faith faith);
	int determineHPMax();
	int determineMPMax();
	int determineEnergyMax();
};


std::vector<Ability> abilities;
/*Resistance evasion;
Resistance physical;
Resistance fire;
Resistance earth;
Resistance water;
Resistance air;
Resistance light;
Resistance dark;*/