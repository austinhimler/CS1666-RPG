#pragma once
#include <vector>

namespace AilmentResource {


	// ailment names in stirng
	// always exhaustingly list abilities of the previous attribute before starting to list abilites of the next attribute, same apply to all other const arrays / vectors / enum

	const std::string ailmentNames[] = {
		"Silence",
		"Poison",
		"Fire",
		"Stun",
		"Curse"
	};

	//ailment descriptions
	inline std::string const &ailmentDescrip(int n) {
		static std::string ailment_description[] = {
			"Target can't use any spell",
			"Just poison, nothing surprising",
			"on FIRE!",
			"If stunned, can't take action",
			"Target is destined to death when the time comes",
		};
		return ailment_description[n];
	}

	// related attributes of each abiility in the order of influence
	const std::vector<int> ailmentRes = {

	};

	// compensation value for change depending on ailments
	const int ailmentCompen[] = {
		10,
		10,
		10,
		10,
		1
	};

	// number of turns the target can't take action
	const int ailmentSA[] = {
		0,
		0,
		0,
		3,
		0
	};

	enum AILMENT_TYPE {
		tHP_ONLY,
		tMP_ONLY,
		tEN_ONLY,
		tHM,
		tHE,
		tME,
		tALL,
		tNONE,
		tCURSE,
	};

	const int ailmentType[] = {
		tNONE,
		tHE,
		tHP_ONLY,
		tHP_ONLY,
		tCURSE
	};
	/*
	//ailment base CD
	const int ailmentCD[] = {
		0,
		0,
		0,
		0,
		0
	};//*/

	/*
	//ailment type
	enum ailment_TYPE {
		tDAMAGE,
		tHEALING,
		tDEFENSE,
		tAILMENT,
		tRESISTANCE,
		tBUFF,
		tESCAPE,
		tSUMMON
	};
	// types of the respective abilities
	const int ailmentType[] = {
		tDAMAGE, // attack
		tDAMAGE, // fireball
		tESCAPE, // escape
		tDEFENSE, // defend
		tSUMMON // summon
				//etc.
	};
	//*/
}