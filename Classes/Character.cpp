#include <string>
#include <vector>
#include "../Headers/Attribute.h"
#include "../Headers/Attributes/Strength.h"
#include "../Headers/Attributes/Intelligence.h"
#include "../Headers/Attributes/Dexterity.h"
#include "../Headers/Attributes/Constitution.h"
#include "../Headers/Attributes/Faith.h"
#include "../Headers/Character.h"


	//std::vector<Attribute> attributes;
	//Had array of attributes in github todo but since all will have the same 5, probably better to just list them
	


	//std::vector<Resistance> resistances;
	//have an array or list each resistance out?
	/*Resistance evasion;
	Resistance physical;
	Resistance fire;
	Resistance earth;
	Resistance water;
	Resistance air;
	Resistance light;
	Resistance dark;*/

	Character::Character() {}

	Character::Character(Strength strength, Intelligence intelligence, Dexterity dexterity, Constitution constitution, Faith faith) {
		this->strength = strength;
		this->intelligence = intelligence;
		this->dexterity = dexterity;
		this->constitution = constitution;
		this->faith = faith;
		mpCurrent = determineMPMax();
		hpCurrent = determineHPMax();
		energyCurrent = determineEnergyMax();
	}

	int Character::determineHPMax() {

		return 100;
	}

	int Character::determineMPMax() {
		return 100;
	}

	int Character::determineEnergyMax(){
		return 100;
	}
