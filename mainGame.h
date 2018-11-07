#pragma once
#include <string>
#include <SDL/SDL_image.h>

bool init();
SDL_Texture* loadImage(std::string fname);
void close();