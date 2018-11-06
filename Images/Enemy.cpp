#include "../Headers/Enemy.h"
#include "../Headers/Globals.h"
#include "../Headers/Helper.h"
#include <iostream>

Enemy::Enemy() : Character("Enemy NPC") { }
Enemy::Enemy(std::string n) : Character(n) {}
Enemy::Enemy(std::string n, int s, int i, int d, int c, int f) {
	isEnemy = true;
	pixelShiftAmountForAnimationInSpriteSheet = 128;
	numIdleAnimationFrames = 4;
	numRunAnimatonFrames = 7;
	currentFrame = 0;
	timeBetweenIdleAnimations = 120;
	timeBetweenRunAnimations = 100;
	imageIdleResource = "Images/Enemies/shadow_cluster/owl.png";
	imageRunResource = "Images/Enemies/shadow_cluster/owl.png";
	Helper helper = Helper();
	textureIdle = helper.loadImage(imageIdleResource.c_str(), gRenderer);
	textureRun = helper.loadImage(imageRunResource.c_str(), gRenderer);
	textureActive = NULL;
	imageWidth = 128;
	imageHeight = 128;
	xPosition = 0; //will need to edit for starting level
	yPosition = 250.0; //will need to edit for starting level
	rectangle = { (int)xPosition, (int)yPosition, imageWidth, imageHeight };
	drawRectangle = { 0, 0, imageWidth, imageHeight };
	speedMax = 150.0;
	acceleration = 2000.0;
}
Enemy::Enemy(std::string n, std::vector<Attribute> attr) : Character(n, attr) {
	isEnemy = true;
}

Enemy::Enemy(std::string n, std::string description, double range, int x0, int y0, int ai) {
	this->name = name;
	this->description = description;
	this->maxRange = range;
	this->xSpawn = x0;
	this->ySpawn = y0;
	this->ai = ai;

	attributes[STR] = Attribute("Strength", 5);
	attributes[INT] = Attribute("Intelligence", 5);
	attributes[DEX] = Attribute("Dexerity", 5);
	attributes[CON] = Attribute("Constitution", 5);
	attributes[FAI] = Attribute("Faith", 5);

	isEnemy = true;
	pixelShiftAmountForAnimationInSpriteSheet = 128;
	numIdleAnimationFrames = 4;
	numRunAnimatonFrames = 7;
	currentFrame = 0;
	timeBetweenIdleAnimations = 120;
	timeBetweenRunAnimations = 100;
	imageIdleResource = "Images/Enemies/shadow_cluster/owl.png";
	imageRunResource = "Images/Enemies/shadow_cluster/owl.png";
	Helper helper = Helper();
	textureIdle = helper.loadImage(imageIdleResource.c_str(), gRenderer);
	textureRun = helper.loadImage(imageRunResource.c_str(), gRenderer);
	textureActive = NULL;
	imageWidth = 128;
	imageHeight = 128;
	xPosition = 0; //will need to edit for starting level
	yPosition = 250.0; //will need to edit for starting level
	rectangle = { (int)xPosition, (int)yPosition, imageWidth, imageHeight };
	drawRectangle = { 0, 0, imageWidth, imageHeight };
	speedMax = 150.0;
	acceleration = 2000.0;
}

void Enemy::makeDecision(int x, int y) {
	MovementDecision dec = MovementDecision((int)this->xPosition, (int)this->yPosition, x, y, this->xSpawn, this->ySpawn);
	dec.generateDecisions();
	if (this->ai == 0) dec.makeAgroDecison(this->maxRange);
	else if (this->ai == 1) dec.makePassiveDecison(this->maxRange);

	if (dec.poolSpawn()) {
		this->xSpawn = dec.getNewSpawn().getX();
		this->ySpawn = dec.getNewSpawn().getY();
	}

	this->xPosition = (double) dec.getMove().getX();
	this->yPosition = (double )dec.getMove().getY();
}

Enemy::operator std::string() {
	std::string s = "Name: " + name + "\n";
	for (auto i : attributes) {
		s += i.toString() + "\n";
	}
	return s;
}


