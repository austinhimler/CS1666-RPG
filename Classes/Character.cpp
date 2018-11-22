#include <iostream>

#include "../Headers/Character.h"

	Character::Character(std::string n, std::vector<Attribute> attr) {
		attributes = attr;
		setHPMax();
		setMPMax();
		setEnergyMax();
		hpCurrent = hpMax;
		mpCurrent = mpMax;
		flip = SDL_FLIP_NONE;
		energyCurrent = energyMax;
		energyRegen = (energyMax < 10)?1:0.1 * energyMax;
		name = n;
		buff = std::vector<int>(BUFFCOUNT, 0);
		level = 1;
		learnAbility(ATTACK);
		learnAbility(DEFEND);
		learnAbility(ESCAPE);
		xVelocity = 0;
		yVelocity = 0;
	}
	Character::Character(std::string n, int s, int i, int d, int c, int f) : Character(n, std::vector<Attribute>({ Attribute("Strength", s), Attribute("Intelligence", i) ,Attribute("Dexerity", d) , Attribute("Constitution", c) ,Attribute("Faith", f) })) {}
	Character::Character(std::string n) : Character(n, 1, 1, 1, 1, 1) {}
	Character::Character() : Character("Character 1") {}

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
			i = rand() % 100;
			if (i <= 49 + FAI) result = -2; // successful escape
			else result = -1; // failing escape
			break;
		case AbilityResource::tDEFENSE:
			buff[ENERGYREGEN]++;
			result = 0;
			break;
		case AbilityResource::tSUMMON:
			break;
		default:
			std::cout << "unimplemented ability type!" << std::endl;
			break;
		}

		return result;
	}


	void Character::checkStatus() {
		if (hpCurrent < 0) hpCurrent = 0;
		else if (hpCurrent > hpMax) hpCurrent = hpMax;
		if (mpCurrent < 0) mpCurrent = 0;
		else if (mpCurrent > mpMax) mpCurrent = mpMax;
		if (energyCurrent < 0) energyCurrent = 0;
		else if (energyCurrent > energyMax) energyCurrent = energyMax;
	}

	void Character::ailmAffect() {
		if (ailments.size() == 0) return;
		Ailment ailm;
		for (int i = 0; i < ailments.size(); i++) {
			ailm = ailments[i];
			switch (ailm.getType())
			{
			case AilmentResource::tNONE:
				break;
			case AilmentResource::tHP_ONLY:
				hpCurrent -= ailm.getVal();
				break;
			case AilmentResource::tMP_ONLY:
				mpCurrent -= ailm.getVal();
				break;
			case AilmentResource::tEN_ONLY:
				energyCurrent -= ailm.getVal();
				break;
			case AilmentResource::tCURSE:
				if (ailm.getTN() == 1)
					hpCurrent = 0;
				break;
			case AilmentResource::tALL:
				break;
			default:
				break;
			}
			checkStatus();
			ailm.update();
			if (ailm.getTN() == 0 && ailm.getSA() == 0)
				ailments.erase(ailments.begin() + i);
		}
	}	

	
	void Character::learnAbility(int a) {
		Ability* abil = new Ability(a, AbilityResource::abilityAttr[a], attributes);
		for (auto& i : abilities) {
			if (i.cmp(a)) {
				i = *abil;//used for updating learned abilities
				return;
			}
		}
		abilities.push_back(*abil);
		abil_helper[a] = (int)abilities.size() - 1;
	}

	int Character::updateEnergy(Ability* a) {
		if (a == nullptr) { // for updating energy between turns
			int temp = energyRegen + buff[ENERGYREGEN];
			energyCurrent += (temp >= 0) ? temp : 0;
			if (energyCurrent > energyMax) energyCurrent = energyMax;
			buff[ENERGYREGEN] = 0;
		}
		else {
			if (energyCurrent < a->getEnergyCost())
				return -1;
			energyCurrent -= a->getEnergyCost();
		}
		return energyCurrent;
	}

	void Character::takeAilm(Ailment ailm) {
		if (ailments.size() != 0)
		{
			Ailment ailm_temp;
			std::vector<Ailment>::iterator temp = find(ailments.begin(), ailments.end(), ailm);
			if (temp != ailments.end())
			{
				ailm_temp = *temp;
				ailm_temp.combineAilment(ailm);
				*temp = ailm_temp;
				return;
			}
			if (ailm == *ailments.end()) {
				ailm_temp = *ailments.end();
				ailm_temp.combineAilment(ailm);
				*ailments.end() = ailm_temp;
				return;
			}
		}
		ailments.push_back(ailm);
	}
	//*/
	std::string Character::toString() {
		std::string s = "Name: " + name + "\n";
		s += "HP: " + to_string(getHPCurrent()) + "/" + to_string(getHPMax())+"\n";
		s += "MP: " + to_string(getMPCurrent()) + "/" + to_string(getMPMax()) + "\n";
		s += "Energy: " + to_string(getEnergyCurrent()) + "/" + to_string(getEnergyMax()) + " Energy Regenerated: " + to_string(energyRegen) + "\n";
		for (auto i : attributes) {
			s += i.toString() + "\n";
		}
		s += "Current Status: ";
		if (getStatus() == 0) {
			s += "Normal\n";
		}
		return s;
	}

	int Character::getHelp(int n) { return abil_helper[n]; }
	int Character::getDex() { return attributes[DEX].current;}
	int Character::getHPMax() { return hpMax; }
	int Character::getMPMax() { return mpMax; }
	int Character::getEnergyMax() { return energyMax; }
	int Character::getStatus() { return 0; }
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
	std::string Character::getImageRightIdleResource() { return imageRightIdleResource; }
	std::string Character::getImageRunResource() { return imageRunResource; }
	std::string Character::getImageUpRunResource() { return imageUpRunResource; }
	std::string Character::getImageDownRunResource() { return imageDownRunResource; }
	std::string Character::getImageDownRightRunResource() { return imageDownRightRunResource; }
	std::string Character::getImageUpRightRunResource() { return imageUpRightRunResource; }
	std::string Character::getName() { return name; }
	SDL_Texture* Character::getTextureIdle() { return textureIdle; }
	SDL_Texture* Character::getTextureIdleNotReady() { return textureIdleNotReady; }
	SDL_Texture* Character::getTextureRightIdle() { return textureIdle; }
	SDL_Texture* Character::getTextureRun() { return textureRun; }
	SDL_Texture* Character::getTextureDownRun() { return textureDownRun; }
	SDL_Texture* Character::getTextureDownRightRun() { return textureDownRightRun; }
	SDL_Texture* Character::getTextureUpRightRun() { return textureUpRightRun; }
	SDL_Texture* Character::getTextureUpRun() { return textureUpRun; }
	SDL_Texture* Character::getTextureActive() { return textureActive; }
	SDL_Rect Character::getRectangle() { return rectangle; }
	std::vector<Attribute> Character::getAttributes() { return attributes; }
	std::vector<Ability> Character::getAbilities() { return abilities; }
	double Character::getSpeedMax() { return speedMax; }
	double Character::getAcceleration() { return acceleration; }
	bool Character::is_Enemy() { return isEnemy; }
	int Character::getLevel() { return level; }
	Attribute Character::getAttr(int i) { return attributes[i]; }
	std::vector<Ailment> Character::getAilm() { return ailments; }