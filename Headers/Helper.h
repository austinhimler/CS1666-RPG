#ifndef _____HELPER_H_____
#define _____HELPER_H_____
#include <string>
#include <vector>
#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <fstream>
#include "Globals.h"

class Helper {
public:
	static SDL_Texture* loadImage(std::string fname,SDL_Renderer* gRenderer);
	static void renderText(const char* text, SDL_Rect* rect, SDL_Color* color, TTF_Font* font);
	static TTF_Font* setFont(const char* path, int n);
protected:
private:

};





#endif