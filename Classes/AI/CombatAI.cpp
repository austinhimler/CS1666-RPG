#include "../../Headers/AI/CombatAI.h"

CombatAI::CombatAI() {}

CombatAI::CombatAI(Enemy* self, std::vector<Player*> players, std::vector<Enemy*> friends) {
	Self = self;
	Players = players;
	Friends = friends;
}

Action CombatAI::BestActionByModifiedPriorityScore() {
	BestAction = Action();
	MPS_Main MPS = MPS_Main(Self, Players, Friends);
	BestAction = MPS.getBestAction();
	return BestAction;
}

Action CombatAI::getBestAction() {
	return BestAction;
}