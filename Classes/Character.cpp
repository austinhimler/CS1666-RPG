#include <string>
#include <vector>
class Character {
public:
	int hpMax;
	int hpCurrent;
	int energyMax;
	int energyCurrent;
	int mpCurrent;
	int mpMax;
	std::string imageResource;

	//std::vector<Attribute> attributes;
	//Had array of attributes in github todo but since all will have the same 5, probably better to just list them
	Strength strength;
	Intelligence intelligence;
	Dexterity dexterity;
	Constitution constitution;
	Faith faith;

	std::vector<Abilitiy> abilities;
	//std::vector<Resistance> resistances;
	//have an array or list each resistance out?
	Resistance evasion;
	Resistance physical;
	Resistance fire;
	Resistance earth;
	Resistance water;
	Resistance air;
	Resistance light;
	Resistance dark;

	Character() {

	}

	Character(Strength strength, Intelligence intelligence, Dexterity dexterity, Constitution constitution, Faith faith) {
		this->strength = strength;
		this->intelligence = intelligence;
		this->dexterity = dexterity;
		this->constitution = constitution;
		this->faith = faith;
		mpCurrent = determineMPMax();
		hpCurrent = determineHPMax();
		energyCurrent = determineEnergyMax();
	}

	void determineHPMax() {

	}

	void determineMPMax() {

	}

	void determineEnergyMax{

	}
};