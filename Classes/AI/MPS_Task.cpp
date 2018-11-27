#include "../../Headers/AI/MPS_Task.h"

void MPS_Task::createAssignments(std::vector<Ability*> ua) {
	for (auto& a : ua)
	{
		if (a->isAOE()) {
			/** Inapprioriately Handling AOE abilites **/
			Assignments.push_back(MPS_TaskAssignment(a, Target, Priority));
		}
		else {
			for (auto& c : Target)
			{
				std::vector<Character*> temp;
				temp.push_back(c);
				Assignments.push_back(MPS_TaskAssignment(a, temp, Priority));
			}
		}
	}
}

void MPS_Task::computePriority(int TaskBasePriority, std::vector<MPS_Modifier*> TLM) {
	Priority = (float)TaskBasePriority;
	for (auto& tlm : TLM) {
		Priority *= tlm->getVal();
	}
}

void MPS_Task::findBestAssignment() {
	BestAssignment = MPS_TaskAssignment();
	if (Assignments.size() == 0) return;
	BestAssignment = Assignments[0];
	for (auto& a : Assignments) {
		if (a.getScore() > BestAssignment.getScore()) {
			BestAssignment = a;
		}
		else if (a.getScore() == BestAssignment.getScore()) {
			int tmp = rand() % 100;
			if (tmp > 49) BestAssignment = a;
		}
	}
	BestScore = BestAssignment.getScore();
}

MPS_Task::MPS_Task(int T, std::vector<Ability*> ua, std::vector<Character*> tar, int TaskBasePriority, std::vector<MPS_Modifier*> TLM) {
	Type = T;
	Target = std::vector<Character*>(tar);
	createAssignments(ua);
	computePriority(TaskBasePriority, TLM);
	findBestAssignment();
}

int MPS_Task::getType() {
	return Type;
}
float MPS_Task::getPriority() {
	return Priority;
}
float MPS_Task::getBestScore() {
	return BestScore;
}
std::vector<Character*> MPS_Task::getTargets() {
	return Target;
}
std::vector<MPS_TaskAssignment> MPS_Task::getAssignments() {
	return Assignments;
}
MPS_TaskAssignment MPS_Task::getBestAssignment() {
	return BestAssignment;
}