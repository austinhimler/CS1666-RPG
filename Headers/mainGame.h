#pragma once
#include <string>
#include <SDL2/SDL_image.h>

bool init();
SDL_Texture* loadImage(std::string fname);
void close();