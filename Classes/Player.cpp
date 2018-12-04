#include "../Headers/Player.h"
#include "../Headers/Globals.h"
#include "../Headers/Helper.h"
#include <iostream>
#include <sstream>
#include <cstring>
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
	Player::Player(std::string n, int s, int i, int d, int c, int f) : Character(n, s, i, d, c, f) {
		learnAbility(FIREBALL);
		learnAbility(SUMMON);
		isEnemy = false;
		level = 10;
		pixelShiftAmountForAnimationInSpriteSheet = 144;
		numIdleAnimationFrames = 6;
		numRunAnimatonFrames = 6;
		currentFrame = 0;
		timeBetweenIdleAnimations = 120;
		timeBetweenRunAnimations = 100;
		if (n.compare("nfl4") == 0)
			imageIdleResource = "Images/Player/Idle_Down_Special.png";
		else imageIdleResource = "Images/Player/Idle_Down.png";
		imageRightIdleResource = "Images/Player/Idle_Right.png";
		imageRunResource = "Images/Player/Run_Right.png";
		imageDownRunResource = "Images/Player/Run_Down.png";
		imageDownRightRunResource = "Images/Player/Run_DownRight.png";
		imageUpRightRunResource = "Images/Player/Run_UpRight.png";
		imageUpRunResource = "Images/Player/Run_Up.png";
		Helper helper = Helper();
		textureIdle = helper.loadImage(imageIdleResource.c_str(), gRenderer);
		textureRightIdle = helper.loadImage(imageRightIdleResource.c_str(), gRenderer);
		textureRun = helper.loadImage(imageRunResource.c_str(), gRenderer);
		textureDownRun = helper.loadImage(imageDownRunResource.c_str(), gRenderer);
		textureDownRightRun = helper.loadImage(imageDownRightRunResource.c_str(), gRenderer);
		textureUpRightRun = helper.loadImage(imageUpRightRunResource.c_str(), gRenderer);
		textureUpRun = helper.loadImage(imageUpRunResource.c_str(), gRenderer);
		textureActive = NULL;
		imageWidth = 144;
		imageHeight = 144;
		xPosition = 0; //will need to edit for starting level
		yPosition = 250.0; //will need to edit for starting level
		rectangle = { (int)xPosition, (int)yPosition, imageWidth, imageHeight };
		drawRectangle = { 0, 0, imageWidth, imageHeight };
		speedMax = 150.0;
		acceleration = 2000.0;
	}
	Player::Player(std::string n, std::vector<Attribute> attr) : Character(n, attr) {
		learnAbility(FIREBALL);
		learnAbility(SUMMON);
		isEnemy = false;
	}
	void Player::setAll(std::string n, int s, int i, int d, int c, int f) {
		attributes[STR] = Attribute("Strength", s);
		attributes[INT] = Attribute("Intelligence", i);
		attributes[DEX] = Attribute("Dexerity", d);
		attributes[CON] = Attribute("Constitution", c);
		attributes[FAI] = Attribute("Faith", f);
		name = n;
		isEnemy = false;

	    pixelShiftAmountForAnimationInSpriteSheet = 144;
		numIdleAnimationFrames = 6;
		numRunAnimatonFrames = 6;
		currentFrame = 0;
		timeBetweenIdleAnimations = 120;
		timeBetweenRunAnimations = 100;
		imageIdleResource = "Images/Player/Idle_Down.png";
		imageRunResource = "Images/Player/Run_Right";
		Helper helper = Helper();
		textureIdle = helper.loadImage(imageIdleResource.c_str(), gRenderer);
		textureRun = helper.loadImage(imageRunResource.c_str(), gRenderer);
		textureActive = NULL;
		imageWidth = 144;
		imageHeight = 144;
		xPosition = 0; //will need to edit for starting level
		yPosition = 250.0; //will need to edit for starting level
		rectangle = {(int)xPosition, (int)yPosition, imageWidth, imageHeight};
		drawRectangle = { 0, 0, imageWidth, imageHeight };
		speedMax = 150.0;
		acceleration = 2000.0;
	}
	int Player::getLevel() { return level; }
	int Player::getCurrentExperience() { return currentExperience; }

	Player::operator std::string() {
		
		std::string s = "Name: " +	name + "\n";
		for (auto i : attributes) {
			s += i.toString() + "\n";
		}
		return s;
		
	}
	//name currentFrame xPosition yPosition xVelocity yVelocity
	std::string Player::ptoString()
	{
		//previous attempt, new attempt uses a stream
		//std::string pString = name + " " + std::to_string(currentFrame) + " " + std::to_string(xPosition) + " " +
		//	std::to_string(yPosition) + " " + std::to_string(xVelocity) + " " + std::to_string(yVelocity);
		std::stringstream st;
		st << name << " " << currentFrame << " " << xPosition << " " << yPosition << " " << xVelocity << " " << yVelocity <<" *"<< std::ends;
		std::cout << st.str() << std::endl;
		return st.str();
	}
	void Player::fromString(std::string in)
	{
		std::vector<std::string> vars;
		size_t pos = 0;
		std::string token;
		while ((pos = in.find(" ")) != std::string::npos) {
			token = in.substr(0, pos);
			std::cout << "Parsed: " << token << endl;
			vars.push_back(token);
			in.erase(0, pos + 1);
		}
		if (vars.size > 5)
		{
			name = vars[0];
			currentFrame = std::stoi(vars[1]);
			xPosition = atof(vars[2].c_str());
			yPosition = atof(vars[3].c_str());
			xVelocity = atof(vars[4].c_str());
			yVelocity = atof(vars[5].c_str());
		}
		return;
	}