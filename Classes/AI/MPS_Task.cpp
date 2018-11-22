#include "../../Headers/AI/MPS_Task.h"

void MPS_Task::createAssignments(std::vector<Ability*> ua) {
	if (Assignments.size() != 0) return;
	for (auto& a : ua)
	{
		if (a->isAOE) {
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
	Priority = TaskBasePriority;
	for (auto& tlm : TLM) {
		Priority *= tlm->getVal();
	}
}

void MPS_Task::findBestAssignment() {
	if (Assignments.size() == 0) return;
	BestAssignment = Assignments[0];
	for (auto& a : Assignments) {
		if (a.getScore() > BestAssignment.getScore()) {
			BestAssignment = a;
		}
	}
}

MPS_Task::MPS_Task(int T, std::vector<Ability*> ua, std::vector<Character*> tar, int TaskBasePriority, std::vector<MPS_Modifier*> TLM) {
	Type = T;
	Target = std::vector<Character*>(tar);
	createAssignments(ua);
	computePriority(TaskBasePriority, TLM);
	findBestAssignment();
}