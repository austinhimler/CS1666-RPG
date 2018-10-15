#ifndef _____BUTTON_H_____
#define _____BUTTON_H_____
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

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
class Tile {
	//Initializes position and type
	Tile(int x, int y, int tileType);

	//Shows the tile
	void render(SDL_Rect& camera);

	//Get the tile type
	int getType();

	//Get the collision box
	SDL_Rect getBox();

	private:
	//The attributes of the tile
	SDL_Rect mBox;

	//The tile type
	int mType;
};

#endif