#ifndef _____CHARACTER_H_____
#define _____CHARACTER_H_____

#include <string>
#include <vector>
#include "Attribute.h"
#include "Globals.h"
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
	int getHPCurrent();
	int getMPCurrent();
	int getMPMax();
	int getEnergyCurrent();
	int getEnergyMax();
	int getPixelShiftAmountForAnimationInSpriteSheet();
	int currentFrame;
	int currentMaxFrame;
	int getNumIdleAnimationFrames();
	int getNumRunAnimationFrames();
	int getTimeBetweenIdleAnimations();
	int getTimeBetweenRunAnimations();
	int getImageWidth();
	int getImageHeight();
	double getSpeedMax();
	double getAcceleration();
	double xVelocity;
	double yVelocity;
	double xDeltaVelocity;
	double yDeltaVelocity;
	int xPosition;
	int yPosition;
	SDL_Rect rectangle;
	SDL_Rect drawRectangle;
	std::string getImageIdleResource();
	std::string getImageRunResource();
	std::string getName();
	SDL_Texture* getTextureIdle();
	SDL_Texture* getTextureRun();
	SDL_Texture* getTextureActive();
	SDL_Rect getRectangle();
	std::vector<Attribute> getAttributes();
	std::vector<Ability> getAbilities();
	int getDex();
	int getHPCur();
	void takeDamage(Ability a);
	void setTextureActive(SDL_Texture*);

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
	int numIdleAnimationFrames;
	int numRunAnimatonFrames;
	int timeBetweenIdleAnimations; //ms
	int timeBetweenRunAnimations; //ms
	int imageWidth;
	int imageHeight;
	double speedMax; // px/s
	double acceleration; // px/s^2

	void setHPMax();
	void setMPMax();
	void setEnergyMax();
	void setRectangle(SDL_Rect);
	void setAttributes(std::vector<Attribute>);
	void setAbilities(std::vector<Ability>);
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