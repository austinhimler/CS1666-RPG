#ifndef _____BUTTON_H_____
#define _____BUTTON_H_____
#include <string>
#include <vector>
#include <SDL2/SDL.h>

class Button {
public:
	std::string type; //up stat, down stat, or start
	int x;
	int y;
	int h;
	int w;
	int pressed;
	bool locked;
	std::string imageResource;
	std::string attribute; //change to Attribute object later
	SDL_Rect rect;
	SDL_Texture* texture;
	SDL_Renderer* gRenderer;
	Button();
	Button(std::string type, int x, int y, int w, int h, std::string imageResource, std::string attribute, SDL_Renderer* gRenderer);
	
private:

};
#endif