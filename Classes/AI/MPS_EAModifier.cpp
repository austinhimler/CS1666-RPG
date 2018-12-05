#include "../../Headers/AI/MPS_EAModifier.h"

MPS_EAModifier::MPS_EAModifier(std::vector<Character*> tar, Ability* abil, int baseEA) {
	Val = baseEA;
	if (abil->isAOE()) Val *= 2;
	Type = MPS_Resource::tEAM;
}

MPS_EAModifier::~MPS_EAModifier(){}

float MPS_EAModifier::getVal() {
	return Val;
}

int MPS_EAModifier::getType() {
	return Type;
}