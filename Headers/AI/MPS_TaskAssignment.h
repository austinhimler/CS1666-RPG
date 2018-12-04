#pragma once
#include <vector>
#include "Action.h"
#include "MPS_Modifier.h"
#include "../Character.h"
#include "../Ability.h"

class MPS_TaskAssignment {
protected:
	Action Act;
	std::vector<MPS_Modifier*> AssignmentLevelModifiers;
	float Score;

public:
	//void createAction(Ability* abil, std::vector<Character*> tar);
	//void createModifiers();
	//void computeScore(float TaskPriority);

	/** Constructors **/
	MPS_TaskAssignment(Ability* abil, std::vector<Character*> tar, float TaskPriority);
	MPS_TaskAssignment(const MPS_TaskAssignment& ta);
	MPS_TaskAssignment();

	/** getters **/
	Action getAction();
	float getScore();
};