#include "../Headers/Ability.h"

	Ability::Ability() {}
	Ability::Ability(int n, std::vector<int> re, std::vector<Attribute> attr) {
		int ec = AbilityResource::baseEnergyCost(n) - DEX/10;
		int cd = AbilityResource::abilityCD[n];
		int t = AbilityResource::abilityType[n];
		int v = 6 * re[0] + 3 * re[1] + 2 * re[2];
		Ability(n, AbilityResource::abilityDescrip(n), ec, cd, v, t, re);
	}
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