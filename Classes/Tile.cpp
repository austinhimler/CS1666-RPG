#include "../Headers/Tile.h"
#include "../Headers/Helper.h"
#include "../Headers/Globals.h"

//0: grass 1:water
Tile::Tile(int x, int y, int tileType)
{
	//Get the offsets
	mBox.x = x;
	mBox.y = y;

	//Set the collision box
	mBox.w = TILE_WIDTH;
	mBox.h = TILE_HEIGHT;
	flip = SDL_FLIP_NONE;
	//Get the tile type
	mType = tileType;
	Helper temp = Helper();
	switch (tileType)
	{
	case 9://pond right edge
		texture = temp.loadImage("Images/Tiles/pondLeft.png", gRenderer);
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case 8://pond top right corner
		texture = temp.loadImage("Images/Tiles/pondTopLeft.png", gRenderer);
		flip=SDL_FLIP_HORIZONTAL;
		break;
	case 7://pond top left corner
		texture = temp.loadImage("Images/Tiles/pondTopLeft.png", gRenderer);
		break;
	case 6://pond bottom right corner
		texture = temp.loadImage("Images/Tiles/pondBottomLeft.png", gRenderer);
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case 5://pond bottom left corner
		texture = temp.loadImage("Images/Tiles/pondBottomLeft.png", gRenderer);
		break;
	case 4://pond bottom edge
		texture = temp.loadImage("Images/Tiles/pondBottom.png", gRenderer);
		break;
	case 3://pond top edge
		texture = temp.loadImage("Images/Tiles/pondTop.png", gRenderer);
		break;
	case 2://pond left edge
		texture = temp.loadImage("Images/Tiles/pondLeft.png", gRenderer);
		break;
	case 1://pond
		texture = temp.loadImage("Images/Tiles/pond.png", gRenderer);
		break;
	case 0://grass
		texture = temp.loadImage("Images/Tiles/singleGrass.png", gRenderer);
		break;
	}
}
//To render everytile have a loop iterate through the tile array calling .render and pass the camera object
//The tile will be rendered to the screen if it collides with the camera object
//only check for collisions with player for tiles if this function returns true
bool Tile::render(SDL_Rect* camera)
{
	
	//If the tile is on screen
	if (SDL_HasIntersection(camera, &mBox))
	{
		SDL_Rect Temp = {mBox.x-camera->x,mBox.y-camera->y, mBox.w,mBox.h};
		SDL_RenderCopyEx(gRenderer,texture,NULL,&Temp,0.0,nullptr,flip);
		return true;
	}
	return false;
}
int Tile::getType()
{
	return mType;
}

SDL_Rect Tile::getBox()
{
	return mBox;
}








