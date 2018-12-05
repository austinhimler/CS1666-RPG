#include "../../Headers/AI/MPS_TaskAssignment.h"
/*
void MPS_TaskAssignment::createAction(Ability* abil, std::vector<Character*> tar) {
	Act = Action(*abil, tar);
}
//*/

void MPS_TaskAssignment::createModifiers(Enemy* Self, Ability* abil, std::vector<Character*> tar, int baseEA) {
	ALMS.EAM = new MPS_EAModifier(tar, abil, baseEA);
	ALMS.RM = new MPS_RModifier(Self, abil);
}
//*/

void MPS_TaskAssignment::computeScore(float TaskPriority) {
	//std::cout << "EAM: " << ALMS.EAM->getVal() << " RM: " << ALMS.RM->getVal() << std::endl;
	Score = TaskPriority * ALMS.EAM->getVal() * ALMS.RM->getVal();
}
//*/

MPS_TaskAssignment::MPS_TaskAssignment(Enemy* Self, Ability* abil, std::vector<Character*> tar, float TaskPriority, int bea) {
	Act = Action(abil, tar);
	createModifiers(Self, abil, tar, bea);
	computeScore(TaskPriority);
	std::cout << AbilityResource::abilityNames[abil->getName()] << ": " << Score << std::endl;
}

MPS_TaskAssignment::MPS_TaskAssignment(const MPS_TaskAssignment& ta) {
	Act = ta.Act;
	ALMS = ta.ALMS;
	Score = ta.Score;
}

MPS_TaskAssignment::~MPS_TaskAssignment() {
	//delete ALMS.EAM;
	//delete ALMS.RM;
}

MPS_TaskAssignment::MPS_TaskAssignment() {
	Act = Action();
	Score = -1;
}

Action MPS_TaskAssignment::getAction() {
	return Act;
}

float MPS_TaskAssignment::getScore() {
	return Score;
}
