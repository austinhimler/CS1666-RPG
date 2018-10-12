#include <string>
#include <vector>
#include "../Headers/Resistance.h"

Resistance::Resistance() {
	Resistance("Resistance 1");
}
Resistance::Resistance(std::string n) {
	Resistance(n, "");
}
Resistance::Resistance(std::string n, std::string d) {
	Resistance(n, d, 0);
}
Resistance::Resistance(std::string n, std::string d, double b) {
	name = n;
	description = d;
	base = b;
	current = b;
}

string Resistance::toString() {
	//string s = name + ": " + description + "\n" + "base: " + base + " current: " + b;

}