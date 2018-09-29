#include <string>
#include <vector>
class Character {
public:
	int hpMax;
	int hpCurrent;
	int energyMax;
	int energyCurrent;
	std::string imageResource;
	std::vector<Attribute> attributes;
	std::vector<Abilitiy> abilities;
	std::vector<Resistance> resistances;

	Character() {

	}

	Character(Strength strength, Intelligence intelligence, Dexterity dexterity, Constitution constitution, Faith faith) {

	}
};