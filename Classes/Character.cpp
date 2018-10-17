#include "../Headers/Character.h"
#include <iostream>

	Character::Character(std::string n, std::vector<Attribute> attr) {
		attributes = attr;
		setHPMax();
		setMPMax();
		setEnergyMax();
		hpCurrent = hpMax;
		mpCurrent = mpMax;
		energyCurrent = energyMax;
		energyRegen = (energyMax < 10)?1:0.1 * energyMax;
		name = n;
		buff = std::vector<int>(BUFFCOUNT, 0);
		learnAbility(ATTACK);
		learnAbility(DEFEND);
		learnAbility(ESCAPE);
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

	int Character::beingTarget(Ability* a) {
		int i;
		int result = -1;
		switch (a->getType()) {
		case AbilityResource::tDAMAGE:
			result = hpCurrent;
			hpCurrent -= a->getVal();
			if (hpCurrent < 0)hpCurrent = 0;
			else result = a->getVal();
			break;
		case AbilityResource::tHEALING:
			result = hpMax - hpCurrent;
			hpCurrent += a->getVal();
			if (hpCurrent > hpMax) hpCurrent = hpMax;
			else result = a->getVal();
			break;
		case AbilityResource::tESCAPE:
			i = rand() % 2;
			if (i == 0) result = -2;
			else result = -1;
			break;
		case AbilityResource::tDEFENSE:
			buff[ENERGYREGEN]++;
			result = 1;
			break;
		case AbilityResource::tSUMMON:
			break;
		default:
			std::cout << "unimplemented ability type!" << std::endl;
			break;
		}

		return result;
	}

	
	void Character::learnAbility(int a) {
		Ability* abil = new Ability(a, {1,2,4}/*currently only str, int, con affect abilities*/, attributes);
		for (auto& i : abilities) {
			if (i.cmp(a)) {
				i = *abil;//used for updating learned abilities
				return;
			}
		}
		abilities.push_back(*abil);
		abil_helper[a] = abilities.size() - 1;
	}

	void Character::updateEnergy() {
		int temp = energyRegen + buff[ENERGYREGEN];
		energyCurrent += (temp >= 0) ? temp : 0;
		if (energyCurrent > energyMax) energyCurrent = energyMax;
		buff[ENERGYREGEN] = 0;
	}
	//*/
	std::string Character::toString() {
		std::string s = "Name: " + name + "\n";
		s += to_string(getHPCurrent()) + "/" + to_string(getHPMax())+"\n";
		for (auto i : attributes) {
			s += i.toString() + "\n";
		}
		return s;
	}

	int Character::getHelp(int n) { return abil_helper[n]; }
	int Character::getDex() { return attributes[DEX].current;}
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

