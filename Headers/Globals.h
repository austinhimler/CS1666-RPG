#ifndef _____GLOBALS_H_____
#define _____GLOBALS_H_____

#include <map>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Abilities/AbilityResource.h"

/*
//namespace Char_Help {
	
	const std::string char_type_string[] = {
		"player",
		"enemy"
	};
	

//}//*/


//const map<string, attr_index> attr_map = (ATTR_NAME[]);

enum CHAR_TYPE {
	PLAYER,
	ENEMY
};

enum attr_index {
	STR,
	INT,
	DEX,
	CON,
	FAI,
	ATTRCOUNT
};

const std::string ATTR_NAMES[] = {
	"Strength",
	"Intelligence",
	"Dexerity",
	"Constitution",
	"Faith"
};


//const std::map<std::string, attr_index> attr_map;

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



#endif // !_____GLOBALS_H_____
