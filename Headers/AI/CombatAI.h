#pragma once
#include <vector>
#include <string>
#include "Action.h"
#include "CombatAIResource.h"
#include "MPS_Main.h"
#include "../Enemy.h"
#include "../Player.h"

class CombatAI {
protected:
	Action BestAction;
	Enemy* Self;
	std::vector<Player*> Players;
	std::vector<Enemy*> Friends;
public:
	Action BestActionByModifiedPriorityScore();

	/** constructors **/
	CombatAI();
	CombatAI(Enemy* self, std::vector<Player*> players, std::vector<Enemy*> friends);

	/** getters **/
	Action getBestAction();
};