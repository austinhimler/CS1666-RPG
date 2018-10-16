#include "../Headers/Character.h"


	Character::Character(std::string n, std::vector<Attribute> attr) {
		attributes = attr;
		setHPMax();
		setMPMax();
		setEnergyMax();
		hpCurrent = hpMax;
		mpCurrent = mpMax;
		energyCurrent = energyMax;
		name = n;
	}
	Character::Character(std::string n, int s, int i, int d, int c, int f) {
		attributes = std::vector<Attribute>(5);
		attributes[STR] = Attribute("Strength", s);
		attributes[INT] = Attribute("Intelligence", i);
		attributes[DEX] = Attribute("Dexerity", d);
		attributes[CON] = Attribute("Constitution", c);
		attributes[FAI] = Attribute("Faith", f);
		new Character(n, attributes);
	}
	Character::Character(std::string n) {
		Character(n, 1, 1, 1, 1, 1);
	}
	Character::Character() {
		Character("Character 1");
	}

	int Character::getHPMax() { return hpMax; }
	int Character::getMPMax() { return mpMax; }
	int Character::getEnergyMax() { return energyMax; }
	void Character::setHPMax() { hpMax = 100 * attributes[CON].current; }
	void Character::setMPMax() { mpMax = 100 * attributes[INT].current; }
	void Character::setEnergyMax() { energyMax = 100 * attributes[DEX].current; }
	void Character::setTextureActive(SDL_Texture* text) { textureActive = text; }
	int Character::getHPCurrent() { return hpCurrent; }
	int Character::getMPCurrent() { return mpCurrent; }
	int Character::getEnergyCurrent() { return energyCurrent;  }
	int Character::getPixelShiftAmountForAnimationInSpriteSheet() { return pixelShiftAmountForAnimationInSpriteSheet; }
	int Character::getNumIdleAnimationFrames() { return numIdleAnimationFrames; }
	int Character::getNumRunAnimationFrames() { return numRunAnimatonFrames; }
	int Character::getTimeBetweenIdleAnimations() { return timeBetweenIdleAnimations; }
	int Character::getTimeBetweenRunAnimations() { return timeBetweenRunAnimations;  }
	int Character::getImageWidth() { return imageWidth; }
	int Character::getImageHeight() { return imageHeight; }
	std::string Character::getImageIdleResource() { return imageIdleResource; }
	std::string Character::getImageRunResource() { return imageRunResource; }
	std::string Character::getName() { return name; }
	SDL_Texture* Character::getTextureIdle() { return textureIdle; }
	SDL_Texture* Character::getTextureRun() { return textureRun; }
	SDL_Texture* Character::getTextureActive() { return textureActive; }
	SDL_Rect Character::getRectangle() { return rectangle; }
	std::vector<Attribute> Character::getAttributes() { return attributes; }
	std::vector<Ability> Character::getAbilities() { return abilities; }
	double Character::getSpeedMax() { return speedMax; }
	double Character::getAcceleration() { return acceleration; }

