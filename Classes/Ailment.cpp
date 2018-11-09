#include "../Headers/Ailment.h"

Ailment::Ailment(int n, double c, int v, int tn, int sa, int d) {
	name = n;
	chance = c;
	value = v;
	turnN = tn;
	stopAction = sa;
	degree = d;
	type = AilmentResource::ailmentType[name];
}

Ailment::Ailment(int n, int f1, int f2, int val) {
	name = n;
	degree = std::rand() % 100;
	if (degree < 30) degree = 1;
	else if (degree < 55) degree = 2;
	else if (degree < 75) degree = 3;
	else if (degree < 90) degree = 4;
	else if (degree < 100) degree = 5;
	chance = (f1 - f2) * AilmentResource::ailmentCompen[name];
	if (chance <= 10) chance = BASE_CHANCE;
	value = val / ABIL_FACTOR + degree * DEGREE_FACTOR;
	turnN = (int)(chance / 3);
	stopAction = AilmentResource::ailmentSA[n];
	type = AilmentResource::ailmentType[name];
}

Ailment::Ailment(int n, double c) {
	name = n;
	chance = c;
}

Ailment::Ailment() {}

Ailment::operator std::string() {
	std::string s = "Name: " + AilmentResource::ailmentNames[name] + "\n" + AilmentResource::ailmentDescrip(name) + "\n" + "Chance: " + to_string(chance) + "\n" + "Degree: " + to_string(degree);
	if (value < 0)
		s.append("Heal Per Turn: ");
	else
		s.append("Damage Per Turn: ");
	s.append( to_string(value) + "   " + "Lasting: " + to_string(turnN) + "\n" + "Stop target for " + to_string(stopAction) + " turn\n");
	return s;
}

bool Ailment::operator==(const Ailment& ailm) {
	if (ailm.name == name) return true;
	else return false;
}

bool Ailment::operator!=(const Ailment& ailm) {
	if (ailm.name != name) return true;
	else return false;
}

Ailment Ailment::operator=(const Ailment& ailm) {
	name = ailm.name;
	value = ailm.value;
	turnN = ailm.turnN;
	degree = ailm.degree;
	stopAction = ailm.stopAction;
	chance = ailm.chance;
	return *this;
}

string Ailment::getName() {	return AilmentResource::ailmentNames[name];}
int Ailment::getVal() {	return value;}
int Ailment::getChance() {  return chance;}
int Ailment::getTN() {	return turnN;}
int Ailment::getSA() { return stopAction; }
int Ailment::getDegree() { return degree; }
std::vector<Resistance> Ailment::getREs() { return resistances; }
int Ailment::getType() { return type; }

bool Ailment::combineAilment(Ailment& ailm) {
	if (*this != ailm) return false;
	value = value - degree * DEGREE_FACTOR + ailm.value - ailm.degree * DEGREE_FACTOR;
	if (degree < ailm.degree)
		degree = ailm.degree;
	value += degree * DEGREE_FACTOR;
	if (turnN < ailm.turnN)
		turnN = ailm.turnN;
	if (stopAction < ailm.stopAction)
		stopAction = ailm.stopAction;
	return true;
}

void Ailment::update() {
	if (stopAction > 0) stopAction--;
	if (turnN > 0) turnN--;
}