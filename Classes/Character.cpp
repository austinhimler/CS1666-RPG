#include "../Headers/Character.h"


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
	Character::Character(std::string n, int s, int i, int d, int c, int f) {
		attributes = std::vector<Attribute>(5);
		attributes[STR] = Attribute("Strength", s);
		attributes[INT] = Attribute("Intelligence", i);
		attributes[DEX] = Attribute("Dexerity", d);
		attributes[CON] = Attribute("Constitution", c);
		attributes[FAI] = Attribute("Faith", f);
		new Character(n, attributes);
	}
	Character::Character(std::string n) {
		Character(n, 1, 1, 1, 1, 1);
	}
	Character::Character() {
		Character("Character 1");
	}

	int Character::getHPMax() {
		return hpMax;
	}
	
	int Character::getMPMax() {
		return mpMax;
	}

	int Character::getEnergyMax() {
		return energyMax;
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

	int Character::getDex() {
		return attributes[DEX].current;
	}

	int Character::getHPCur() {
		return hpCurrent;
	}