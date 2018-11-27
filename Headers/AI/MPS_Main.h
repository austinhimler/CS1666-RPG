#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "CombatAIResource.h"
#include "MPS_Data.h"
#include "MPS_Task.h"
#include "MPS_Modifier.h"
#include "MPS_ASModifier.h"
#include "MPS_TaskAssignment.h"
#include "../Ability.h"
#include "Action.h"
#include "../Enemy.h"
#include "../Character.h"
#include "../Player.h"

class MPS_Main {
protected:
	std::vector<MPS_Task> Tasks;
	std::vector<Ability*> Abilities;
	Action BestAction;
	std::vector<MPS_Modifier*> TaskLevelModifiers;
	static std::vector<int> TaskBasePriority;
public:
	void createAbilities(Enemy* Self);
	void readTBP();
	void createTLMs(Enemy* Self, std::vector<Player*> Players, std::vector<Enemy*> Friends);
	void createTasks(Enemy* Self, std::vector<Player*> Players, std::vector<Enemy*> Friends);
	void findBestAction();

	/** constructors & desstructor **/
	MPS_Main(Enemy* Self, std::vector<Player*> Players, std::vector<Enemy*> Friends);
	~MPS_Main();

	/** getters **/
	Action getBestAction();

};