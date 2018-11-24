#pragma once
#include <vector>
#include "../Ability.h"
#include "../Character.h"

class Action {
protected:
	Ability* Abil;
	std::vector<Character*> Target;
	bool IsNULL;
public:
	/** Constructors **/
	Action(Ability* a, std::vector<Character*> tar);
	Action(const Action& act);
	Action();
	
	/** getters **/
	Ability* getAbil();
	std::vector<Character*> getTar();
	bool isNULL();


};