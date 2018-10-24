#ifndef _____CHARACTER_H_____
#define _____CHARACTER_H_____

#include <string>
#include <vector>
#include "Attribute.h"
#include "Resistance.h"
#include "Ability.h"
#include "Globals.h"
#include <map>

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
	int getDex();
	int getHelp(int n);
	bool is_Enemy();

	double xVelocity;
	double yVelocity;
	double xDeltaVelocity;
	double yDeltaVelocity;
	double xPosition;
	double yPosition;
	Uint32 timeSinceLastMovement;
	Uint32 timeSinceLastAnimation;
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
	
	int beingTarget(Ability* a); // return -1 for succesful escape, -2 for failed escape, non-negative integers for HP healed or damaged
	//void takeDamage(Ability a);
	void learnAbility(int a);

	std::vector<int> useAbility(Ability* a); // return vector containing only one -1 if not enough of enenry, return vector of only one -2 if not enough of MP, otherwise return vector containing energy and mp used
	void regenEnenrgy();

	string toString();
	void setTextureActive(SDL_Texture*);

protected:
	bool isEnemy;
	int hpMax;
	int hpCurrent;
	int energyMax;
	int energyCurrent;
	int energyRegen;
	int mpMax;
	int mpCurrent;
	std::vector<int> buff;
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

	std::vector<Attribute> attributes;
	std::vector<Ability> abilities;
	std::map<int, int> abil_helper;
	
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