#include "../../Headers/AI/MPS_ASModifier.h"

MPS_ASModifier::MPS_ASModifier() {
	Val = 1;
	Type = MPS_Resource::tASM;
}
MPS_ASModifier::MPS_ASModifier(Character* c) : MPS_ASModifier() {
	
}
MPS_ASModifier::MPS_ASModifier(std::string Path) {

}

int MPS_ASModifier::getType() { return Type; }
float MPS_ASModifier::getVal() { return Val; }