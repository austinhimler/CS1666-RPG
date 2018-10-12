#ifndef _____RESISTANCE_H_____
#define _____RESISTANCE_H_____

#include <string>
#include <vector>

using namespace std;

class Resistance {
public:
	Resistance();
	Resistance(std::string n);
	Resistance(std::string n, std::string d);
	Resistance(std::string n, std::string d, double b);
	string toString();
private:
	std::string name;
	std::string description;
	double base;
	double current;
	

};

#endif