#include "../../Headers/AI/MPS_EAModifier.h"

MPS_EAModifier::MPS_EAModifier(Character* tar, Ability* abil) {
	MPS_Data BaseEAMData("Data/AI/MPS_Effectiveness.csv");
	std::vector<float> tmp = BaseEAMData.getData();
	
}

MPS_EAModifier::~MPS_EAModifier(){}

float MPS_EAModifier::getVal() {
	return Val;
}

int MPS_EAModifier::getType() {
	return Type;
}