
#ifndef _____ITEM_H_____
#define _____ITEM_H_____
#include <string>
#include <vector>
#include "Globals.h"
class Item {
	public:
		std::vector<std::string> name;
		std::vector<std::string> description;
		int weight;
		int armor;
		int damage;
		SDL_Rect rectangle;
		SDL_Rect drawRectangle;
		SDL_Texture* texture;
		Item(std::vector<std::string> name, std::vector<std::string> description, int weight, SDL_Rect rectangle, SDL_Rect drawRectangle, SDL_Texture* texture);
};
#endif