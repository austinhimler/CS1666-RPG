
#ifndef _____CHARACTER_H_____
#define _____CHARACTER_H_____
#include <string>
#include <vector>
#include "Attribute.h"
#include "Resistance.h"
#include "Ability.h"
#include "Globals.h"
#include "Ailment.h"
#include <map>
#include <algorithm>

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
	
	int getLevel();
	int getHPMax();
	int getHPCurrent();
	int getMPCurrent();
	int getMPMax();
	int getEnergyCurrent();
	int getEnergyMax();
	int getStatus();
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
	std::vector<Attribute> getAttributes();
	Attribute getAttr(int i);
	std::vector<Ability> getAbilities();
	std::vector<Ailment> getAilm();
	
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
	SDL_RendererFlip flip;
	std::string getImageIdleResource();
	std::string getImageRightIdleResource();
	std::string getImageRunResource();
	std::string getImageDownRunResource();
	std::string getImageDownRightRunResource();
	std::string getImageUpRightRunResource();
	std::string getImageUpRunResource();
	std::string getName();
	SDL_Texture* getTextureRightIdle();
	SDL_Texture* getTextureIdle();
	SDL_Texture* getTextureIdleNotReady();
	SDL_Texture* getTextureRun();
	SDL_Texture* getTextureDownRun();
	SDL_Texture* getTextureDownRightRun();
	SDL_Texture* getTextureUpRightRun();
	SDL_Texture* getTextureUpRun();
	SDL_Texture* getTextureActive();
	SDL_Rect getRectangle();
	
	int beingTarget(Ability* a); // return -1 for succesful escape, -2 for failed escape, non-negative integers for HP healed or damaged
	
	//void takeDamage(Ability a);
	void learnAbility(int a);

	void takeAilm(Ailment ailm);
	void ailmAffect();

	void checkStatus();

	int updateEnergy(Ability* a);

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
	int level;
	std::vector<int> buff;
	std::string imageIdleResource;
	std::string imageIdleNotReadyResource;
	std::string imageRightIdleResource;
	std::string imageRunResource;
	std::string imageDownRunResource;
	std::string imageDownRightRunResource;
	std::string imageUpRightRunResource;
	std::string imageUpRunResource;
	std::string name;
	SDL_Texture* textureIdle;
	SDL_Texture* textureIdleNotReady;
	SDL_Texture* textureRightIdle;
	SDL_Texture* textureRun;
	SDL_Texture* textureDownRun;
	SDL_Texture* textureDownRightRun;
	SDL_Texture* textureUpRightRun;
	SDL_Texture* textureUpRun;
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
	std::vector<Ailment> ailments;
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
