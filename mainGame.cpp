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
#include "Headers/Tile.h"
#include "Headers/CombatManager.h"
#include "Headers/Cluster.h"

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
Player player1 = Player("nlf4",1,1,1,1,1);

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

bool check_collision(SDL_Rect a, SDL_Rect b) {
	// Check vertical overlap
	if (a.y + a.h <= b.y)
		return false;
	if (a.y >= b.y + b.h)
		return false;

	// Check horizontal overlap
	if (a.x >= b.x + b.w)
		return false;
	if (a.x + a.w <= b.x)
		return false;

	// Must overlap in both
	return true;
}

void combatTransition(){
	SDL_Rect wipe = { 0,0,72,72 };
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	for (; wipe.x < 720; wipe.x += 72)
	{
		for (wipe.y = 0; wipe.y < 720; wipe.y += 72)
		{
			SDL_RenderFillRect(gRenderer, &wipe);
			SDL_RenderPresent(gRenderer);
			SDL_Delay(10);
		}
	}
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
	int j = 0;
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
	SDL_Rect pointsAllocatedRectangle = { 227, 32, 0, 0 };
	SDL_Rect strengthTextRectangle = { 250, 115, 0, 0 };
	SDL_Rect intelligenceTextRectangle = { 250, 205, 0, 0 };
	SDL_Rect dexterityTextRectangle = { 250, 302, 0, 0 };
	SDL_Rect constitutionTextRectangle = { 250, 395, 0, 0 };
	SDL_Rect faithTextRectangle = { 250, 490, 0, 0 };
	SDL_Rect nameTextRectangle = { 143, 640, 0,0 };
	SDL_Rect errorTextRectangle = { 465, 580, 0, 0 };
	SDL_Rect errorTextRectangleLong = { 445, 580, 0, 0 };
	SDL_Color textColor = { 0, 0, 0, 0 };
	SDL_Color errorColor = { 255, 0, 0, 0 };
	std::string nameInputText;
	std::string errorInputText;

	std::vector<Button*> buttons;
	SDL_Texture* upPress = loadImage("Images/UI/CreateScreen/pointUpArrow_Pressed.png");
	SDL_Texture* downPress = loadImage("Images/UI/CreateScreen/pointDownArrow_Pressed.png");
	SDL_Texture* character = loadImage("Images/Player/Character_Idle.png");


	//need attr objects
	buttons.push_back(new Button("up", 340, 80, 35, 45, "Images/UI/CreateScreen/pointUpArrow.png", "strength", gRenderer));
	buttons.push_back(new Button("down", 340, 130, 35, 42, "Images/UI/CreateScreen/pointDownArrow.png", "strength", gRenderer));
	buttons.push_back(new Button("up", 340, 175, 35, 45, "Images/UI/CreateScreen/pointUpArrow.png", "intelligence", gRenderer));
	buttons.push_back(new Button("down", 340, 225, 35, 42, "Images/UI/CreateScreen/pointDownArrow.png", "intelligence", gRenderer));
	buttons.push_back(new Button("up", 340, 270, 35, 45, "Images/UI/CreateScreen/pointUpArrow.png", "dexterity", gRenderer));
	buttons.push_back(new Button("down", 340, 320, 35, 42, "Images/UI/CreateScreen/pointDownArrow.png", "dexterity", gRenderer));
	buttons.push_back(new Button("up", 340, 365, 35, 45, "Images/UI/CreateScreen/pointUpArrow.png", "constitution", gRenderer));
	buttons.push_back(new Button("down", 340, 415, 35, 42, "Images/UI/CreateScreen/pointDownArrow.png", "constitution", gRenderer));
	buttons.push_back(new Button("up", 340, 460, 35, 45, "Images/UI/CreateScreen/pointUpArrow.png", "faith", gRenderer));
	buttons.push_back(new Button("down", 340, 510, 35, 42, "Images/UI/CreateScreen/pointDownArrow.png", "faith", gRenderer));
	buttons.push_back(new Button("start", 450, 625, 230, 56, "Images/UI/CreateScreen/StartButton.png", "", gRenderer));

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
				if (e.key.keysym.sym == SDLK_BACKSPACE && nameInputText.length() > 0) {
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
			if (!i->pressed > 0 || i->attribute == "")
				SDL_RenderCopy(gRenderer, i->texture, NULL, &i->rect);
			else
			{
				if (i->type == "up")
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

void combatScene() {










}

void combatScene(std::vector<Character> combatants) {
	for (auto i : combatants) {
		std::cout << i.getName();
	}








}
void playGame() {

	Cluster enemy1 = Cluster(1);
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	player1.setTextureActive(player1.getTextureIdle());
	player1.currentMaxFrame = player1.getNumIdleAnimationFrames();
	
	enemy1.setTextureActive(enemy1.getTextureIdle());
	enemy1.currentMaxFrame = enemy1.getNumIdleAnimationFrames();

	std::vector<Character> charactersOnScreen;
	std::vector<Character> combatants;


	Uint32 timeSinceLastMovement = SDL_GetTicks();
	Uint32 timeSinceLastAnimation = SDL_GetTicks();
	player1.timeSinceLastMovement = timeSinceLastMovement;
	player1.timeSinceLastAnimation = timeSinceLastAnimation;
	enemy1.timeSinceLastAnimation = timeSinceLastAnimation;

	double timePassed = 0;

	charactersOnScreen.push_back(player1);
	charactersOnScreen.push_back(enemy1);

	SDL_Event e;
	bool inOverworld = true;
	bool keepPlaying = true;
	while (keepPlaying) {

		while (inOverworld) {
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					inOverworld = false;
					return;
				}
			}

			// figure out how much of a second has passed
			timePassed = (SDL_GetTicks() - timeSinceLastMovement) / 1000.0;
			player1.xDeltaVelocity = 0;
			player1.yDeltaVelocity = 0;


			const Uint8* keyState = SDL_GetKeyboardState(nullptr);
			if (keyState[SDL_SCANCODE_W])
				player1.yDeltaVelocity -= (player1.getAcceleration() * timePassed);
			if (keyState[SDL_SCANCODE_A])
				player1.xDeltaVelocity -= (player1.getAcceleration() * timePassed);
			if (keyState[SDL_SCANCODE_S])
				player1.yDeltaVelocity += (player1.getAcceleration() * timePassed);
			if (keyState[SDL_SCANCODE_D])
				player1.xDeltaVelocity += (player1.getAcceleration() * timePassed);

			if (player1.xDeltaVelocity == 0) {
				if (player1.xVelocity > 0)
					if (player1.xVelocity < (player1.getAcceleration() * timePassed))
						player1.xVelocity = 0;
					else
						player1.xVelocity -= (player1.getAcceleration() * timePassed);
				else if (player1.xVelocity < 0)
					if (-player1.xVelocity < (player1.getAcceleration() * timePassed))
						player1.xVelocity = 0;
					else
						player1.xVelocity += (player1.getAcceleration() * timePassed);
			}
			else {
				player1.xVelocity += player1.xDeltaVelocity;
			}

			if (player1.yDeltaVelocity == 0) {
				if (player1.yVelocity > 0)
					if (player1.yVelocity < (player1.getAcceleration() * timePassed))
						player1.yVelocity = 0;
					else
						player1.yVelocity -= (player1.getAcceleration() * timePassed);
				else if (player1.yVelocity < 0)
					if (-player1.yVelocity < (player1.getAcceleration() * timePassed))
						player1.yVelocity = 0;
					else
						player1.yVelocity += (player1.getAcceleration() * timePassed);
			}
			else {
				player1.yVelocity += player1.yDeltaVelocity;
			}

			//bound within Max Speed
			if (player1.xVelocity < -player1.getSpeedMax())
				player1.xVelocity = -player1.getSpeedMax();
			else if (player1.xVelocity > player1.getSpeedMax())
				player1.xVelocity = player1.getSpeedMax();
			//bound within Max Speed
			if (player1.yVelocity < -player1.getSpeedMax())
				player1.yVelocity = -player1.getSpeedMax();
			else if (player1.yVelocity > player1.getSpeedMax())
				player1.yVelocity = player1.getSpeedMax();

			//Change sprite if character is in motion
			if (player1.xVelocity != 0 || player1.yVelocity != 0) {
				if (player1.getTextureActive() != player1.getTextureRun()) {
					player1.setTextureActive(player1.getTextureRun());
					player1.currentFrame = 0;
					player1.currentMaxFrame = player1.getNumRunAnimationFrames();
				}
			}
			else {
				if (player1.getTextureActive() != player1.getTextureIdle()) {
					player1.setTextureActive(player1.getTextureIdle());
					player1.currentFrame = 0;
					player1.currentMaxFrame = player1.getNumIdleAnimationFrames();
				}
			}

			//Move vertically
			player1.yPosition += (player1.yVelocity * timePassed);
			if (player1.yPosition < 0 || (player1.yPosition + player1.getImageHeight() > SCREEN_HEIGHT)) {
				//go back into window
				player1.yPosition -= (player1.yVelocity * timePassed);
			}

			//Move horizontally
			player1.xPosition += (player1.xVelocity * timePassed);
			if (player1.xPosition < 0 || (player1.xPosition + player1.getImageWidth() > SCREEN_HEIGHT)) {
				//go back into window
				player1.xPosition -= (player1.xVelocity * timePassed);
			}

			timeSinceLastMovement = SDL_GetTicks();

			if (player1.xVelocity > 0 && flip == SDL_FLIP_HORIZONTAL)
				flip = SDL_FLIP_NONE;
			else if (player1.xVelocity < 0 && flip == SDL_FLIP_NONE)
				flip = SDL_FLIP_HORIZONTAL;

			//Set Black
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(gRenderer);

			if (player1.getTextureActive() == player1.getTextureIdle()) {
				if (SDL_GetTicks() - player1.timeSinceLastAnimation > player1.getTimeBetweenIdleAnimations()) {
					player1.currentFrame = (player1.currentFrame + 1) % player1.currentMaxFrame;
					player1.timeSinceLastAnimation = SDL_GetTicks();
				}
			}
			else {
				if (SDL_GetTicks() - player1.timeSinceLastAnimation > player1.getTimeBetweenRunAnimations()) {
					player1.currentFrame = (player1.currentFrame + 1) % player1.currentMaxFrame;
					player1.timeSinceLastAnimation = SDL_GetTicks();
				}
			}

			if (enemy1.getTextureActive() == enemy1.getTextureIdle()) {
				if (SDL_GetTicks() - enemy1.timeSinceLastAnimation > enemy1.getTimeBetweenIdleAnimations()) {
					enemy1.currentFrame = (enemy1.currentFrame + 1) % enemy1.currentMaxFrame;
					enemy1.timeSinceLastAnimation = SDL_GetTicks();
				}
			}

			// Start drawing clusters
			enemy1.drawRectangle.x = enemy1.currentFrame * enemy1.getPixelShiftAmountForAnimationInSpriteSheet();
			enemy1.rectangle.x = (int)enemy1.xPosition;
			enemy1.rectangle.y = (int)enemy1.yPosition;
			SDL_RenderCopy(gRenderer, enemy1.getTextureActive(), &enemy1.drawRectangle, &enemy1.rectangle);
			// Finish drawing clusters

			// Start drawing player
			player1.drawRectangle.x = player1.currentFrame * player1.getPixelShiftAmountForAnimationInSpriteSheet();
			player1.rectangle.x = (int)player1.xPosition;
			player1.rectangle.y = (int)player1.yPosition;
			SDL_RenderCopyEx(gRenderer, player1.getTextureActive(), &player1.drawRectangle, &player1.rectangle, 0.0, nullptr, flip);
			// Finish drawing player
			SDL_RenderPresent(gRenderer);

			if (check_collision(player1.rectangle, enemy1.rectangle)){
				combatants = enemy1.characterGroup;
				combatants.push_back(player1);
				inOverworld = false;
			}

		}

		while (!inOverworld) {
			combatTransition();
			combatScene(combatants);
			CombatManager cm;
			//convert combatants vector of characters to pointer of characters
			vector<Character *> c;
			for (Character i : combatants)
				c.push_back(&i);
			bool inCombat = cm.combatManager(c);
			enemy1.xPosition = 999;
			enemy1.yPosition = 999;
			inOverworld = true;
		}

		//while(gameOn) gameloop
		   //render top viewport: render player, enemy, overworld
		   //render bottom viewport: UI
		   //movement
		   //collision detection
		   //when player collides into enemy
		   // combatScene(vector of Type Characters);
	}
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
	/*CombatManager cm;
	vector<Character*> c;
	bool keepFighting = cm.combatManager(c);*/
	bool keepPlaying = characterCreateScreen();
	if (keepPlaying) {
		playGame();
		playCredits();
	}
	close();
	
	return 0;
}
