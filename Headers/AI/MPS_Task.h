#pragma once
#include <vector>
#include "../Ability.h"
#include "../Character.h"
#include "MPS_TaskAssignment.h"
#include "MPS_Modifier.h"
#include "MPS_Data.h"
#include "../Enemy.h"

class MPS_Task {
protected:
		int Type;
		float Priority;
		float BestScore;
		Enemy* Self;
		std::vector<Character*> Target;
		std::vector<MPS_TaskAssignment> Assignments;
		MPS_TaskAssignment BestAssignment;
public:
	void createAssignments(std::vector<Ability*> UsableAbilities, std::vector<int> BaseEffectiveness);	//if don't have any assignment, create assignments
	void computePriority(int TaskBasePriority, std::vector<MPS_Modifier*> TLM);
	void findBestAssignment();

	/** Constructors **/
	MPS_Task(Enemy* self, int T, std::vector<Ability*> UsableAbilities, std::vector<Character*> tar, int TaskBasePriority, std::vector<MPS_Modifier*> TaskLevelModifiers, std::vector<int> BaseEffectiveness);

	/** getters **/
	int getType();
	float getPriority();
	float getBestScore();
	std::vector<Character*> getTargets();
	std::vector<MPS_TaskAssignment> getAssignments();
	MPS_TaskAssignment getBestAssignment();
};