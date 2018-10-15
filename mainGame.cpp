#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <cmath>
#include <fstream>
#include "Headers/Globals.h"
#include "Headers/Player.h"
#include "Headers/Button.h"
#include "Headers/Character.h"

// Function declarations
bool init();
SDL_Texture* loadImage(std::string fname);
void close();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
std::vector<SDL_Texture*> gTex;

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;

//std::vector<SDL_Texture*> gTex;
// Music var
Mix_Music *gMusic = NULL;
Mix_Chunk *gBSound = NULL;
TTF_Font* font; 

//Player ONE
Player player1 = Player::Player("nlf4",1,1,1,1,1);

bool init() {
	// Flag what subsystems to initialize
	// For now, just video
	//added audio init
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}


	gWindow = SDL_CreateWindow("CS1666-RPG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	/* Create a renderer for our window
	 * Use hardware acceleration (last arg)
	 * Choose first driver that can provide hardware acceleration
	 *   (second arg, -1)
	 */
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == nullptr) {
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

	// Initialize PNG loading via SDL_image extension library
	int imgFlags = IMG_INIT_PNG;
	imgFlags = imgFlags | IMG_INIT_JPG;//add jpg support
	int retFlags = IMG_Init(imgFlags);
	if (retFlags != imgFlags) {
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}
	//initialize the sdl mixer  
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
		//return false;
	}

	if (TTF_Init() == -1) {
		std::cout << "TTF could not initialize. Error: %s\n", TTF_GetError();
		return false;
	}
	font = TTF_OpenFont("Fonts/Stacked pixel.ttf", 30);
	if (font == NULL) {
		std::cout << "font was null";
	}

	return true;
}

bool check_collision(SDL_Rect* a, SDL_Rect* b) {
	// Check vertical overlap
	if (a->y + a->h <= b->y)
		return false;
	if (a->y >= b->y + b->h)
		return false;

	// Check horizontal overlap
	if (a->x >= b->x + b->w)
		return false;
	if (a->x + a->w <= b->x)
		return false;

	// Must overlap in both
	return true;
}


