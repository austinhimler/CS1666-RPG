#include "../../Headers/AI/MPS_TaskAssignment.h"
/*
void MPS_TaskAssignment::createAction(Ability* abil, std::vector<Character*> tar) {
	Act = Action(*abil, tar);
}
//*/
/*
void MPS_TaskAssignment::createModifiers() {
	AssignmentLevelModifiers = std::vector<MPS_Modifier*>();
}
//*/
/*
void MPS_TaskAssignment::computeScore(float TaskPriority) {
	 Score = TaskPriority;
}
//*/

MPS_TaskAssignment::MPS_TaskAssignment(Ability* abil, std::vector<Character*> tar, float TaskPriority) {
	Act = Action(*abil, tar);
	AssignmentLevelModifiers = std::vector<MPS_Modifier*>();
	Score = TaskPriority;
}

MPS_TaskAssignment::MPS_TaskAssignment(const MPS_TaskAssignment& ta) {
	Act = ta.Act;
	AssignmentLevelModifiers = ta.AssignmentLevelModifiers;
	Score = ta.Score;
}

MPS_TaskAssignment::MPS_TaskAssignment() {
}

Action MPS_TaskAssignment::getAction() {
	return Act;
}

float MPS_TaskAssignment::getScore() {
	return Score;
}
