#include "../../Headers/AI/Action.h"

Action::Action(Ability* abil, std::vector<Character*> tar) {
	Abil = abil;
	Target = tar;
	IsNULL = false;
}

Action::Action(const Action& act) {
	Abil = act.Abil;
	Target = act.Target;
	IsNULL = act.IsNULL;
}
Action::Action() {
	Abil = nullptr;
	Target = std::vector<Character*>();
	IsNULL = true;
}

Ability* Action::getAbil() {
	return Abil;
}

std::vector<Character*> Action::getTar() {
	return Target;
}

bool Action::isNULL() {
	return IsNULL;
}