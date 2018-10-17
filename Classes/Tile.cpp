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

	//Get the tile type
	mType = tileType;
	Helper temp = Helper();
	switch (tileType)
	{
	case 1:
		texture = temp.loadImage("Images/Tiles/64^64 lake.png", gRenderer);
		break;
	case 0:
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
		SDL_RenderCopy(gRenderer,texture,NULL,&Temp);
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








