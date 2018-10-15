#ifndef _____CHARACTER_H_____
#define _____CHARACTER_H_____

#include <string>
#include <vector>
#include "Attribute.h"
#include "GLobals.h"
#include "Resistance.h"
#include "Ability.h"

/*
#include "Attributes/Strength.h"
#include "Attributes/Intelligence.h"
#include "Attributes/Dexterity.h"
#include "Attributes/Constitution.h"
#include "Attributes/Faith.h"
*/


class Character {

public:
	Character();
	Character(std::string n);
	Character(std::string n, int s, int i, int d, int c, int f);
	Character(std::string n, std::vector<Attribute> attr);
	int getHPMax();
	int getMPMax();
	int getEnergyMax();
	void takeDamage(Ability a);
	
protected:
	int hpMax;
	int hpCurrent;
	int energyMax;
	int energyCurrent;
	int mpMax;
	int mpCurrent;
	std::string imageResource;
	std::string name;


	void setHPMax();
	void setMPMax();
	void setEnergyMax();
	/*
	Strength strength;
	Intelligence intelligence;
	Dexterity dexterity;
	Constitution constitution;
	Faith faith;
	*/
	std::vector<Attribute> attributes;
	std::vector<Ability> abilities;

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