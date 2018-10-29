#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <cmath>
#include <fstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <cmath>
#include <fstream>
#include "Headers/Globals.h"


#include "Headers/mainGame.h"
#include "Headers/Player.h"
#include "Headers/Button.h"
#include "Headers/Tile.h"
#include "Headers/CombatManager.h"
#include "Headers/Cluster.h"
#include "Headers/LoadTexture.h"
#include "Headers/Globals.h"

// Function declarations

bool init();//Starts up SDL, creates window, and initializes OpenGL

void close();//Frees media and shuts down SDL


SDL_Texture* loadImage(std::string fname);

SDL_Renderer* gRenderer = nullptr;
// Globals

SDL_Window* gWindow = nullptr;//The window rendering to

//SDL_Renderer* gRenderer = nullptr;

SDL_GLContext gContext;//OpenGL context


std::vector<SDL_Texture*> gTex;
void handleMain();



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

    
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	
	// Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}

	//Create window
	gWindow = SDL_CreateWindow("CS1666-RPG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL| SDL_WINDOW_SHOWN);
	if (gWindow == nullptr) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	
	//set all the required Options for GLFW, Use OpenGL 3.2 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);//Double-buffering
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	//Create rendering context for OpenGL
	gContext = SDL_GL_CreateContext(gWindow);

	if (gContext == NULL)
	{
		std::cout << "OpenGL context could not be created! SDL Error: %s\n" << SDL_GetError() << std::endl;
		return false;
	}

	//Initialize GLEW
	glewExperimental = GL_TRUE; //use the new OpenGL functions and extensions
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
	}
    
	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
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

	
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	//draw OpenGL; 
	//SDL_GL_SwapWindow(SDL_Window* window)-> Use this function to update a window with OpenGL rendering.
	
	
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
SDL_Rect * loadMap(Tile* tiles[]) {
	Tile::loadTiles();
	bool tilesLoaded = true;
	int x = 0, y = 0;
	std::vector<SDL_Rect> blockedTiles;
	std::ifstream map("map1.txt");
	if (!map.is_open())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else {
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if ((tileType >= 0) && (tileType != 0 || tileType != 1))
			{
				tiles[i] = new Tile(x, y, tileType);
				if (tiles[i]->solid = true)
					blockedTiles.push_back(tiles[i]->getBox());
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}
			x += TILE_WIDTH;

			//If we've gone too far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}
	}
	return &blockedTiles[0];
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
	
	SDL_GL_DeleteContext(gContext);
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
	int charImageW = 128;
	int charImageH = 128;
	int charAnimationPixelShift = 128;
	int delaysPerFrame = 0;
	int frame = 0;

	SDL_Rect characterBox = { 472, 225, 128, 128 };
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
	SDL_Texture* upLocked = loadImage("Images/UI/CreateScreen/pointUpArrow_Locked.png");
	SDL_Texture* downLocked = loadImage("Images/UI/CreateScreen/pointDownArrow_Locked.png");
	SDL_Texture* upUnLocked = loadImage("Images/UI/CreateScreen/pointUpArrow.png");
	SDL_Texture* downUnLocked = loadImage("Images/UI/CreateScreen/pointUpArrow.png");

	SDL_Texture* character = loadImage("Images/Player/Player_Idle.png");


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

	LoadTexture background; 
	background.loadFromFile("Images/UI/CreateScreen/characterCreateV2NoButtons.png",gRenderer);
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
							if (nameInputText == "nlf4" || pointsToAllocate == 0) {
								if (nameInputText == "nlf4" || nameInputText != "") {
									Mix_PlayChannel(-1, gBSound, 0);
									onCharacterCreate = false;
									if (nameInputText == "nlf4")
										player1 = Player::Player(nameInputText, 10, 10, 10, 10, 10);//player1.setAll(nameInputText, 10, 10, 10, 10, 10);
									else
										player1 = Player(nameInputText, strength, intelligence, dexterity, constitution, faith);//player1.setAll(nameInputText, strength, intelligence, dexterity, constitution, faith);
									std::cout << std::string(player1); //displays player 1
									//make Character Object, validate, return to main
									for (auto i : buttons) {
										delete(i);
									}
									background.free();
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

		background.renderBackground(gRenderer);
		//Renders buttons and shows pressed image if pressed
		for (auto i : buttons) {
			if (i->attribute == "strength") {
				if (i->type == "up") {
					if (strength == maxStat) {
						i->locked = true;
					}
					else i->locked = false;
				}
				else {
					if (strength == minStat) {
						i->locked = true;
					}
					else i->locked = false;
				}
			}
			else if (i->attribute == "intelligence") {
				if (i->type == "up") {
					if (intelligence == maxStat) {
						i->locked = true;
					}
					else i->locked = false;
				}
				else {
					if (intelligence == minStat) {
						i->locked = true;
					}
					else i->locked = false;
				}
			}
			else if (i->attribute == "dexterity") {
				if (i->type == "up") {
					if (dexterity == maxStat) {
						i->locked = true;
					}
					else i->locked = false;
				}
				else {
					if (dexterity == minStat) {
						i->locked = true;
					}
					else i->locked = false;
				}
			}
			else if (i->attribute == "constitution") {
				if (i->type == "up") {
					if (constitution == maxStat) {
						i->locked = true;
					}
					else i->locked = false;
				}
				else {
					if (constitution == minStat) {
						i->locked = true;
					}
					else i->locked = false;
				}
			}
			else if (i->attribute == "faith") {
				if (i->type == "up") {
					if (faith == maxStat) {
						i->locked = true;
					}
					else i->locked = false;
				}
				else {
					if (faith == minStat) {
						i->locked = true;
					}
					else i->locked = false;
				}
			}
			if (!i->locked) {
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
			else {
				
				if (i->type == "up")
					SDL_RenderCopy(gRenderer, upLocked, NULL, &i->rect);
				else
					SDL_RenderCopy(gRenderer, downLocked, NULL, &i->rect);
			}
		}


		charImageX = frame * charAnimationPixelShift;


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

void saveGame() {

}

int handlePauseMenu(bool inPauseMenu, std::vector<Character*> charactersOnScreen, Tile *tiles[900], SDL_Rect camera) {
	std::vector<Button*> buttons;
	buttons.push_back(new Button("continue", 240, 100, 260, 64, "Images/UI/PauseMenu/ContinueButton.png", "", gRenderer));
	buttons.push_back(new Button("save",     240, 300, 260, 64, "Images/UI/PauseMenu/SaveButton.png", "", gRenderer));
	buttons.push_back(new Button("mainMenu", 240, 500, 260, 64, "Images/UI/PauseMenu/MainMenuButton.png", "", gRenderer));
	SDL_Texture* background = loadImage("Images/UI/PauseMenu/PauseMenuNoButtons.png"); 

	int backToOverworld = 0;
	int backToMainMenu = 1;

	SDL_Event e;
	while (inPauseMenu) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				inPauseMenu = false;
				return backToOverworld;
			}

			if (e.button.button == (SDL_BUTTON_LEFT) && e.type == SDL_MOUSEBUTTONDOWN) {
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				for (auto i : buttons) {
					//if mouse is clicked inside a button
					if (((mouseX >= i->x) && (mouseX <= (i->x + i->w))) &&
						((mouseY >= i->y) && (mouseY <= (i->y + i->h))))
					{
						if (i->type == "continue") {
							for (auto i : buttons) {
								delete(i);
							}
							SDL_DestroyTexture(background);
							inPauseMenu = false;
							return backToOverworld;
						}
						else if (i->type == "save") {
							saveGame();
						}
						else if (i->type == "mainMenu") {
							for (auto i : buttons) {
								delete(i);
							}
							SDL_DestroyTexture(background);
							inPauseMenu = false;
							return backToMainMenu;
						}

					}

				}

			}
			
		}
		//Set Black
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		for (int i = 0; i < 900; i++) {
			tiles[i]->render(&camera);
		}

		for (auto &i : charactersOnScreen) {
			if (i->xVelocity > 0 && i->flip == SDL_FLIP_HORIZONTAL)
				i->flip = SDL_FLIP_NONE;
			else if (i->xVelocity < 0 && i->flip == SDL_FLIP_NONE)
				i->flip = SDL_FLIP_HORIZONTAL;

			if (i->getTextureActive() == i->getTextureIdle()) {
				if (SDL_GetTicks() - i->timeSinceLastAnimation > i->getTimeBetweenIdleAnimations()) {
					i->currentFrame = (i->currentFrame + 1) % i->currentMaxFrame;
					i->timeSinceLastAnimation = SDL_GetTicks();
				}
			}
			else {
				if (SDL_GetTicks() - i->timeSinceLastAnimation > i->getTimeBetweenRunAnimations()) {
					i->currentFrame = (i->currentFrame + 1) % i->currentMaxFrame;
					i->timeSinceLastAnimation = SDL_GetTicks();
				}
			}

			i->drawRectangle.x = i->currentFrame *i->getPixelShiftAmountForAnimationInSpriteSheet();
			i->rectangle.x = (int)i->xPosition - camera.x;
			i->rectangle.y = (int)i->yPosition - camera.y;
			SDL_RenderCopyEx(gRenderer, i->getTextureActive(), &i->drawRectangle, &i->rectangle, 0.0, nullptr, i->flip);
		}

		//SDL_RenderCopy(gRenderer, background, NULL, NULL);
		for (auto &i : buttons) {
			SDL_RenderCopy(gRenderer, i->texture, NULL, &i->rect);
		}

		SDL_RenderPresent(gRenderer);
		SDL_Delay(16);
	}
}

void combatScene(std::vector<Character*> combatants) {
	/*for (auto i : combatants) {
		std::cout << i->getName();
	}*/

}
void playGame() {

	Cluster enemy1 = Cluster(1);
	//SDL_RendererFlip flip = SDL_FLIP_NONE;

	player1.setTextureActive(player1.getTextureIdle());
	player1.currentMaxFrame = player1.getNumIdleAnimationFrames();

	enemy1.setTextureActive(enemy1.getTextureIdle());
	enemy1.currentMaxFrame = enemy1.getNumIdleAnimationFrames();

	// Randomly spawn the enemy
	enemy1.xPosition = player1.xPosition + 150;//rand() % (LEVEL_WIDTH - enemy1.getImageWidth());
	enemy1.yPosition = player1.yPosition + 150; //rand() % (LEVEL_HEIGHT - enemy1.getImageHeight());

	std::vector<Character*> charactersOnScreen;
	std::vector<Character*> combatants;


	Uint32 timeSinceLastMovement = SDL_GetTicks();
	Uint32 timeSinceLastAnimation = SDL_GetTicks();
	player1.timeSinceLastMovement = timeSinceLastMovement;
	player1.timeSinceLastAnimation = timeSinceLastAnimation;
	enemy1.timeSinceLastAnimation = timeSinceLastAnimation;

	std::string hudHealthString = "Health: " + to_string(player1.getHPCurrent());
	std::string hudLevelString = "Level: " + to_string(player1.getLevel());
	SDL_Rect hudHealthTextRectangle = { 10, 10, 0, 0 };
	SDL_Rect hudLevelTextRectangle = { 10, 35, 0, 0 };
	SDL_Color hudTextColor = { 0, 0, 0, 0 };

	double timePassed = 0;
	int response = 0;

	charactersOnScreen.push_back(&player1);
	charactersOnScreen.push_back(&enemy1);

	Tile*  tiles[TOTAL_TILES];

	//tiles
	//Need to delete this to stop memory leak if we load more than one map
	SDL_Rect* BlockedTiles = loadMap(tiles);
	std::cout << player1.xPosition;
	std::cout << "\n";
	std::cout << player1.yPosition;


	SDL_Event e;
	SDL_Rect camera = { 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
	bool inOverworld = true;
	bool combatStarted = false;
	bool inPauseMenu = false;
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
			double runingAddSpeed = 0;

			const Uint8* keyState = SDL_GetKeyboardState(nullptr);
			if (keyState[SDL_SCANCODE_W])
				player1.yDeltaVelocity -= (player1.getAcceleration() * timePassed);
			if (keyState[SDL_SCANCODE_A])
				player1.xDeltaVelocity -= (player1.getAcceleration() * timePassed);
			if (keyState[SDL_SCANCODE_S])
				player1.yDeltaVelocity += (player1.getAcceleration() * timePassed);
			if (keyState[SDL_SCANCODE_D])
				player1.xDeltaVelocity += (player1.getAcceleration() * timePassed);
			if (keyState[SDL_SCANCODE_LSHIFT])
				runingAddSpeed = 200;


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
			if (player1.xVelocity < -(player1.getSpeedMax() + runingAddSpeed))
				player1.xVelocity = -(player1.getSpeedMax() + runingAddSpeed);
			else if (player1.xVelocity > (player1.getSpeedMax() + runingAddSpeed))
				player1.xVelocity = (player1.getSpeedMax() + runingAddSpeed);
			//bound within Max Speed
			if (player1.yVelocity < -(player1.getSpeedMax() + runingAddSpeed))
				player1.yVelocity = -(player1.getSpeedMax() + runingAddSpeed);
			else if (player1.yVelocity > (player1.getSpeedMax() + runingAddSpeed))
				player1.yVelocity = (player1.getSpeedMax() + runingAddSpeed);

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
			if (player1.yPosition < 0 || (player1.yPosition + player1.getImageHeight() > LEVEL_HEIGHT)) {
				//go back into window
				player1.yPosition -= (player1.yVelocity * timePassed);
			}

			//Move horizontally
			player1.xPosition += (player1.xVelocity * timePassed);
			if (player1.xPosition < 0 || (player1.xPosition + player1.getImageWidth() > LEVEL_WIDTH)) {
				//go back into window
				player1.xPosition -= (player1.xVelocity * timePassed);
			}
			//calculate tile player is currently standing on
			int currentTile = (int)(player1.xPosition + (player1.rectangle.w / 2)) / TILE_WIDTH;
			currentTile += (int)((player1.yPosition + player1.rectangle.h) / TILE_HEIGHT) * 30;


			if (tiles[currentTile]->mType != 0) {
				//toDo
				bool xVelPos = player1.xVelocity > 0;
				bool xVelNeg = player1.xVelocity < 0;
				bool yVelPos = player1.yVelocity > 0;
				bool yVelNeg = player1.yVelocity < 0;
				player1.xVelocity = 0;
				player1.yVelocity = 0;
				if (xVelPos)
					player1.xPosition -= 1;
				if (xVelNeg)
					player1.xPosition += 1;
				if (yVelNeg)
					player1.yPosition += 1;
				if (yVelPos)
					player1.yPosition -= 1;

				//player1.xPosition -= 1;
				//player1.yPosition -= 1;
			}




			camera.x = (player1.xPosition + player1.rectangle.w / 2) - SCREEN_WIDTH / 2;
			camera.y = (player1.yPosition + player1.rectangle.h / 2) - SCREEN_HEIGHT / 2;
			if (camera.x < 0) {
				camera.x = 0;
			}
			if (camera.y < 0) {
				camera.y = 0;
			}
			if (camera.x > LEVEL_WIDTH - camera.w) {
				camera.x = LEVEL_WIDTH - camera.w;
			}
			if (camera.y > LEVEL_HEIGHT - camera.h) {
				camera.y = LEVEL_HEIGHT - camera.h;
			}

			timeSinceLastMovement = SDL_GetTicks();

			//Set Black
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			for (int i = 0; i < 900; i++) {
				tiles[i]->render(&camera);
			}

			for (auto &i : charactersOnScreen) {
				if (i->xVelocity > 0 && i->flip == SDL_FLIP_HORIZONTAL)
					i->flip = SDL_FLIP_NONE;
				else if (i->xVelocity < 0 && i->flip == SDL_FLIP_NONE)
					i->flip = SDL_FLIP_HORIZONTAL;

				if (i->getTextureActive() == i->getTextureIdle()) {
					if (SDL_GetTicks() - i->timeSinceLastAnimation > i->getTimeBetweenIdleAnimations()) {
						i->currentFrame = (i->currentFrame + 1) % i->currentMaxFrame;
						i->timeSinceLastAnimation = SDL_GetTicks();
					}
				}
				else {
					if (SDL_GetTicks() - i->timeSinceLastAnimation > i->getTimeBetweenRunAnimations()) {
						i->currentFrame = (i->currentFrame + 1) % i->currentMaxFrame;
						i->timeSinceLastAnimation = SDL_GetTicks();
					}
				}

				i->drawRectangle.x = i->currentFrame *i->getPixelShiftAmountForAnimationInSpriteSheet();
				i->rectangle.x = (int)i->xPosition - camera.x;
				i->rectangle.y = (int)i->yPosition - camera.y;
				SDL_RenderCopyEx(gRenderer, i->getTextureActive(), &i->drawRectangle, &i->rectangle, 0.0, nullptr, i->flip);
			}

			hudHealthString = "Health: " + to_string(player1.getHPCurrent());
			hudLevelString = "Level: " + to_string(player1.getLevel());
			renderText(hudHealthString.c_str(), &hudHealthTextRectangle, &hudTextColor);
			renderText(hudLevelString.c_str(), &hudLevelTextRectangle, &hudTextColor);

			SDL_RenderPresent(gRenderer);

			if (keyState[SDL_SCANCODE_ESCAPE]) {
				inOverworld = false;
				inPauseMenu = true;
			}

			if (check_collision(player1.rectangle, enemy1.rectangle)) {
				combatants.clear();
				combatants.push_back(&player1);
				for (auto i : enemy1.characterGroup)
				{
					combatants.push_back(i);
				}
				inOverworld = false;
				combatStarted = true;
			}

		}

		if (inPauseMenu) {
			response = handlePauseMenu(inPauseMenu, charactersOnScreen, tiles, camera);
			int backToOverWorld = 0;
			int backToMainMenu = 1;
			if (response == backToOverWorld) {
				inOverworld = true;
			}
			else if (response == backToMainMenu) {
				keepPlaying = false;
			}
			inPauseMenu = false;
		}

		while (combatStarted) {
			combatTransition();
			CombatManager cm;
			//std::cout << combatants.size();
			//convert combatants vector of characters to pointer of characters
			//vector<Character *> c;
			//for (auto i : combatants)
				//c.push_back(&i);
			bool inCombat = cm.combatMain(combatants);
			enemy1.xPosition = rand() % (LEVEL_WIDTH - enemy1.getImageWidth());
			enemy1.yPosition = rand() % (LEVEL_HEIGHT - enemy1.getImageHeight());
			inOverworld = true;
		}

		if (response == 1) { //backToMainMenu
			//for (auto i : charactersOnScreen) {
			//	delete(&i);
			//}
			//destroy tiles etc?
			handleMain();
		}
	}
}

