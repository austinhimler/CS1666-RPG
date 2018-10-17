#include <string>
#include <vector>
#include <SDL.h>
#include "../Headers/Button.h"
#include "../Headers/Helper.h"
	//std::string type; //up stat, down stat, or start
	//int x;
	//int y;
	//int h;
	//int w;
	//int pressed = 0;
	//std::string imageResource;
	//std::string attribute; //change to Attribute object later
	//SDL_Rect rect;
	//SDL_Texture* texture;
Button::Button() {}
Button::Button(std::string type, int x, int y, int w, int h, std::string imageResource, std::string attribute, SDL_Renderer* gRenderer) {
		this->type = type;
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->imageResource = imageResource;
		this->rect = { x, y, w, h };
		this->attribute = attribute;
		this->gRenderer = gRenderer;
		Helper temp = Helper();
		this->texture = temp.loadImage(imageResource,gRenderer);
}


	//SDL_Texture* loadImage(std::string fname) {
	//	SDL_Texture* newText = nullptr;

	//	SDL_Surface* startSurf = IMG_Load(fname.c_str());
	//	if (startSurf == nullptr) {
	//		std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
	//		return nullptr;
	//	}

	//	SDL_SetColorKey(startSurf, SDL_TRUE, SDL_MapRGB(startSurf->format, 0, 0xFF, 0xFF));//if the color is 0, 0xFF, 0xFF, it should be cleaned

	//	newText = SDL_CreateTextureFromSurface(gRenderer, startSurf);
	//	if (newText == nullptr) {
	//		std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
	//	}

	//	SDL_FreeSurface(startSurf);

	//	return newText;
	//}







