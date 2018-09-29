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

	Attribute(int stat) {
		base = stat;
		current = stat;
	}
};