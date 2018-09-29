#include <string>
#include <vector>
class Ability {
public:
	std::string name;
	std::string description;
	int energyCost;
	int cooldown; //might not add
	
	//maybe determine these with another subclass of each type? e.g. class for DamageAbility, HealingAbility, DefenseAbility
	//or we might not even need something like this
	bool isDamage;
	bool isHealing;
	bool isDefense;

	std::vector<Resistance> resistancesEffected;
	Attribute abilityAttributeType;
};