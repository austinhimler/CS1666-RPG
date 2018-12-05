#include "../../Headers/AI/MPS_ASModifier.h"

MPS_ASModifier::MPS_ASModifier() {
	Val = 1;
	Type = MPS_Resource::tASM;
}
MPS_ASModifier::MPS_ASModifier(Character* c) {
	if (c->getHPCurrent() == 0) Val = 1;
	else Val = c->getHPMax() / c->getHPCurrent();
	Type = MPS_Resource::tASM;
}
MPS_ASModifier::MPS_ASModifier(std::string Path) {}

MPS_ASModifier::~MPS_ASModifier() {}

int MPS_ASModifier::getType() { return Type; }
float MPS_ASModifier::getVal() { return Val; }