SDL_Texture* loadImage(std::string fname) {
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

void close() {
	for (auto i : gTex) {
		SDL_DestroyTexture(i);
		i = nullptr;
	}

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;
	TTF_CloseFont(font);
	font = NULL;
	//Free music
	Mix_FreeMusic(gMusic);
	Mix_FreeChunk(gBSound);
	gMusic = NULL;
	// Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void playCredits() {
	//Load the music
	gMusic = Mix_LoadMUS("CREDIT_IMG/BGM.wav");
	if (gMusic == NULL)
		std::cout << "Failed to load music" << std::endl;
	//Play the music
	Mix_PlayMusic(gMusic, -1);

	gTex.push_back(loadImage("CREDIT_IMG/dsgCredits.png"));
	gTex.push_back(loadImage("CREDIT_IMG/RyanKillenCreditImage.jpg")); //Ryan Killen - rek77
	gTex.push_back(loadImage("CREDIT_IMG/bmbCredits.jpg"));
	gTex.push_back(loadImage("CREDIT_IMG/dank_farnan_meme.jpg")); //Austin Himler - arh121
	gTex.push_back(loadImage("CREDIT_IMG/Kexin Wang.jpg"));
	gTex.push_back(loadImage("CREDIT_IMG/justin.jpg"));
	gTex.push_back(loadImage("CREDIT_IMG/my_greatest_creation.png")); // jake
	gTex.push_back(loadImage("CREDIT_IMG/ilum.jpg")); // James Finkel
	gTex.push_back(loadImage("CREDIT_IMG/SankethKolliCredit.jpg")); //Sanketh Kolli - ssk38
	gTex.push_back(loadImage("CREDIT_IMG/mjl159Credits.png")); //Mitchell Leng - mjl159


//This is for the actual credits
	SDL_Event e;
	int j=0;
	for (auto i : gTex)
	{
		while (j < 101)
		{
			SDL_PollEvent(&e);
			if (e.type == SDL_QUIT) {
				Mix_HaltMusic();
				return;
			}
			if (j == 0)
			{
				SDL_RenderCopy(gRenderer, i, NULL, NULL);
				SDL_RenderPresent(gRenderer);
				
			}
			SDL_Delay(30);
			j++;
		}
		j = 0;
	}
	//Stop the music
	Mix_HaltMusic();
}


void renderText(const char* text, SDL_Rect* rect, SDL_Color* color) {
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

bool characterCreateScreen() {
	//loads music and starts it
	gMusic = Mix_LoadMUS("CREDIT_IMG/charactercreate.wav");
	if (gMusic == NULL)
		std::cout << "Failed to load music" << std::endl;
	gBSound = Mix_LoadWAV("CREDIT_IMG/BSound.wav");
	if (gBSound == NULL)
	{
		printf("Failed to load Button sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	//Play the music
	Mix_PlayMusic(gMusic, -1);


	bool onCharacterCreate = true;
	int pointsToAllocate = 25;
	int maxStat = 10;
	int minStat = 1;
	int strength = 1;
	int intelligence = 1;
	int dexterity = 1;
	int constitution = 1;
	int faith = 1;
	int charImageX = 0;
	int charImageY = 0;
	int charImageW = 200;
	int charImageH = 148;
	int delaysPerFrame = 0;
	int frame = 0;

	SDL_Rect characterBox = { 436, 205, 200, 148 };
	SDL_Rect pointsAllocatedRectangle = { 227, 32, 0, 0};
	SDL_Rect strengthTextRectangle = { 250, 115, 0, 0 };
	SDL_Rect intelligenceTextRectangle = { 250, 205, 0, 0 };
	SDL_Rect dexterityTextRectangle = { 250, 302, 0, 0 };
	SDL_Rect constitutionTextRectangle = { 250, 395, 0, 0 };
	SDL_Rect faithTextRectangle = { 250, 490, 0, 0 };
	SDL_Rect nameTextRectangle = { 143, 640, 0,0 };
	SDL_Rect errorTextRectangle = { 465, 580, 0, 0 };
	SDL_Rect errorTextRectangleLong = {445, 580, 0, 0 };
	SDL_Color textColor = { 0, 0, 0, 0 };
	SDL_Color errorColor = { 255, 0, 0, 0 };
	std::string nameInputText;
	std::string errorInputText;

	std::vector<Button*> buttons;
	SDL_Texture* upPress= loadImage("Images/UI/CreateScreen/pointUpArrow_Pressed.png");
	SDL_Texture* downPress = loadImage("Images/UI/CreateScreen/pointDownArrow_Pressed.png");
	SDL_Texture* character = loadImage("Images/Player/Character_Idle.png");
	
	
	//need attr objects
	buttons.push_back(new Button("up", 340, 80, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "strength",gRenderer));
	buttons.push_back(new Button("down", 340, 130, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "strength",gRenderer));
	buttons.push_back(new Button("up", 340, 175, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "intelligence",gRenderer));
	buttons.push_back(new Button("down", 340, 225, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "intelligence", gRenderer));
	buttons.push_back(new Button("up", 340, 270, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "dexterity", gRenderer));
	buttons.push_back(new Button("down", 340, 320, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "dexterity", gRenderer));
	buttons.push_back(new Button("up", 340, 365, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "constitution", gRenderer));
	buttons.push_back(new Button("down", 340, 415, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "constitution", gRenderer));
	buttons.push_back(new Button("up", 340, 460, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "faith",gRenderer));
	buttons.push_back(new Button("down", 340, 510, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "faith", gRenderer));
	buttons.push_back(new Button("start", 450, 600, 244, 95, "Images/UI/CreateScreen/StartButton.png", "", gRenderer));

	SDL_Texture* background = loadImage("Images/UI/CreateScreen/characterCreateV2NoButtons.png"); //Moved to fix memory leak
	
	SDL_Event e;
	while (onCharacterCreate) {
		while (SDL_PollEvent(&e)) {
			
			if (e.type == SDL_QUIT) {
				Mix_HaltMusic();
				return false; //end game
			}
			
			if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_KEYDOWN) {
				errorInputText = "";
			}
			if (e.button.button == (SDL_BUTTON_LEFT) && e.type == SDL_MOUSEBUTTONDOWN) {
				
				std::cout << "button clicked";
				std::cout << pointsToAllocate;
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				int deltaAttribute;
				for (auto i : buttons) {
					//if mouse is clicked inside a button
					if (((mouseX >= i->x) && (mouseX <= (i->x + i->w))) &&
						((mouseY >= i->y) && (mouseY <= (i->y + i->h)))) 
					{
						i->pressed = 5;
						if (i->type == "start") {
							if (pointsToAllocate == 0) {
								if (nameInputText != "") {
									Mix_PlayChannel(-1, gBSound, 0);
									onCharacterCreate = false;
									player1.setAll(nameInputText, strength, intelligence, dexterity, constitution, faith);
									std::cout << std::string(player1); //displays player 1
									//make Character Object, validate, return to main
									for (auto i : buttons) {
										delete(i);
									}
									SDL_DestroyTexture(background);
									Mix_HaltMusic();
									return true;
								}
								else {
									errorInputText = "Enter Your Name!";
								}
							}
							else {
								errorInputText = "Points Remaining!";
								break; //not valid to start, break out of for loop
							}
						}

						if (i->type == "up") {
							if (pointsToAllocate > 0) {
								Mix_PlayChannel(-1, gBSound, 0);
								deltaAttribute = 1;
							}
							else {
								errorInputText = "No Points Remaining!";
								deltaAttribute = 0;
							}
						}
						else if (i->type == "down") {
							Mix_PlayChannel(-1, gBSound, 0);
							deltaAttribute = -1;
						}

						if (i->attribute == "strength") {
							if ((deltaAttribute + strength) <= maxStat && (deltaAttribute + strength) >= minStat) {
								strength += deltaAttribute;
								pointsToAllocate -= deltaAttribute;
							}
							else if ((deltaAttribute + strength) > maxStat) {
								errorInputText = "Max Strength!";
							}
							else if ((deltaAttribute + strength) < minStat) {
								errorInputText = "Min Strength!";
							}
						}
						else if (i->attribute == "intelligence") {
							if ((deltaAttribute + intelligence) <= maxStat && (deltaAttribute + intelligence) >= minStat) {
								intelligence += deltaAttribute;
								pointsToAllocate -= deltaAttribute;
							}
							else if ((deltaAttribute + intelligence) > maxStat) {
								errorInputText = "Max Intelligence!";
							}
							else if ((deltaAttribute + intelligence) < minStat) {
								errorInputText = "Min Intelligence!";
							}
						}
						else if (i->attribute == "dexterity") {
							if ((deltaAttribute + dexterity) <= maxStat && (deltaAttribute + dexterity) >= minStat) {
								dexterity += deltaAttribute;
								pointsToAllocate -= deltaAttribute;
							}
							else if ((deltaAttribute + dexterity) > maxStat) {
								errorInputText = "Max Dexterity!";
							}
							else if ((deltaAttribute + dexterity) < minStat) {
								errorInputText = "Min Dexterity!";
							}
						}
						else if (i->attribute == "constitution") {
							if ((deltaAttribute + constitution) <= maxStat && (deltaAttribute + constitution) >= minStat) {
								constitution += deltaAttribute;
								pointsToAllocate -= deltaAttribute;
							}
							else if ((deltaAttribute + constitution) > maxStat) {
								errorInputText = "Max Constitution!";
							}
							else if ((deltaAttribute + constitution) < minStat) {
								errorInputText = "Min Constitution!";
							}
						}
						else if (i->attribute == "faith") {
							if ((deltaAttribute + faith) <= maxStat && (deltaAttribute + faith) >= minStat) {
								faith += deltaAttribute;
								pointsToAllocate -= deltaAttribute;
							}
							else if ((deltaAttribute + faith) > maxStat) {
								errorInputText = "Max Faith!";
							}
							else if ((deltaAttribute + faith) < minStat) {
								errorInputText = "Min Faith!";
							}
						}
						break;
					}
					
				}
			}
			
			else if (e.type == SDL_KEYDOWN) {
				//remove char if backspace
				if (e.key.keysym.sym == SDLK_BACKSPACE && nameInputText.length() > 0){
					Mix_PlayChannel(-1, gBSound, 0);
					nameInputText.pop_back();
				}
			}

			else if (e.type == SDL_TEXTINPUT) {
				//add char
				//set length limit to arbitrariy be 13 (fits textbox about right, depends on what user enters)
				if (nameInputText.length() < 13) {
					Mix_PlayChannel(-1, gBSound, 0);
					nameInputText += e.text.text;
				}
			}
		}

		SDL_RenderCopy(gRenderer, background, NULL, NULL);
		//Renders buttons and shows pressed image if pressed
		for (auto i : buttons) {
			if(!i->pressed>0||i->attribute=="")
				SDL_RenderCopy(gRenderer, i->texture, NULL, &i->rect);
			else
			{
				if(i->type=="up")
					SDL_RenderCopy(gRenderer, upPress, NULL, &i->rect);
				else
					SDL_RenderCopy(gRenderer, downPress, NULL, &i->rect);
				i->pressed--;
			}
		}
		
		
		charImageX = frame * 200;

		
		SDL_Rect charactersRectangle = { charImageX, charImageY, charImageW, charImageH };
		SDL_RenderCopy(gRenderer, character, &charactersRectangle, &characterBox);

		

		//to add more frames per image to make it more fluid
		//definitely not the best way to do this, need to sync to a consistent gametime
		delaysPerFrame++;
		if (delaysPerFrame >= 6) {
			frame++;
			delaysPerFrame = 0;
		}
		if (frame == 4) {
			frame = 0;
		}
		
		std::string strengthString = std::to_string(strength);
		std::string intelligenceString = std::to_string(intelligence);
		std::string dexterityString = std::to_string(dexterity);
		std::string constitutionString = std::to_string(constitution);
		std::string faithString = std::to_string(faith);
		std::string pointsLeftToAllocateString = std::to_string(pointsToAllocate);

		renderText(strengthString.c_str(), &strengthTextRectangle, &textColor);
		renderText(intelligenceString.c_str(), &intelligenceTextRectangle, &textColor);
		renderText(dexterityString.c_str(), &dexterityTextRectangle, &textColor);
		renderText(constitutionString.c_str(), &constitutionTextRectangle, &textColor);
		renderText(faithString.c_str(), &faithTextRectangle, &textColor);
		renderText(pointsLeftToAllocateString.c_str(), &pointsAllocatedRectangle, &textColor);
		if (nameInputText.length() > 0) {
			renderText(nameInputText.c_str(), &nameTextRectangle, &textColor);
		}
		if (errorInputText.length() > 13) {
			renderText(errorInputText.c_str(), &errorTextRectangleLong, &errorColor);
		}
		else if (errorInputText.length() > 0) {
			renderText(errorInputText.c_str(), &errorTextRectangle, &errorColor);
		}
		SDL_RenderPresent(gRenderer);
		SDL_Delay(16);
	}
}


void playGame() {
	//max level size
	const int LEVEL_WIDTH = 4200;
	const int LEVEL_HEIGHT = 4200;
	//TILE DEFINITIONS
	const int TILE_WIDTH = 42;
	const int TILE_HEIGHT = 42;
	const int TOTAL_TILES = 10000;
	const int NUM_SPRITES = 3;
	//The different tile sprites
	const int TILE_SOLID = 0;
	const int TILE_WALL = 1;
	const int TILE_FLOOR = 2;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	SDL_Rect characterBox = {250, 250, 200, 148 };
	SDL_Rect enemyBox = { 400, 100, 384, 308 };

	SDL_Texture* characterTextureActive = NULL;
	SDL_Texture* characterTextureRun = loadImage("Images/Player/Character_Run.png");
	SDL_Texture* characterTextureIdle = loadImage("Images/Player/Character_Idle.png");
	SDL_Texture* enemyTextureIdle = loadImage("Images/Enemies/Enemy1/Enemy_Idle.png");
	characterTextureActive = characterTextureIdle;
	
	int charMoveSpeed = 2;
	int characterMoveAcceleration = 2;
	int characterMoveMaxSpeed = 4;

	int xVelocity = 0;
	int yVelocity = 0;
	int xDeltaVelocity;
	int yDeltaVelocity;
	int charImageX = 0;
	int charImageY = 0;
	int charImageW = 200;
	int charImageH = 148;
	int enemyImageX = 0;
	int enemyImageY = 0;
	int enemyImageW = 384;
	int enemyImageH = 308;
	int delaysPerFrame = 0;
	int frame = 0;
	int maxFrame = 4;


	SDL_Event e;
	bool inOverworld = true;
	while (inOverworld) {
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) {
				inOverworld = false;
				return;
			}
		}

		xDeltaVelocity = 0;
		yDeltaVelocity = 0;
		const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		if (keyState[SDL_SCANCODE_W])
			yDeltaVelocity -= characterMoveAcceleration;
		if (keyState[SDL_SCANCODE_A])
			xDeltaVelocity -= characterMoveAcceleration;
		if (keyState[SDL_SCANCODE_S])
			yDeltaVelocity += characterMoveAcceleration;
		if (keyState[SDL_SCANCODE_D])
			xDeltaVelocity += characterMoveAcceleration;

		if (xDeltaVelocity == 0) {
			if (xVelocity > 0)
				xDeltaVelocity = -characterMoveAcceleration;
			else if (xVelocity < 0)
				xDeltaVelocity = characterMoveAcceleration;
		}
		if (yDeltaVelocity == 0) {
			if (yVelocity > 0)
				yDeltaVelocity = -characterMoveAcceleration;
			else if (yVelocity < 0)
				yDeltaVelocity = characterMoveMaxSpeed;
		}

		xVelocity += xDeltaVelocity;
		yVelocity += yDeltaVelocity;

		//Change sprite if character is in motion
		if (xVelocity != 0 || yVelocity != 0) {
			if (characterTextureActive != characterTextureRun) {
				characterTextureActive = characterTextureRun;
				frame = 0;
				maxFrame = 6;
			}
		}
		else {
			if (characterTextureActive != characterTextureIdle) {
				characterTextureActive = characterTextureIdle;
				frame = 0;
				maxFrame = 4;
			}
		}
	
		//bound within Max Speed
		if (xVelocity < -characterMoveMaxSpeed)
			xVelocity = -characterMoveMaxSpeed;
		else if (xVelocity > characterMoveMaxSpeed)
			xVelocity = characterMoveMaxSpeed;
		//bound within Max Speed
		if (yVelocity < -characterMoveMaxSpeed)
			yVelocity = -characterMoveMaxSpeed;
		else if (yVelocity > characterMoveMaxSpeed)
			yVelocity = characterMoveMaxSpeed;

		//Move vertically
		characterBox.y += yVelocity;
		if (characterBox.y < 0 || (characterBox.y + characterBox.h > SCREEN_HEIGHT)) {
			//go back into window
			characterBox.y -= yVelocity;
		}

		//Move horizontally
		characterBox.x += xVelocity;
		if (characterBox.x < 0 || (characterBox.x + characterBox.w > SCREEN_WIDTH)) {
			//go back into window
			characterBox.x -= xVelocity;
		}
		if (xVelocity > 0 && flip == SDL_FLIP_HORIZONTAL)
			flip = SDL_FLIP_NONE;
		else if (xVelocity < 0 && flip == SDL_FLIP_NONE)
			flip = SDL_FLIP_HORIZONTAL;
		//Set Black
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);

		enemyImageX = frame * 384;
		charImageX = frame * 200;
		
		SDL_Rect charactersRectangle = { charImageX, charImageY, charImageW, charImageH};
		SDL_Rect enemyRectangle = { enemyImageX, enemyImageY, enemyImageW, enemyImageH };
		SDL_RenderCopyEx(gRenderer, characterTextureActive, &charactersRectangle, &characterBox,0.0,nullptr, flip);
		
		SDL_RenderCopy(gRenderer, enemyTextureIdle, &enemyRectangle, &enemyBox);

		SDL_RenderPresent(gRenderer);

		//to add more frames per image to make it more fluid
		//definitely not the best way to do this, need to sync to a consistent gametime
		delaysPerFrame++;
		if (delaysPerFrame >= 6) {
			frame++;
			delaysPerFrame = 0;
		}
		if (frame == maxFrame) {
			frame = 0;
		}
		SDL_Delay(16);
	}
	
	 //while(gameOn) gameloop
		//render top viewport: render player, enemy, overworld
		//render bottom viewport: UI
		//movement
		//collision detection
		//when player collides into enemy
		// combatScene(vector of Type Characters);
}

void combatScene() {










}

int main(int argc, char *argv[]) {
	/*
	Resistance r = Resistance("Resistance");
	std::cout << r.to_string() << std::endl;
	//*/
	
	if (!init()) {
		std::cout << "Failed to initialize!" << std::endl;
		close();
		return 1;
	}
	bool keepPlaying = characterCreateScreen();
	if (keepPlaying) {
		playGame();
		playCredits();
	}
	close();
	//*/
	return 0;
}
