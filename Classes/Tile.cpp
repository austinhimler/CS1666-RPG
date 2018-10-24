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
	impassable = true;
	switch (tileType)
	{
	case 9://pond right edge
		texture = pondLeft;
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case 8://pond top right corner
		texture = pondTopLeft;
		flip= SDL_FLIP_HORIZONTAL;
		break;
	case 7://pond top left corner
		texture = pondTopLeft;
		break;
	case 6://pond bottom right corner
		texture = pondBottomLeft;
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case 5://pond bottom left corner
		texture = pondBottomLeft;
		break;
	case 4://pond bottom edge
		texture = pondBottom;
		break;
	case 3://pond top edge
		texture = pondTop;
		break;
	case 2://pond left edge
		texture = pondLeft;
		break;
	case 1://pond
		texture = pond;
		break;
	case 0://grass
		texture = grass;
		impassable = false;
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

void Tile::loadTiles()
{

	pondTopLeft = Helper::loadImage("Images/Tiles/pondTopLeft.png", gRenderer);

	pondLeft = Helper::loadImage("Images/Tiles/pondLeft.png", gRenderer);

	pondBottomLeft = Helper::loadImage("Images/Tiles/pondBottomLeft.png", gRenderer);

	pondBottom = Helper::loadImage("Images/Tiles/pondBottom.png", gRenderer);

	pondTop = Helper::loadImage("Images/Tiles/pondTop.png", gRenderer);

	pond = Helper::loadImage("Images/Tiles/pond.png", gRenderer);

	grass = Helper::loadImage("Images/Tiles/singleGrass.png", gRenderer);

}






