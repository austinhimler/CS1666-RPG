#ifndef _____GLOBALS_H_____
#define _____GLOBALS_H_____
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

enum attr_index {
	STR,
	INT,
	DEX,
	CON,
	FAI
};

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
std::vector<SDL_Texture*> gTex;

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;

#endif // !_____GLOBALS_H_____
