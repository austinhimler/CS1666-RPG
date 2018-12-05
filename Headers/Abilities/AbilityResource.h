
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
		"Arrow Shot",
		"Defend",
		"Smite",
		"Heal",
		"Mass Heal",
		"Escape"
	};

	//ability descriptions
	inline std::string const &abilityDescrip(int n) {
		static std::string ability_description[] = {
			"Basic attack",
			"Cast a Fireball, potentially causing some burnt on target",
			"Shoot a arrow at blinding speed",
			"Take the defending stance and increase energy regeneration for next turn",
			"Smite your enemy to oblivion",
			"Restore HP of one target",
			"Restore HP across whole team",
			"Try to escape from combat"
		};
		return ability_description[n];
	}

	//abiilty base stats
	inline int const &baseEnergyCost(int n) {
		static int base_energy_cost[] = {
			10,
			25,
			30,
			1,
			50,
			30,
			60,
			50
		};
		return base_energy_cost[n];
	}

	//abiilty base value
	inline int const &baseValue(int n) {
		static int base_value[] = {
			10,
			25,
			30,
			8,
			100,
			100,
			60,
			50
		};
		return base_value[n];
	}

	// ability base MP cost
	const int abilityMPCost[] = {
		0,
		10,
		0,
		0,
		100,
		100,
		100,
		0
	};

	//ability base CD
	const int abilityCD[] = {
		0,
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
		tABILITY_TYPE_NUM,
	};

	// types of the respective abilities
	const int abilityType[] = {
		tDAMAGE, // attack
		tDAMAGE, // fireball
		tDAMAGE, // arrow shot
		tDEFENSE, // defend
		tDAMAGE, // smite
		tHEALING, // heal
		tHEALING, // mass heal
		tESCAPE // escape
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
		true,
		false
	};

	// related attributes of each abiility in the order of influence
	const std::vector<int> abilityAttr[] = {
		{STR, DEX, CON},
		{INT, DEX, FAI},
		{DEX, CON, STR},
		{CON, STR, DEX},
		{FAI, INT, DEX},
		{FAI, INT, CON},
		{FAI, INT, CON},
		{DEX, FAI, INT}
	};

	//ability related attributes
	const int MAX_RELATED_ATTR = 3;


	using namespace MPS_Resource;
	const int MPS_TASKTYPE[] = {
		tMPS_TASK_DAMAGE,
		tMPS_TASK_DAMAGE,
		tMPS_TASK_DAMAGE,
		tADD_SELF_RE,
		tMPS_TASK_DAMAGE,
		tRECOVER_SELF_HP,
		tRECOVER_FRIEND_HP,
		tMPS_TASK_ESCAPE,
	};
	using namespace std;
}
#endif