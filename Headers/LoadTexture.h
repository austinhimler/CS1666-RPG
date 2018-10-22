#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include "Globals.h"
class LoadTexture
{
public:
	//Initializes variables
	LoadTexture();

	//Deallocates memory
	~LoadTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path,SDL_Renderer* gRenderer);


	//Creates blank texture
	bool createBlank(int width, int height, SDL_TextureAccess  SDL_TEXTUREACCESS_STREAMING, SDL_Renderer* gRenderer);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE,SDL_Renderer* gRenderer);
	void renderBackground(SDL_Renderer* gRenderer);
	//Set self as render target
	void setAsRenderTarget(SDL_Renderer* gRenderer);

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//Pixel manipulators
	bool lockTexture();
	bool unlockTexture();
	void* getPixels();
	void copyPixels(void* pixels);
	int getPitch();
	Uint32 getPixel32(unsigned int x, unsigned int y);

private:
	//The actual hardware texture
	SDL_Texture* mTexture;
	void* mPixels;
	int mPitch;

	//Image dimensions
	int mWidth;
	int mHeight;
};

