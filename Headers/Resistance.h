#pragma once

#include <string>
#include <vector>

using namespace std;

class Resistance {
public:
	Resistance();
	Resistance(std::string n);
	Resistance(std::string n, std::string d);
	Resistance(std::string n, std::string d, double b);
	string to_string();

private:
	std::string name;
	std::string description;
	double base;
	double current;
};