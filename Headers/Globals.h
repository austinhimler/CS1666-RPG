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

enum CHAR_TYPE {
	PLAYER,
	ENEMY
};


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

enum SPRITEINDEX {
	NOT_READY,
	IDLE_DOWN,
	IDLE_RIGHT,
	IDLE_LEFT,
	IDLE_UP,
	RUN_DOWN,
	RUN_RIGHT,
	RUN_LEFT,
	RUN_UP,
	RUN_DOWN_RIGHT,
	RUN_DOWN_LEFT,
	RUN_UP_RIGHT,
	RUN_UP_LEFT
};

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern std::vector<SDL_Texture*> gTex;

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;
const int ENEMIES_PER_CLUSTER = 1;
const int STARTING_ENEMIES = 2;
const std::vector<std::string> ALL_MAPS = { "map1.txt", "map2.txt", "map3.txt" };
const int MAX_HORIZONTAL_TILES = 30;
const int MAX_VERTICAL_TILES = 30;
const int MAX_INSIDE_TILE = 64;
const int MAX_X_POSITION = 1920;
const int MAX_Y_POSITION = 1920;



#endif // !_____GLOBALS_H_____
