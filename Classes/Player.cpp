#include "../Headers/Player.h"
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
	Player::Player() {}
	Player::Player(int s, int i, int d, int c, int f, std::string n)
	{
		strength = s;
		intelligence = i;
		dexterity = d;
		constitution = c;
		faith = f;
		name = n;
	}
	void Player::setAll(int s, int i, int d, int c, int f, std::string n)
	{
		strength = s;
		intelligence = i;
		dexterity = d;
		constitution = c;
		faith = f;
		name = n;
		return;
	}
	Player::operator std::string()
	{
		std::string strengthString = std::to_string(strength);
		std::string intelligenceString = std::to_string(intelligence);
		std::string dexterityString = std::to_string(dexterity);
		std::string constitutionString = std::to_string(constitution);
		std::string faithString = std::to_string(faith);
		std::string s = "Name: " + name + " Strength: " + strengthString + " Intelligence: " + intelligenceString + " Dexterity: " + dexterityString + " Constitution: " + constitutionString + " Faith: " + faithString; ;
		return s;
	}
