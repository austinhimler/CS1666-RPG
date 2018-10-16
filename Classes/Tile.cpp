#include "../Headers/Tile.h"
#include "../Headers/Helper.h"
#include "../Headers/Globals.h"

//0 lake 1 grass
Tile::Tile(int x, int y, int tileType)
{
	//Get the offsets
	mBox->x = x;
	mBox->y = y;

	//Set the collision box
	mBox->w = TILE_WIDTH;
	mBox->h = TILE_HEIGHT;

	//Get the tile type
	mType = tileType;
	Helper temp = Helper();
	switch (tileType)
	{
	case 0:
		texture = temp.loadImage("Images/Tiles/64^64 lake.png", gRenderer);
		break;
	case 1:
		texture = temp.loadImage("Images/Tiles/singleGrass.png", gRenderer);
		break;
	}
}
void Tile::render(SDL_Rect* camera)
{
	
	//If the tile is on screen
	if (SDL_IntersectRect(camera, mBox, NULL))
	{
		//Show the tile
		SDL_RenderCopy(gRenderer,texture,NULL,mBox);
	}
}
int Tile::getType()
{
	return mType;
}

SDL_Rect* Tile::getBox()
{
	return mBox;
}








