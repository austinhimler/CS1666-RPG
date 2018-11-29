#pragma once
#include "MPS_Modifier.h"
#include "CombatAIResource.h"
#include "../Character.h"

class MPS_ASModifier : public MPS_Modifier {
protected:
public:
	/** constructors and destructors **/
	// set Val = 1 and Type = tASM
	MPS_ASModifier();
	// currently just calls the default constructor
	MPS_ASModifier(Character* c);
	// currently doesn't do anything
	MPS_ASModifier(std::string Path); 
	~MPS_ASModifier();

	/** getters **/
	int getType();
	float getVal();
};