#include "../Headers/Ability.h"

	Ability::Ability(int n, std::string d, int ec, int cd, int v, int t, std::vector<int> re) {
		name = n;
		description = d;
		energyCost = ec;
		if (energyCost < 1) energyCost = 1;
		cooldown = cd;
		if (cooldown < 0) cooldown = 0;
		type = t;
		relatedAttr = re;
		value = v;
		if (value < 0) value = 1;
	}
	Ability::Ability(int n, std::vector<int> re, std::vector<Attribute> attr) : Ability(n, AbilityResource::abilityDescrip(n), AbilityResource::baseEnergyCost(n) - DEX / 10, AbilityResource::abilityCD[n], 6 * re[0] + 3 * re[1] + 2 * re[2], AbilityResource::abilityType[n], re) {}

	Ability::Ability() {}


	bool Ability::cmp(Ability a) {
		if (a.name == name) return true;
		else return false;
	}

	bool Ability::cmp(int a) {
		if (name == a) return true;
		else return false;
	}

	//getters
	int Ability::getName() {
		return name;
	}
	int Ability::getEnergyCost() {
		return energyCost;
	}
	int Ability::getCD() {
		return cooldown;
	}
	int Ability::getVal() {
		return value;
	}
	int Ability::getType() {
		return type;
	}