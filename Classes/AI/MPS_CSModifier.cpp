#include "../../Headers/AI/MPS_CSModifier.h"

MPS_CSModifier::MPS_CSModifier() {
	Val = 1;
	Type = MPS_Resource::tASM;
}
MPS_CSModifier::MPS_CSModifier(Character* c, int t) {
	if (c->getHPCurrent() == 0) Val = 0;
	else Val = (float)(c->getHPMax() / c->getHPCurrent());
	Type = t;
}
MPS_CSModifier::MPS_CSModifier(std::string Path) {}

MPS_CSModifier::~MPS_CSModifier() {}

int MPS_CSModifier::getType() { return Type; }
float MPS_CSModifier::getVal() { return Val; }