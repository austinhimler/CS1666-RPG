#pragma once

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Abilities/AbilityResource.h"

enum attr_index {
	STR,
	INT,
	DEX,
	CON,
	FAI,
	ATTRCOUNT
};

enum ABILITY {
	ATTACK,
	FIREBALL,
	ESCAPE,
	DEFEND,
	SUMMON,
	ABILCOUNT
};


enum BUFF {
	ENERGYREGEN,
	BUFFCOUNT
};

enum AILMENTS {
	SILENCE,
	POISON,
};

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern std::vector<SDL_Texture*> gTex;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;