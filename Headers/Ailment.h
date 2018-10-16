#pragma once
#include <string>

enum AILMENTS {
	SILENCE,
	POISON,
};

class Ailment {
public:
	Ailment(int n, double c);
protected:
	int name;
	double chance;
};