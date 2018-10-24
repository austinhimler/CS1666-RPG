#ifndef _____TILE_H_____
#define _____TILE_H_____
#include <stdio.h>
#include <string>
#include <fstream>

#include <SDL.h>
#include <SDL_image.h>

#include "Helper.h"
#include "Globals.h"

//max level size
const int LEVEL_WIDTH = 1920;
const int LEVEL_HEIGHT = 1920;
//TILE DEFINITIONS
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;
const int TOTAL_TILES = 900;
const int NUM_SPRITES = 2;
//The different tile sprites
const int TILE_LAKE = 0;
const int TILE_GRASS = 1;
//const int TILE_FLOOR = 2;
static SDL_Texture* grass;
static SDL_Texture* pond;
static SDL_Texture* pondLeft;
static SDL_Texture* pondBottom;
static SDL_Texture* pondTopLeft;
static SDL_Texture* pondTop;
static SDL_Texture* pondBottomLeft;
//loads tile textures



class Tile {
public:
	static void loadTiles();
	SDL_Texture* texture;
	//Initializes position and type
	Tile(int x, int y, int tileType);

	//Shows the tile
	bool render(SDL_Rect* camera);

	//Get the tile type
	int getType();

	//Get the collision box
	SDL_Rect getBox();

	SDL_RendererFlip flip;
	//The attributes of the tile
	SDL_Rect mBox;

	//The tile type
	int mType;
	bool solid;
};
#endif

