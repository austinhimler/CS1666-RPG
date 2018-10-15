#include "../Headers/Player.h"
#include "../Headers/Globals.h"
#include "../Headers/Helper.h"
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


	    pixelShiftAmountForAnimationInSpriteSheet = 200;
		numIdleAnimationFrames = 4;
		numRunAnimatonFrames = 6;
		currentFrame = 0;
		timeBetweenIdleAnimations = 120;
		timeBetweenRunAnimations = 100;
		imageIdleResource = "Images/Player/Character_Idle.png";
		imageRunResource = "Images/Player/Character_Run.png";
		Helper helper = Helper();
		textureIdle = helper.loadImage(imageIdleResource.c_str(), gRenderer);
		textureRun = helper.loadImage(imageRunResource.c_str(), gRenderer);
		textureActive = NULL;
		imageWidth = 200;
		imageHeight = 148;
		xPosition = 250; //will need to edit for starting level
		yPosition = 250; //will need to edit for starting level
		rectangle = {xPosition, yPosition, imageWidth, imageHeight};
		speedMax = 250;
		acceleration = 200;
	}


	Player::operator std::string() {
		std::string s = "Name: " +	name + "\n";
		for (auto i : attributes) {
			s += i.toString() + "\n";
		}
		return s;
	}
