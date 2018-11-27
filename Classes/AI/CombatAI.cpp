#include "../../Headers/AI/CombatAI.h"

CombatAI::CombatAI() {}

CombatAI::CombatAI(Enemy* self, std::vector<Player*> players, std::vector<Enemy*> friends) {
	Self = self;
	Players = players;
	Friends = friends;
	BestAction = Action();
}

void CombatAI::BestActionByModifiedPriorityScore() {

	MPS_Main MPS = MPS_Main(Self, Players, Friends);
	BestAction = MPS.getBestAction();
}

Action CombatAI::getBestAction() {
	if (BestAction.isNULL()) {
		BestActionByModifiedPriorityScore();
	}
	return BestAction;
}