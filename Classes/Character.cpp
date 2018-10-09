#include "../Headers/Character.h"

	Character::Character() {
		Character("Character 1");
	}
	Character::Character(std::string n) {
		Character(n, 1,1,1,1,1);
	}
	Character::Character(std::string n, int s, int i, int d, int c, int f) {
		attributes = std::vector<Attribute>(5);
		attributes[STR] = Attribute("Strength", s);
		attributes[INT] = Attribute("Intelligence", i);
		attributes[DEX] = Attribute("Dexerity", d);
		attributes[CON] = Attribute("Constitution", c);
		attributes[FAI] = Attribute("Faith", f);
		Character(n, attributes);

	}
	Character::Character(std::string n, std::vector<Attribute> attr) {
		attributes = attr;
		setHPMax();
		setMPMax();
		setEnergyMax();
		hpCurrent = hpMax;
		mpCurrent = mpMax;
		energyCurrent = energyMax;
		name = n;
	}

	void Character::setHPMax() {
		hpMax = 100 * attributes[CON].current;
	}

	void Character::setMPMax() {
		mpMax = 100 * attributes[INT].current;
	}

	void Character::setEnergyMax() {
		energyMax = 100 * attributes[DEX].current;
	}
	/*
	int Character::getHPMax() {
		return 100 * CON;
	}

	int Character::getMPMax() {
		return 100;
	}

	int Character::getEnergyMax(){
		return 100;
	}
	*/