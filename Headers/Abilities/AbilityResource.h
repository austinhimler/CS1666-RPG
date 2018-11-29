
#ifndef _____ABILITYRESOURCE_H_____
#define _____ABILITYRESOURCE_H_____
#include <vector>
#include "../Attributes/Attr_Globals.h"
#include "../AI/CombatAIResource.h"

namespace AbilityResource {


	// ability names in stirng
	// always exhaustingly list abilities of the previous attribute before starting to list abilites of the next attribute, same apply to all other const arrays / vectors / enum

	const std::string abilityNames[] = {
		"Attack",
		"Fireball",
		"Escape",
		"Defend",
		"Summon",
		"Heal",
		"Mass Heal"
	};

	//ability descriptions
	inline std::string const &abilityDescrip(int n) {
		static std::string ability_description[] = {
			"Basic attack",
			"Cast a Fireball, potentially causing some burnt on target",
			"Try to escape from battle",
			"Take the defending stance and increase energy regeneration for next turn",
			"Try to summon an unknonw...emmmmmm...thing, to \"help\" your fight",
			"Restore HP of one target",
			"Restore HP across whole team"
		};
		return ability_description[n];
	}

	//abiilty base stats
	inline int const &baseEnergyCost(int n) {
		static int base_energy_cost[] = {
			20,
			25,
			10,
			0,
			50,
			50,
			50
		};
		return base_energy_cost[n];
	}

	//abiilty base value
	inline int const &baseValue(int n) {
		static int base_value[] = {
			10,
			3,
			60,
			8,
			0
		};
		return base_value[n];
	}

	// ability base MP cost
	const int abilityMPCost[] = {
		0,
		10,
		0,
		0,
		10000,
		10000,
		10000
	};

	//ability base CD
	const int abilityCD[] = {
		0,
		0,
		0,
		0,
		0,
		0,
		0
	};
	
	//ability type
	enum ABILITY_TYPE {
		tDAMAGE,
		tHEALING,
		tDEFENSE,
		tAILMENT,
		tRESISTANCE,
		tBUFF,
		tESCAPE,
		tSUMMON,
		tABILITY_TYPE_NUM
	};

	// types of the respective abilities
	const int abilityType[] = {
		tDAMAGE, // attack
		tDAMAGE, // fireball
		tESCAPE, // escape
		tDEFENSE, // defend
		tSUMMON, // summon
		tHEALING, // heal
		tHEALING // mass heal
				//etc.
	};

	// isAOE of the respective abilites
	const bool abilityIsAOE[] = {
		false,
		false,
		false,
		false,
		false,
		false,
		true
	};

	// related attributes of each abiility in the order of influence
	const std::vector<int> abilityAttr[7] = {
		{STR, DEX, CON},
		{INT, DEX, FAI},
		{DEX, CON, STR},
		{CON, STR, DEX},
		{FAI, INT, DEX},
		{FAI, INT, CON},
		{FAI, INT, CON}
	};

	//ability related attributes
	const int MAX_RELATED_ATTR = 3;


	using namespace MPS_Resource;
	const int MPS_TASKTYPE[] = {
		tMPS_TASK_DAMAGE,
		tMPS_TASK_DAMAGE,
		tMPS_TASK_ESCAPE,
		tADD_SELF_RE,
		tADD_FRIEND_NUM,
		tRECOVER_SELF_HP,
		tRECOVER_SELF_HP
	};
	using namespace std;
}
#endif