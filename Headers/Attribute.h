#pragma once
#include "Ability.h"
#include <string>
#include <vector>
class Attribute {
	int base;
	int current;
	std::string name;
	std::vector<Ability> abilities;
	Attribute();
	Attribute(int stat, std::string name);
};