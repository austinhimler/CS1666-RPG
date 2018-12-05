#pragma once
#include <vector>
#include "Action.h"
#include "MPS_Modifier.h"
#include "../Character.h"
#include "../Ability.h"
#include "MPS_EAModifier.h"
#include "MPS_RModifier.h"

class MPS_TaskAssignment {
protected:
	Action Act;
	struct AssignmentLevelModifiers {
		MPS_EAModifier* EAM;
		MPS_RModifier* RM;
	} ALMS;

	float Score;

public:
	//void createAction(Ability* abil, std::vector<Character*> tar);
	void createModifiers(Enemy* Self, Ability* abil, std::vector<Character*> tar, int bea);
	void computeScore(float TaskPriority);

	/** Constructors and destructor **/
	MPS_TaskAssignment(Enemy* Self, Ability* abil, std::vector<Character*> tar, float TaskPriority, int bea);
	MPS_TaskAssignment(const MPS_TaskAssignment& ta);
	MPS_TaskAssignment();
	~MPS_TaskAssignment();

	/** getters **/
	Action getAction();
	float getScore();
};