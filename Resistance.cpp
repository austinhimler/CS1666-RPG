#include "Headers/Resistance.h"

Resistance::Resistance() {
	this->name = "resistance name";
	this->type = 0;
	this->resistanceValue = (double) 0.0;
}
Resistance::Resistance(std::string name, int type, double resistanceValue) {
	this->name = name;
	if (type < 0 || type > 3) type = 0;
	this->type = type;
	if (resistanceValue < 0.0 || resistanceValue > 1.0) resistanceValue = 0.0;
	this->resistanceValue = resistanceValue;
}