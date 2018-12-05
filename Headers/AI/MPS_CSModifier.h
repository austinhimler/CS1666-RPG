#pragma once
#include "MPS_Modifier.h"
#include "CombatAIResource.h"
#include "../Character.h"

class MPS_CSModifier : public MPS_Modifier {
protected:
public:
	/** constructors and destructors **/
	// set Val = 1 and Type = tASM
	MPS_CSModifier();
	MPS_CSModifier(Character* c, int t);
	// currently doesn't do anything
	MPS_CSModifier(std::string Path);
	~MPS_CSModifier();

	/** getters **/
	int getType();
	float getVal();
};