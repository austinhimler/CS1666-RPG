#include <string>
#include <vector>
class Attribute {
public:
	int mainStat;
	int base;
	int current;
	std::string name;
	Attribute() {

	}

	Attribute(int stat, std::string name) {
		base = stat;
		current = stat;
		this->name = name;
	}
};