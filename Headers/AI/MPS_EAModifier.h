#pragma once
#include "MPS_Modifier.h"
#include "MPS_Data.h"
#include "../Ability.h"
#include "../Character.h"

class MPS_EAModifier :  MPS_Modifier {
public:
	MPS_EAModifier(Character* tar, Ability* abil);
	~MPS_EAModifier();

	/** getters **/
	float getVal();
	int getType();
};