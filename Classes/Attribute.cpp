#include "../Headers/Attribute.h"
	Attribute::Attribute() {
		Attribute("Attribute 1");
	}

	Attribute::Attribute(std::string n) {
		Attribute(n, 1);
	}
	Attribute::Attribute(std::string name, int stat) {
		base = stat;
		current = stat;
		name = name;
	}

	std::string Attribute::toString() {
		std::string s = name;
		s += "base: " + std::to_string(base) + " cur: " + std::to_string(current);
		return s;
	}