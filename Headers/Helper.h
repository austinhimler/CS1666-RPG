#pragma once
#ifndef _____HELPER_H_____
#define _____HELPER_H_____
#include <string>
#include <vector>
#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <cmath>
#include <fstream>
class Helper {
public:
	static SDL_Texture* loadImage(std::string fname,SDL_Renderer* gRenderer);

private:


};





#endif