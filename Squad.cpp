#include "Squad.h"

Squad::Squad(std::string name) {
	this->name = name;
	size = 0;
}
bool Squad::addPlayer(Player p) {
	if (size == 4) return false;
	else {
		this->squad[size] = p;
		size++;
		return true;
	}
}
bool Squad::isEmpty() { return size == 0; }
bool Squad::isFull() { return size == 4; }
