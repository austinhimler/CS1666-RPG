#include "../Headers/Ability.h"

	
	Ability::Ability(int n, std::string d, int ec, int cd, int v, int t)
	{
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
		IsAOE = AbilityResource::abilityIsAOE[n];
		MPSTaskType = AbilityResource::MPS_TASKTYPE[n];

	}
	Ability::Ability(int n, std::vector<int> re, std::vector<Attribute> attr) {
		name = n;
		description = AbilityResource::abilityDescrip(n);
		energyCost = AbilityResource::baseEnergyCost(n) - DEX / 10;
		baseValue = AbilityResource::baseValue(n);
		if (energyCost < 1) energyCost = 1;
		MPCost = AbilityResource::abilityMPCost[n];
		cooldown = AbilityResource::abilityCD[n];
		if (cooldown < 0) cooldown = 0;
		if (re.size() >= 3)
		{
			switch (name)
			{
			case ATTACK:
				value = baseValue + attr[re[0]].getCur() + ((attr[re[1]].getCur()+baseValue) / 2);
				break;
			case FIREBALL:
				value = (int)(2 * baseValue * attr[re[0]].getCur() + 1.5 * baseValue * attr[re[1]].getCur());
				break;
			case ARROWSHOT:
				value = baseValue + 2 * attr[re[0]].getCur();
				break;
			case DEFEND:
				value = baseValue + 2 * attr[re[0]].getCur();
				break;
			case SMITE:
				value = baseValue + 2 * attr[re[0]].getCur();
				break;
			case HEAL:
				value = baseValue + attr[re[0]].getCur();
				break;
			case MASSHEAL:
				value = baseValue + attr[re[0]].getCur();
				break;
			case ESCAPE:
				value = value = baseValue + attr[re[0]].getCur() / 10;
				break;
			default:
				value = 1;
			}
		}
		else value = 1;
		if (value < 0) value = 1;
		type = AbilityResource::abilityType[n];
		IsAOE = AbilityResource::abilityIsAOE[n];
		MPSTaskType = AbilityResource::MPS_TASKTYPE[n];
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
	int Ability::getMPSTaskType() {
		return MPSTaskType;
	}
	bool Ability::isAOE() {
		return IsAOE;
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
