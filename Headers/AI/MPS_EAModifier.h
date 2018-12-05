#pragma once
#include "MPS_Modifier.h"
#include "CombatAIResource.h"
#include "MPS_Data.h"
#include "../Ability.h"
#include "../Character.h"

class MPS_EAModifier :  MPS_Modifier {
public:
	// currently takes baseEA as the Val
	MPS_EAModifier(std::vector<Character*> tar, Ability* abil, int baseEA);
	~MPS_EAModifier();

	/** getters **/
	float getVal();
	int getType();
};