#ifndef _____CHARACTER_H_____
#define _____CHARACTER_H_____

#include <string>
#include <vector>
#include "Attribute.h"
#include "GLobals.h"
#include "Resistance.h"
#include "Ability.h"

/*
#include "Attributes/Strength.h"
#include "Attributes/Intelligence.h"
#include "Attributes/Dexterity.h"
#include "Attributes/Constitution.h"
#include "Attributes/Faith.h"
*/


class Character {

public:
	Character();
	Character(std::string n);
	Character(std::string n, int s, int i, int d, int c, int f);
	Character(std::string n, std::vector<Attribute> attr);
	int getHPMax();
	int getMPMax();
	int getEnergyMax();
	void takeDamage(Ability a);
	int getXPosition();
	int getYPosition();
	
protected:
	int hpMax;
	int hpCurrent;
	int energyMax;
	int energyCurrent;
	int mpMax;
	int mpCurrent;
	std::string imageIdleResource;
	std::string imageRunResource;
	std::string name;
	SDL_Texture* textureIdle;
	SDL_Texture* textureRun;
	SDL_Texture* textureActive;
	int pixelShiftAmountForAnimationInSpriteSheet;
	int currentFrame;
	int numIdleAnimationFrames;
	int numRunAnimatonFrames;
	int timeBetweenIdleAnimations; //ms
	int timeBetweenRunAnimations; //ms
	SDL_Rect rectangle;
	int speed; // pixels per second
	int speedMax; 
	int acceleration; // px/s^2
	int imageWidth;
	int imageHeight;
	int xPosition;
	int yPosition;

	void setHPMax();
	void setMPMax();
	void setEnergyMax();
	/*
	Strength strength;
	Intelligence intelligence;
	Dexterity dexterity;
	Constitution constitution;
	Faith faith;
	*/
	std::vector<Attribute> attributes;
	std::vector<Ability> abilities;

};



/*Resistance evasion;
Resistance physical;
Resistance fire;
Resistance earth;
Resistance water;
Resistance air;
Resistance light;
Resistance dark;*/

#endif