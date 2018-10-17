#include "../Headers/Attribute.h"

Attribute::Attribute(std::string n, int stat) {
	base = stat;
	current = stat;
	name = n;
}

Attribute::Attribute(std::string n) : Attribute(n, 1) {}

Attribute::Attribute() : Attribute("Attribute 1"){}

int Attribute::getCur() {
	return current;
}

	std::string Attribute::toString() {
		std::string s = name;
		s += " base: " + std::to_string(base) + " cur: " + std::to_string(current);
		return s;
	}