#pragma once
#include "MPS_Modifier.h"
#include "CombatAIResource.h"

class MPS_ASModifier : public MPS_Modifier {
protected:
public:
	/** constructors **/
	MPS_ASModifier();
	MPS_ASModifier(std::string Path);

	/** getters **/
	int getType();
	float getVal();
};