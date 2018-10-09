#include "../Headers/Player.h"
#include <iostream>
	/*
	int strength = 1;
	int intelligence = 1;
	int dexterity = 1;
	int constitution = 1;
	int faith = 1;
	int currentExprience;
	Inventory inventory
	std::string name;
	*/
	//stores stats
	Player::Player() : Character("Player 1") {}
	Player::Player(std::string n) : Character(n) {}
	Player::Player(std::string n, int s, int i, int d, int c, int f) : Character(n, s,i,d,c,f){}
	Player::Player(std::string n, std::vector<Attribute> attr) : Character(n, attr) {}
	void Player::setAll(std::string n, int s, int i, int d, int c, int f) {
		attributes[STR] = Attribute("Strength", s);
		attributes[INT] = Attribute("Intelligence", i);
		attributes[DEX] = Attribute("Dexerity", d);
		attributes[CON] = Attribute("Constitution", c);
		attributes[FAI] = Attribute("Faith", f);
		name = n;
	}
	Player::operator std::string() {
		std::string s = "Name: " +	name + "\n";
		for (auto i : attributes) {
			s += i.toString() + "\n";
		}
		return s;
	}
