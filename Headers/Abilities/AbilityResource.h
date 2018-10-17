#pragma once

#include <vector>

namespace AbilityResource {


	//ability descriptions
	inline std::string const &abilityDescrip(int n) {
		static std::string ability_description[] = {
			"Basic attack",
			"",
			"Try to escape from battle",
			"Take the defending stance and increase energy regeneration for next turn",
			""
		};
		return ability_description[n];
	}

	//abiilty base stats
	inline int const &baseEnergyCost(int n) {
		static int base_energy_cost[] = {
			1,
			1,
			1,
			1,
			1
		};
		return base_energy_cost[n];
	}

	//ability base CD
	const int abilityCD[] = {
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
		tSUMMON
	};

	const int abilityType[] = {
		tDAMAGE,
		tDAMAGE,
		tESCAPE,
		tDEFENSE,
		tSUMMON
	};

	//ability related attributes
	const int MAX_RELATED_ATTR = 3;

}