#pragma once
#include <vector>
#include "../Ability.h"
#include "../Character.h"
#include "MPS_TaskAssignment.h"
#include "MPS_Modifier.h"
#include "MPS_Data.h"

class MPS_Task {
protected:
		int Type;
		float Priority;
		float BestScore;
		std::vector<Character*> Target;
		std::vector<MPS_TaskAssignment> Assignments;
		MPS_TaskAssignment BestAssignment;
public:
	void createAssignments(std::vector<Ability*> UsableAbilities);	//if don't have any assignment, crerate assignments
	void computePriority(int TaskBasePriority, std::vector<MPS_Modifier*> TLM);
	void findBestAssignment();

	/** Constructors **/
	MPS_Task(int T, std::vector<Ability*> UsableAbilities, std::vector<Character*> tar, int TaskBasePriority, std::vector<MPS_Modifier*> TaskLevelModifiers);

	/** getters **/
	int getType();
	float getPriority();
	float getBestScore();
	std::vector<Character*> getTargets();
	std::vector<MPS_TaskAssignment> getAssignments();
	MPS_TaskAssignment getBestAssignment();
};