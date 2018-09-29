#include <string>
#include <vector>
class Attribute {
public:
	int mainStat;
	int base;
	int current;
	std::string name;
	std::vector<Ability> abilities; //abilities this attribute has, might not need this
	Attribute() {

	}

	Attribute(int stat, std::string name) {
		base = stat;
		current = stat;
		this->name = name;
	}
};