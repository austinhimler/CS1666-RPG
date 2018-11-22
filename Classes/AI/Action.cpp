#include "../../Headers/AI/Action.h"

Action::Action(Ability abil, std::vector<Character*> tar) {
	Abil = abil;
	Target = tar;
}

Action::Action(const Action& act) {
	Abil = act.Abil;
	Target = act.Target;
}
Action::Action() {
	Abil = Ability();
	Target = std::vector<Character*>();
}

Ability Action::getAbil() {
	return Abil;
}

std::vector<Character*> Action::getTar() {
	return Target;
}