class MovementDecision {
	class Location {
	private:
		int x0, y0;
	public:
		Location() {
			x0 = NULL;
			y0 = NULL;
		}
		Location(int a, int b) {
			x0 = a;
			y0 = b;
		}
		int getX() { return x0; }
		int getY() { return y0; }
		double calcDistance(Location l) { return ((x0 - l.getX()) ^ 2) + (y0 - l.getY() ^ 2); }
	};
private:
	Location ePosition, pPosition, decision; //enemy position, player position, decision preference
	std::vector<Location> decisions; //list of decision options
	Location spawn, nSpawn;
	bool flag = 0;

public:
	MovementDecision(int x0, int y0, int x1, int y1, int sx, int sy) {
		ePosition = Location(x0, y0);
		pPosition = Location(x1, y1);
		spawn = Location(sx, sy);
		nSpawn = Location();
	}

	Location getMove() { return decision; }

bool newSpawnPoint() { return flag; }
Location getNewSpawn() { return nSpawn; }

void generateDecisions() {
	//add current position
	decisions.push_back(Location(ePosition.getX(), ePosition.getY()));
	//add all adjacent positions
	int moveX, moveY;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			moveX = ePosition.getX() + i;
			moveY = ePosition.getY() + j;
			if (moveX < 0 && moveX > SCREEN_WIDTH - 1) moveX = ePosition.getX();
			if (moveY < 0 && moveY > SCREEN_HEIGHT - 1) moveY = ePosition.getY();
			decisions.push_back(Location(moveX, moveY));
		}
	}
}
/*
compare distance (b/t npc and player) that would result from different
choices; looking for choice with shortest distance (agro)

set decison to parsed choice only if
1. subsequently distance (b/t npc and player) is less than range
2. distance is less than npc's current distance to player
3. distance is less than previously calculated distances
*/
void makeAgroDecison(double range) {
	//initialize to current distance between player and npc
	double tempDistance = ePosition.calcDistance(pPosition);
	/*
	if current distance greater than range...
	npc will head toward spawn point
	spawn does not change
	*/
	if (tempDistance > range) {
		double tempDistance = ePosition.calcDistance(spawn);
		/*
		parse all possible decisons to find move to minimize distance
		b/t npc and spawn
		*/
		for (auto i : decisions) {
			if (((i.getX() - spawn.getX()) ^ 2) + ((i.getY() - spawn.getY()) ^ 2) <= tempDistance) {
				tempDistance = ((i.getX() - pPosition.getX()) ^ 2) + ((i.getY() - pPosition.getY()) ^ 2);
				decision = Location(i.getX(), i.getY());
			}
		}
	}
	/*
	o/w parse all possible decisons to find move to minimize distance
	b/t player and npc
	*/
	else {
		for (auto i : decisions) {
			if (((i.getX() - pPosition.getX()) ^ 2) + ((i.getY() - pPosition.getY()) ^ 2) <= tempDistance) {
				tempDistance = ((i.getX() - pPosition.getX()) ^ 2) + ((i.getY() - pPosition.getY()) ^ 2);
				decision = Location(i.getX(), i.getY());
			}
		}
	}
}
/*
compare distance (b/t npc and player) that would result from
various possible decision-choices

look for choice with largest distance (passive)

if current distance is greater than range, set new spawn
current location

o/w set current preference to (parsed) decision-choice only if
1. subsequently distance (b/t npc and player) is less than range
2. distance is greater than than npc's current distance to player
3. distance is greater than previously calculated distances

result will be final decision
*/
void makePassiveDecison(double range) {
	//initialize to current distance between player and npc
	double tempDistance = ePosition.calcDistance(pPosition);
	/*
	if current distance is greater than range...
	set current position as new spawn for npc,
	then set movement decision as current location
	*/
	if (tempDistance > range) {
		flag = true;
		nSpawn = Location(ePosition.getX(), ePosition.getY());
		decision = Location(ePosition.getX(), ePosition.getY());
	}
	/*
	o/w parse all possible decisons to find move to maximize distance
	b/t player and npc
	*/
	else {
		for (auto i : decisions) {
			//parse for greatest distance
			if (((i.getX() - pPosition.getX()) ^ 2) + ((i.getY() - pPosition.getY()) ^ 2) >= tempDistance) {
				tempDistance = ((i.getX() - pPosition.getX()) ^ 2) + ((i.getY() - pPosition.getY()) ^ 2);
				decision = Location(i.getX(), i.getY());
			}
		}
	}
}
bool poolSpawn() { return flag; }
Location getNewSpawn() { return nSpawn; }
};