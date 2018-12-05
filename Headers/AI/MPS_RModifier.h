#pragma once
#include "MPS_Modifier.h"
#include "../Ability.h"
#include "CombatAIResource.h"
#include "../Enemy.h"

class MPS_RModifier : MPS_Modifier{
public:
	MPS_RModifier(Enemy* Self, Ability* abil);
	~MPS_RModifier();

	/** getters **/
	float getVal();
	int getType();
};