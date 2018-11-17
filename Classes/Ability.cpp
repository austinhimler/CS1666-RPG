#include "../Headers/Ability.h"
#include "../Headers/Abilities/AbilityResource.h"
	
	Ability::Ability(int n, std::string d, int ec, int cd, int v, int t) {
		name = n;
		description = d;
		energyCost = ec;
		MPCost = AbilityResource::abilityMPCost[n];
		if (energyCost < 1) energyCost = 1;
		cooldown = cd;
		if (cooldown < 0) cooldown = 0;
		value = v; 
		type = t;
		if (value < 0) value = 1;
	}
	Ability::Ability(int n, std::vector<int> re, std::vector<Attribute> attr) {
		name = n;
		description = AbilityResource::abilityDescrip(n);
		energyCost = AbilityResource::baseEnergyCost(n) - DEX / 10;
		if (energyCost < 1) energyCost = 1;
		MPCost = AbilityResource::abilityMPCost[n];
		cooldown = AbilityResource::abilityCD[n];
		if (cooldown < 0) cooldown = 0;
		if (re.size() >= 3)
			value = 6 * attr[re[0]].getCur() + 3 * attr[re[1]].getCur() + 2 * attr[re[2]].getCur();
		else
			value = 0;
		if (value < 0) value = 1;
		type = AbilityResource::abilityType[n];
	}


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
	int Ability::getMPCost() {
		return MPCost;
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

	Ability::operator std::string() {
		std::string s = AbilityResource::abilityNames[name] + "\n";
		s += AbilityResource::abilityDescrip(name) + "\n";
		s += "Energy Cost: " + energyCost;
		s += "\nCool Down: " + cooldown;
		s += "\nRelated Attributes: " + ATTR_NAMES[AbilityResource::abilityAttr[type][0]] + ", " + ATTR_NAMES[AbilityResource::abilityAttr[type][1]] + ", "+ ATTR_NAMES[AbilityResource::abilityAttr[type][2]] + "\n";
		s += "Value: " + value;
		return s;
	}
