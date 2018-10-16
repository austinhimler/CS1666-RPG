#include <string>
#include <vector>
#include "../Headers/Resistance.h"

Resistance::Resistance() : Resistance("Resistance 1") {
}
Resistance::Resistance(std::string n) : Resistance(n, "Increase your " + n) {

}
Resistance::Resistance(std::string n, std::string d) : Resistance(n, d, 0.0) {
}
Resistance::Resistance(std::string n, std::string d, double b) {
	name = n;
	description = d;
	base = b;
	current = b;
}

string Resistance::to_string() {
	string s = name + ": " +description + "\n" + "base: " + std::to_string((int)(base * 100)) + "% current: " + std::to_string((int)(current * 100)) + "%";
	return s;
}