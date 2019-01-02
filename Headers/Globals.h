#ifndef _____GLOBALS_H_____
#define _____GLOBALS_H_____

#include <map>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Attributes/Attr_Globals.h"
#include "Abilities/AbilityResource.h"
#include "Ailments/AilmentResource.h"
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


//const map<std::string, ATTR_NAMES> attr_map = {

//};

enum ABILITY {
	ATTACK,
	FIREBALL,
	ARROWSHOT,
	DEFEND,
	SMITE,
	HEAL,
	MASSHEAL,
	ESCAPE,
	ABILCOUNT
};

enum BUFF {
	ENERGYREGEN,
	BUFFCOUNT
};

enum AILMENTS {
	SILENCE,
	POISON,
	FIRE,
	STUN,
	CURSE
};
	
enum COMBATFLAGS {
	IN_COMBAT,
	PLAYER_WINS,
	ENEMY_WINS,
	PLAYER_ESCAPES,
	ENEMY_ESCAPES,
	DEAD,
	ESCAPED,
	PLAYER_EXIT
};

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern std::vector<SDL_Texture*> gTex;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;



#endif // !_____GLOBALS_H_____
