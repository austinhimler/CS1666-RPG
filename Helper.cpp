#include "Headers/Helper.h"

SDL_Texture* Helper::loadImage(std::string fname, SDL_Renderer* gRenderer) 
{
	SDL_Texture* newText = nullptr;

	SDL_Surface* startSurf = IMG_Load(fname.c_str());
	if (startSurf == nullptr) {
		std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	SDL_SetColorKey(startSurf, SDL_TRUE, SDL_MapRGB(startSurf->format, 0, 0xFF, 0xFF));//if the color is 0, 0xFF, 0xFF, it should be cleaned

	newText = SDL_CreateTextureFromSurface(gRenderer, startSurf);
	if (newText == nullptr) {
		std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(startSurf);

	return newText;
}

void Helper::renderText(const char* text, SDL_Rect* rect, SDL_Color* color, TTF_Font* font) {
	SDL_Surface* surface;
	SDL_Texture* texture;
	
	surface = TTF_RenderText_Solid(font, text, *color);
	texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	rect->w = surface->w;
	rect->h = surface->h;
	SDL_FreeSurface(surface);
	SDL_RenderCopy(gRenderer, texture, NULL, rect);
	SDL_DestroyTexture(texture);
}

SDL_Texture* loadText(std::string text, SDL_Color* color, TTF_Font* font) {
	SDL_Surface* surface;
	SDL_Texture* texture;

	surface = TTF_RenderText_Solid(font, text.c_str(), *color);
	texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	if (texture == nullptr) {
		std::cout << "Unable to create texture from: '" << text << "'! SDL Error: " << SDL_GetError() << std::endl;
	}
	return texture;
}

TTF_Font* Helper::setFont(const char* path, int n) {
	TTF_Font* font = TTF_OpenFont(path, n);
	if (font == NULL) {
		std::cout << "font was null";
	}
	return font;
}