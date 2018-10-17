#pragma once

#include <string>
#include <SDL_image.h>

bool init();
SDL_Texture* loadImage(std::string fname);
void close();