void printProgramLog(GLuint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}

void printShaderLog(GLuint shader)
{
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}
/*
if return...
-1 - SDL_QUIT
0 - character creation screen
1 - farnan memes (credits)
2 - load game (currently inactive)
*/

int mainMenu() {

	bool run = true;
	std::vector<Button*> buttons;

	SDL_Texture* start = loadImage("Images/UI/MainMenu/StartButton.png");
	SDL_Texture* credits = loadImage("Images/UI/MainMenu/CreditsButton.png");
	SDL_Texture* load = loadImage("Images/UI/MainMenu/NewButton.png");
	SDL_Texture* title = loadImage("Images/UI/MainMenu/title.png");
	SDL_Rect space = { 100, 50, 526, 72 };
	//need attr objects
	buttons.push_back(new Button("start", 240, 200, 240, 64, "Images/UI/MainMenu/StartButton.png", "", gRenderer));
	buttons.push_back(new Button("credits", 240, 350, 240, 64, "Images/UI/MainMenu/CreditsButton.png", "", gRenderer));
	buttons.push_back(new Button("load", 240, 500, 240, 64, "Images/UI/MainMenu/NewButton.png", "", gRenderer));

	SDL_Texture* background = loadImage("Images/UI/MainMenu/MainMenuNoButtons.png"); //Moved to fix memory leak

	SDL_Event e;
	while (run) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				close();
			}
			if (e.button.button == (SDL_BUTTON_LEFT) && e.type == SDL_MOUSEBUTTONDOWN) {
				std::cout << "button clicked";
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);

				for (auto i : buttons) {
					//if mouse is clicked inside a button
					if (((mouseX >= i->x) && (mouseX <= (i->x + i->w))) && ((mouseY >= i->y) && (mouseY <= (i->y + i->h)))) {
						if (i->type == "start") {
							for (auto i : buttons) {
								delete(i);
							}
							SDL_DestroyTexture(background);
							run = false;
							return 0;
						}
						else if (i->type == "credits") {
							for (auto i : buttons) {
								delete(i);
							}
							SDL_DestroyTexture(background);
							run = false;
							return 1;
						}
						else if (i->type == "load") {
							for (auto i : buttons) {
								delete(i);
							}
							SDL_DestroyTexture(background);
							run = false;
							return 2;
						}
						break;
					}
				}
			}
			SDL_RenderCopy(gRenderer, background, NULL, NULL);
			SDL_RenderCopy(gRenderer, title, NULL, &space);
			for (auto i : buttons) {
				if (i->type == "start") SDL_RenderCopy(gRenderer, start, NULL, &i->rect);
				else if (i->type == "credits") SDL_RenderCopy(gRenderer, credits, NULL, &i->rect);
				else if (i->type == "load")  SDL_RenderCopy(gRenderer, load, NULL, &i->rect);
			}
			SDL_RenderPresent(gRenderer);
			SDL_Delay(16);
		}
	}
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	/*
	CombatManager cm;
	std::vector<Character*> combatants;
	combatants.push_back(new Player("nlf4", 10, 10, 10, 10, 10));
	combatants.push_back(new Enemy("W.G.", 10, 10, 10, 5, 10));
	bool inCombat = cm.combatMain(combatants);
	//*/

	if (!init()) {
		std::cout << "Failed to initialize!" << std::endl;
		close();
		return 1;
	}
	handleMain();
}
void handleMain() {
	int a = mainMenu();
	bool b;

	switch (a) {
	case 0:
		b = characterCreateScreen();
		if (b) playGame();
		break;
	case 1:
		playCredits();
	}
	close();
	//*/
	return;
}
