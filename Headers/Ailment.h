#pragma once
#include <string>

class Ailment {
public:
	Ailment(int n, double c);
protected:
	int name;
	double chance;
};
