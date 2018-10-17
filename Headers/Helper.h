#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class Helper {
public:
	static SDL_Texture* loadImage(std::string fname,SDL_Renderer* gRenderer);

private:


};