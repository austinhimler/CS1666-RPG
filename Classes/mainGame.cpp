#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <cmath>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <cmath>
#include <fstream>
#include "../Headers/Globals.h"


#include <SDL2/SDL_net.h>

//CHCHCHCHCHANGES
#include "../Headers/mainGame.h"
#include "../Headers/Player.h"
#include "../Headers/Button.h"
#include "../Headers/Tile.h"
#include "../Headers/CombatManager.h"
#include "../Headers/Cluster.h"
#include "../Headers/LoadTexture.h"
#include "../Headers/Globals.h"

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

bool isHost;
bool isClient;
int port;
IPaddress ipAddress;
TCPsocket serverSocket;
TCPsocket clientSocket;

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;
const int ENEMIES_PER_CLUSTER = 1;
const int STARTING_ENEMIES = 1;
const vector<string> ALL_MAPS = { "map1.txt", "map2.txt", "map3.txt" };
int MAP_INDEX = 0;
//std::vector<SDL_Texture*> gTex;
// Music var
Mix_Music *gMusic = NULL;
Mix_Chunk *gBSound = NULL;
TTF_Font* font;

//Player ONE
Player* player1 = new Player("nlf4",1,1,1,1,1);

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
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);//Double-buffering
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


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
	font = TTF_OpenFont("Fonts/ka1.ttf", 20);
	if (font == NULL) {
		std::cout << "font was null";
	}

	SDLNet_Init();

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


SDL_Rect * loadMap(Tile* tiles[],string mapToLoad) {
	Tile::loadTiles();
	bool tilesLoaded = true;
	int x = 0, y = 0;
	std::vector<SDL_Rect> blockedTiles;
	std::ifstream map(mapToLoad);
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
		SDLNet_TCP_Close(clientSocket);
		SDLNet_TCP_Close(serverSocket);
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
			SDL_Delay(60);
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
void combatTransition() {
	//Load the music
	gMusic = Mix_LoadMUS("Audio/Into_Combat_Test.wav");
	if (gMusic == NULL)
		std::cout << "Failed to load music" << std::endl;
	//Play the music
	Mix_PlayMusic(gMusic, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
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
	Mix_PauseMusic();
}
void levelTransition() {
	SDL_Rect wholeS = { 0,0,720,720 };
	SDL_Rect words = { 220,200,120,60 };
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderFillRect(gRenderer, &wholeS);
	SDL_RenderPresent(gRenderer);
	string level = "Loading Next Level";
	SDL_Color TextColor = { 255, 255, 255, 0 };
	renderText(level.c_str(), &words, &TextColor);
	SDL_RenderPresent(gRenderer);
	SDL_Rect wipe = { 180,240,20,20 };
	SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
	for (; wipe.x < 540; wipe.x += 20)
	{
		SDL_RenderFillRect(gRenderer, &wipe);
		SDL_RenderPresent(gRenderer);
		SDL_Delay(100);
	}
}
void EndTransition() {
	gMusic = Mix_LoadMUS("Audio/Victory_2_Test.wav");
	if (gMusic == NULL)
		std::cout << "Failed to load music" << std::endl;
	//Play the music
	Mix_PlayMusic(gMusic, -1);
	SDL_Rect wholeS = { 0,0,720,720 };
	SDL_Rect word1 = { 220,200,120,60 };
	SDL_Rect word2 = { 30, 240,120,60};
	SDL_Rect word3 = { 260, 280,120,60 };
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderFillRect(gRenderer, &wholeS);
	SDL_RenderPresent(gRenderer);
	string line1 = "Congratulations!";
	string line2 = "You have successfully completed The Game!";
	string line3 = "Cya Nerd.";
	SDL_Color TextColor = { 255, 255, 255, 0 };
	renderText(line1.c_str(), &word1, &TextColor);
	renderText(line2.c_str(), &word2, &TextColor);
	renderText(line3.c_str(), &word3, &TextColor);
	SDL_RenderPresent(gRenderer);
	SDL_Rect wipe = { 180,240,20,20 };
	SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
	SDL_Delay(200);
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
	int charImageW = 144;
	int charImageH = 144;
	int charAnimationPixelShift = 144;
	int delaysPerFrame = 0;
	int frame = 0;
	
	int attributeX = 245;

	SDL_Rect characterBox = { 470, 225, 144, 144 };
	SDL_Rect pointsAllocatedRectangle = { 227, 32, 0, 0 };
	SDL_Rect strengthTextRectangle = { attributeX, 115, 0, 0 };
	SDL_Rect intelligenceTextRectangle = { attributeX, 205, 0, 0 };
	SDL_Rect dexterityTextRectangle = { attributeX, 302, 0, 0 };
	SDL_Rect constitutionTextRectangle = { attributeX, 395, 0, 0 };
	SDL_Rect faithTextRectangle = { attributeX, 490, 0, 0 };
	SDL_Rect nameTextRectangle = { 143, 640, 0,0 };
	SDL_Rect errorTextRectangle = { 465, 580, 0, 0 };
	SDL_Rect errorTextRectangleLong = { 415, 580, 0, 0 };
	SDL_Color textColor = { 112, 96, 80, 0 };
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

	SDL_Texture* character = loadImage("Images/Player/Idle_Down.png");


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
						//i->locked = false;
						if (i->type == "start") {
							if (nameInputText == "nlf4" || nameInputText == "nfl4" || pointsToAllocate == 0) {
								if (nameInputText == "nlf4" || nameInputText == "nfl4" || nameInputText != "") {
									Mix_PlayChannel(-1, gBSound, 0);
									onCharacterCreate = false;
									if (nameInputText == "nfl4" || nameInputText == "nlf4")
										player1 = new Player(nameInputText, 10, 10, 10, 10, 10);//player1->setAll(nameInputText, 10, 10, 10, 10, 10);
									else
										player1 = new Player(nameInputText, strength, intelligence, dexterity, constitution, faith);//player1->setAll(nameInputText, strength, intelligence, dexterity, constitution, faith);
									std::cout << std::string(*player1); //displays player 1
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
				//Move on by pressing enter
				else if (e.key.keysym.sym == SDLK_RETURN) {
						if (nameInputText == "nfl4" || nameInputText == "nlf4" || pointsToAllocate == 0) {
							if (nameInputText != "") {
								Mix_PlayChannel(-1, gBSound, 0);
								onCharacterCreate = false;
								if (nameInputText == "nfl4" || nameInputText == "nlf4")
									player1 = new Player(nameInputText, 10, 10, 10, 10, 10);//player1->setAll(nameInputText, 10, 10, 10, 10, 10);
								else
									player1 = new Player(nameInputText, strength, intelligence, dexterity, constitution, faith);//player1->setAll(nameInputText, strength, intelligence, dexterity, constitution, faith);
								std::cout << std::string(*player1); //displays player 1
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
			}
			else if (e.type == SDL_TEXTINPUT) {
				//add char
				//set length limit to arbitrariy be 11 (fits textbox about right, depends on what user enters)
				if (nameInputText.length() < 11) {
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
/*
move location of Cluster cl

a indicates movement type
up/down/left/right/random
*/
void moveCluster(std::vector<Cluster*> c, std::string move, double time, Tile* map[900], int cycle) {

	for (auto cl : c) {
		if (cl->getTextureActive() != cl->getTextureIdleNotReady()) {
			int a = -1;
			if (move == "up") a = 0;
			else if (move == "left") a = 1;
			else if (move == "down") a = 2;
			else if (move == "right") a = 3;
			else if (move == "random" && cycle == 0) {
				a = rand() % 4;
			}
			else if (move == "random" && cycle % 100 == 0) {
				a = rand() % 4;
			}
			else if (move == "random" && cycle % 100 != 0) {
				a = cl->lastDirection;
			}
			cl->lastDirection = a;
			cl->xDeltaVelocity = 0;
			cl->yDeltaVelocity = 0;
			//std::cout << move << std::endl;
			/*
			std::cout << "CLuster" << std::endl;
			std::cout << cl->xPosition << std::endl;
			std::cout << cl->yPosition << std::endl;
			//std::cout << a << std::endl;
			*/

			if (a == 0) {
				cl->yDeltaVelocity -= (cl->getAcceleration() * time);
			}
			else if (a == 1) {
				cl->xDeltaVelocity -= (cl->getAcceleration() * time);
			}
			else if (a == 2) {
				cl->yDeltaVelocity += (cl->getAcceleration() * time);
			}
			else if (a == 3) {
				cl->xDeltaVelocity += (cl->getAcceleration() * time);
			}

			if (cl->xDeltaVelocity == 0) {
				if (cl->xVelocity > 0) {
					if (cl->xVelocity < (cl->getAcceleration() * time))
						cl->xVelocity = 0;
					else
						cl->xVelocity -= cl->getAcceleration() * time;
				}
				else if (cl->xVelocity < 0) {
					if (-cl->xVelocity < (cl->getAcceleration() * time))
						cl->xVelocity = 0;
					else
						cl->xVelocity += cl->getAcceleration() * time;
				}
			}
			else {
				cl->xVelocity += cl->xDeltaVelocity;
			}
			if (cl->yDeltaVelocity == 0) {
				if (cl->yVelocity > 0) {
					if (cl->yVelocity < (cl->getAcceleration() * time))
						cl->yVelocity = 0;
					else
						cl->yVelocity -= cl->getAcceleration() * time;
				}
				else if (cl->yVelocity < 0) {
					if (-cl->yVelocity < (cl->getAcceleration() * time))
						cl->yVelocity = 0;
					else
						cl->yVelocity += cl->getAcceleration() * time;
				}
			}
			else {
				cl->yVelocity += cl->yDeltaVelocity;
			}

			if (cl->xVelocity < -cl->getSpeedMax())
				cl->xVelocity = -cl->getSpeedMax();
			else if (cl->xVelocity > cl->getSpeedMax())
				cl->xVelocity = cl->getSpeedMax();
			if (cl->yVelocity < -cl->getSpeedMax())
				cl->yVelocity = -cl->getSpeedMax();
			else if (cl->yVelocity > cl->getSpeedMax()) cl->yVelocity = cl->getSpeedMax();

			if (cl->xVelocity != 0 || cl->yVelocity != 0) {

				if (cl->yVelocity == 0) {
					if (cl->getTextureActive() != cl->getTextureRun()) {
						cl->setTextureActive(cl->getTextureRun());
						cl->currentFrame = 0;
						cl->currentMaxFrame = cl->getNumRunAnimationFrames();
					}
				}


				if (cl->xVelocity == 0 && cl->yVelocity > 0) {
					if (cl->getTextureActive() != cl->getTextureDownRun()) {
						cl->setTextureActive(cl->getTextureDownRun());
						cl->currentFrame = 0;
						cl->currentMaxFrame = cl->getNumRunAnimationFrames();
					}
				}



				if (cl->xVelocity != 0 && cl->yVelocity > 0) {
					if (cl->getTextureActive() != cl->getTextureDownRightRun()) {
						cl->setTextureActive(cl->getTextureDownRightRun());
						cl->currentFrame = 0;
						cl->currentMaxFrame = cl->getNumRunAnimationFrames();
					}
				}

				if (cl->xVelocity != 0 && cl->yVelocity < 0) {
					if (cl->getTextureActive() != cl->getTextureUpRightRun()) {
						cl->setTextureActive(cl->getTextureUpRightRun());
						cl->currentFrame = 0;
						cl->currentMaxFrame = cl->getNumRunAnimationFrames();
					}
				}



				if (cl->xVelocity == 0 && cl->yVelocity < 0) {
					if (cl->getTextureActive() != cl->getTextureUpRun()) {
						cl->setTextureActive(cl->getTextureUpRun());
						cl->currentFrame = 0;
						cl->currentMaxFrame = cl->getNumRunAnimationFrames();

					}
				}
			}

			else {
				if (cl->getTextureActive() != cl->getTextureIdle()) {
					cl->setTextureActive(cl->getTextureIdle());
					cl->currentFrame = 0;
					cl->currentMaxFrame = cl->getNumIdleAnimationFrames();
				}
			}

			int beforeMoveX = (int)cl->xPosition;
			int beforeMoveY = (int)cl->yPosition;
			cl->yPosition += (cl->yVelocity * time);
			if (cl->yPosition < 0 || (cl->yPosition + cl->getImageHeight() > LEVEL_HEIGHT)) {
				cl->yPosition -= (cl->yVelocity * time);
			}
			cl->xPosition += (cl->xVelocity * time);
			if (cl->xPosition < 0 || (cl->xPosition + cl->getImageWidth() > LEVEL_WIDTH)) {
				cl->xPosition -= (cl->xVelocity * time);
			}
			int current = (int)(cl->xPosition + cl->rectangle.w / 2) / TILE_WIDTH;
			current += (int)((cl->yPosition + cl->rectangle.h) / TILE_HEIGHT) * 30;


			//std::cout << cl->xPosition << std::endl;
			//std::cout << cl->yPosition << std::endl;


			if (map[current]->mType != 0) {
				cl->xPosition = beforeMoveX;
				cl->yPosition = beforeMoveY;
			}

			for (auto i : c) {
				if (i != cl) {
					int other = (int)(i->xPosition + i->rectangle.w / 2) / TILE_WIDTH;
					other += (int)((i->yPosition + i->rectangle.h) / TILE_HEIGHT) * 30;
					if (other == current) {
						cl->xPosition = beforeMoveX;
						cl->yPosition = beforeMoveY;
					}
				}
			}
			//std::cout << cl->xPosition << std::endl;
			//std::cout << cl->yPosition << std::endl;
		}
	}
}

void combatScene(std::vector<Character*> combatants) {
	/*for (auto i : combatants) {
		std::cout << i->getName();
	}*/

}


bool handleNetworkingSetup() {
	bool waitForInput = true;
	while (waitForInput) {
		std::cout << "Do you want to do Networking? 'y' or 'n'" << std::endl;
		std::string networkingAnswer;
		std::getline(std::cin, networkingAnswer);

		if (networkingAnswer == "y") {

		}
		else {
			return false;
		}

		std::cout << "Are you host or client? Type 'h' for host 'c' for client?" << std::endl;
		std::string hostOrClientInput;
		std::getline(std::cin, hostOrClientInput);
		if (hostOrClientInput == "h") {
			isHost = true;
			isClient = false;
		}
		else if (hostOrClientInput == "c") {
			isHost = false;
			isClient = true;
		}
		else {
			break;
		}


		if (isClient) {
			std::cout << "Enter IP address:\n" << std::endl;
			std::string ipInput;
			std::getline(std::cin, ipInput);

			std::cout << "Enter Port:\n" << std::endl;
			std::cin >> port;

			SDLNet_ResolveHost(&ipAddress, ipInput.c_str(), port);
			

			clientSocket = NULL;
			while (clientSocket == NULL)
			{
				clientSocket = SDLNet_TCP_Open(&ipAddress);
			}

			std::cout << clientSocket << std::endl;
			std::cout << ipInput.c_str() << std::endl;
			std::cout << ipAddress.host << std::endl;
			std::cout << ipAddress.port << std::endl;
		
		}

		if (isHost) {
			std::cout << "Enter Port:\n" << std::endl;
			std::cin >> port;

			SDLNet_ResolveHost(&ipAddress, NULL, port);
			serverSocket = SDLNet_TCP_Open(&ipAddress);
			bool noClient = true;
			while (noClient)
			{
				//waits for a client to connect
				clientSocket = SDLNet_TCP_Accept(serverSocket);
				if (clientSocket)
				{
					noClient = false;
				}
			}
			std::cout << serverSocket << std::endl;
			std::cout << ipAddress.host << std::endl;
			std::cout << ipAddress.port << std::endl;
		}

		return true;
	}

}

void playGame() {

	bool doNetworking = handleNetworkingSetup();

	//Load the music
	gMusic = Mix_LoadMUS("Audio/Walking_Test.wav");
	if (gMusic == NULL)
		std::cout << "Failed to load music" << std::endl;
	//Play the music
	Mix_PlayMusic(gMusic, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
	for (MAP_INDEX = 0; MAP_INDEX < ALL_MAPS.size(); MAP_INDEX++)
	{
		vector<Cluster*> allEnemies = vector<Cluster*>();
		Cluster* CollidingCluster;
		for (int num_enemy = 0; num_enemy < STARTING_ENEMIES * (MAP_INDEX + 1); num_enemy++)
		{
			Cluster* enemy = new Cluster((rand() % (ENEMIES_PER_CLUSTER + MAP_INDEX)) + 1);
			cout << "Enemy " << num_enemy + 1 << " Cluster Size: " << enemy->clusterSize << endl;
			allEnemies.push_back(enemy);
		}

		//SDL_RendererFlip flip = SDL_FLIP_NONE;

		int tile_test = -1;

		player1->setTextureActive(player1->getTextureIdle());
		player1->currentMaxFrame = player1->getNumIdleAnimationFrames();


		Tile*  tiles[TOTAL_TILES];

		//tiles
		//Need to delete this to stop memory leak if we load more than one map
		SDL_Rect* BlockedTiles = loadMap(tiles, ALL_MAPS.at(MAP_INDEX));
		for (;;)
		{
			int t_tile = (int)(player1->xPosition + (player1->rectangle.w / 2)) / TILE_WIDTH;
			t_tile += (int)((player1->yPosition + player1->rectangle.h) / TILE_HEIGHT) * 30;
			if (tiles[t_tile]->mType == 0)
				break;
			player1->xPosition = rand() % (LEVEL_WIDTH - player1->getImageWidth());
			player1->yPosition = rand() % (LEVEL_HEIGHT - player1->getImageHeight());
		}

		for (auto i : allEnemies)
		{
			i->setTextureActive(i->getTextureIdle());
			i->currentMaxFrame = i->getNumIdleAnimationFrames();
			// Randomly spawn the enemy
			for (;;)
			{
				i->xPosition = rand() % (LEVEL_WIDTH - (2 * i->getImageWidth()));
				i->yPosition = rand() % (LEVEL_HEIGHT - (2 * i->getImageHeight()));
				int t_tile = (int)(i->xPosition + (i->rectangle.w / 2)) / TILE_WIDTH;
				t_tile += (int)((i->yPosition + i->rectangle.h) / TILE_HEIGHT) * 30;
				if (tiles[t_tile]->mType == 0)
					break;
			}
		}

		std::vector<Character*> charactersOnScreen;
		std::vector<Character*> combatants;


		Uint32 timeSinceLastMovement = SDL_GetTicks();
		Uint32 timeSinceLastAnimation = SDL_GetTicks();
		player1->timeSinceLastMovement = timeSinceLastMovement;
		player1->timeSinceLastAnimation = timeSinceLastAnimation;
		for (auto i : allEnemies)
		{
			cout << "Enemy Coordinates: (" << i->xPosition << "," << i->yPosition << ")" << endl;
			i->timeSinceLastAnimation = timeSinceLastAnimation;
		}
		std::string hudHealthString = "Health: " + to_string(player1->getHPCurrent());
		std::string hudLevelString = "Level: " + to_string(player1->getLevel());
		SDL_Rect hudHealthTextRectangle = { 10, 10, 0, 0 };
		SDL_Rect hudLevelTextRectangle = { 10, 35, 0, 0 };
		SDL_Color hudTextColor = { 0, 0, 0, 0 };

		double timePassed = 0;
		int response = 0;

		charactersOnScreen.push_back(player1);
		for (auto i : allEnemies)
		{
			charactersOnScreen.push_back(i);
		}


		std::cout << player1->xPosition;
		std::cout << "\n";
		std::cout << player1->yPosition;

		int cycle = 0;

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
				player1->xDeltaVelocity = 0;
				player1->yDeltaVelocity = 0;
				double runningAddSpeed = 0;

				const Uint8* keyState = SDL_GetKeyboardState(nullptr);
				if (keyState[SDL_SCANCODE_W])
					player1->yDeltaVelocity -= (player1->getAcceleration() * timePassed);
				if (keyState[SDL_SCANCODE_A])
					player1->xDeltaVelocity -= (player1->getAcceleration() * timePassed);
				if (keyState[SDL_SCANCODE_S])
					player1->yDeltaVelocity += (player1->getAcceleration() * timePassed);
				if (keyState[SDL_SCANCODE_D])
					player1->xDeltaVelocity += (player1->getAcceleration() * timePassed);
				if (keyState[SDL_SCANCODE_LSHIFT])
					runningAddSpeed = 200;


				if (player1->xDeltaVelocity == 0) {
					if (player1->xVelocity > 0)
						if (player1->xVelocity < (player1->getAcceleration() * timePassed))
							player1->xVelocity = 0;
						else
							player1->xVelocity -= (player1->getAcceleration() * timePassed);
					else if (player1->xVelocity < 0)
						if (-player1->xVelocity < (player1->getAcceleration() * timePassed))
							player1->xVelocity = 0;
						else
							player1->xVelocity += (player1->getAcceleration() * timePassed);
				}
				else {
					player1->xVelocity += player1->xDeltaVelocity;
				}

				if (player1->yDeltaVelocity == 0) {
					if (player1->yVelocity > 0)
						if (player1->yVelocity < (player1->getAcceleration() * timePassed))
							player1->yVelocity = 0;
						else
							player1->yVelocity -= (player1->getAcceleration() * timePassed);
					else if (player1->yVelocity < 0)
						if (-player1->yVelocity < (player1->getAcceleration() * timePassed))
							player1->yVelocity = 0;
						else
							player1->yVelocity += (player1->getAcceleration() * timePassed);
				}
				else {
					player1->yVelocity += player1->yDeltaVelocity;
				}

				//bound within Max Speed
				if (player1->xVelocity < -(player1->getSpeedMax() + runningAddSpeed))
					player1->xVelocity = -(player1->getSpeedMax() + runningAddSpeed);
				else if (player1->xVelocity > (player1->getSpeedMax() + runningAddSpeed))
					player1->xVelocity = (player1->getSpeedMax() + runningAddSpeed);
				//bound within Max Speed
				if (player1->yVelocity < -(player1->getSpeedMax() + runningAddSpeed))
					player1->yVelocity = -(player1->getSpeedMax() + runningAddSpeed);
				else if (player1->yVelocity > (player1->getSpeedMax() + runningAddSpeed))
					player1->yVelocity = (player1->getSpeedMax() + runningAddSpeed);

				//Change sprite if character is in motion
				if (player1->xVelocity != 0 || player1->yVelocity != 0) {

					if (player1->yVelocity == 0) {
						if (player1->getTextureActive() != player1->getTextureRun()) {
							player1->setTextureActive(player1->getTextureRun());
							player1->currentFrame = 0;
							player1->currentMaxFrame = player1->getNumRunAnimationFrames();
						}
					}


					if (player1->xVelocity == 0 && player1->yVelocity > 0) {
						if (player1->getTextureActive() != player1->getTextureDownRun()) {
							player1->setTextureActive(player1->getTextureDownRun());
							player1->currentFrame = 0;
							player1->currentMaxFrame = player1->getNumRunAnimationFrames();
						}
					}



					if (player1->xVelocity != 0 && player1->yVelocity > 0) {
						if (player1->getTextureActive() != player1->getTextureDownRightRun()) {
							player1->setTextureActive(player1->getTextureDownRightRun());
							player1->currentFrame = 0;
							player1->currentMaxFrame = player1->getNumRunAnimationFrames();
						}
					}

					if (player1->xVelocity != 0 && player1->yVelocity < 0) {
						if (player1->getTextureActive() != player1->getTextureUpRightRun()) {
							player1->setTextureActive(player1->getTextureUpRightRun());
							player1->currentFrame = 0;
							player1->currentMaxFrame = player1->getNumRunAnimationFrames();
						}
					}



					if (player1->xVelocity == 0 && player1->yVelocity < 0) {
						if (player1->getTextureActive() != player1->getTextureUpRun()) {
							player1->setTextureActive(player1->getTextureUpRun());
							player1->currentFrame = 0;
							player1->currentMaxFrame = player1->getNumRunAnimationFrames();

						}
					}
				}

				else {
					if (player1->getTextureActive() != player1->getTextureIdle()) {
						player1->setTextureActive(player1->getTextureIdle());
						player1->currentFrame = 0;
						player1->currentMaxFrame = player1->getNumIdleAnimationFrames();
					}
				}

				int beforeMoveX = player1->xPosition;
				int beforeMoveY = player1->yPosition;
				//Move vertically
				player1->yPosition += (player1->yVelocity * timePassed);
				if (player1->yPosition < 0 || (player1->yPosition + player1->getImageHeight() > LEVEL_HEIGHT)) {
					//go back into window
					player1->yPosition -= (player1->yVelocity * timePassed);
				}

				//Move horizontally
				player1->xPosition += (player1->xVelocity * timePassed);
				if (player1->xPosition < 0 || (player1->xPosition + player1->getImageWidth() > LEVEL_WIDTH)) {
					//go back into window
					player1->xPosition -= (player1->xVelocity * timePassed);
				}
				//calculate tile player is currently standing on
				int currentTile = (int)(player1->xPosition + (player1->rectangle.w / 2)) / TILE_WIDTH;
				currentTile += (int)((player1->yPosition + player1->rectangle.h) / TILE_HEIGHT) * 30;

				// Show which tile the character is standing on
				/*
				if (currentTile != tile_test) {
					cout << currentTile << endl;
					tile_test = currentTile;
				}
				*/

				if (tiles[currentTile]->mType != 0) {
					player1->xPosition = beforeMoveX;
					player1->yPosition = beforeMoveY;
					/*
					//toDo
					bool xVelPos = player1->xVelocity > 0;
					bool xVelNeg = player1->xVelocity < 0;
					bool yVelPos = player1->yVelocity > 0;
					bool yVelNeg = player1->yVelocity < 0;
					player1->xVelocity = 0;
					player1->yVelocity = 0;
					if (xVelPos)
						player1->xPosition -= 1;
					if (xVelNeg)
						player1->xPosition += 1;
					if (yVelNeg)
						player1->yPosition += 1;
					if (yVelPos)
						player1->yPosition -= 1;

					//player1->xPosition -= 1;
					//player1->yPosition -= 1;
					*/
				}

				camera.x = (player1->xPosition + player1->rectangle.w / 2) - SCREEN_WIDTH / 2;
				camera.y = (player1->yPosition + player1->rectangle.h / 2) - SCREEN_HEIGHT / 2;
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

				if (isClient) {
					int length;
					int result;
					const char* msg = "test!";

					length = strlen(msg) + 1; // add one for the terminating NULL
					result = SDLNet_TCP_Send(clientSocket, msg, length);
					if (result < length) {
						printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
						// It may be good to disconnect sock because it is likely invalid now.
					}

				} if (isHost) {

					// receive some text from sock
					//TCPsocket sock;
					#define MAXLEN 1024
					int result;
					char msg[MAXLEN];

					result = SDLNet_TCP_Recv(clientSocket, msg, MAXLEN);
					if (result <= 0) {
						// An error may have occured, but sometimes you can just ignore it
						// It may be good to disconnect sock because it is likely invalid now.
					}
					printf("Received: \"%s\"\n", msg);
				}



				moveCluster(allEnemies, "random", timePassed, tiles, cycle);
				cycle++;

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
					else if (i->getTextureActive() == i->getTextureIdleNotReady()) {
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

				hudHealthString = "Health: " + to_string(player1->getHPCurrent());
				hudLevelString = "Level: " + to_string(player1->getLevel());
				renderText(hudHealthString.c_str(), &hudHealthTextRectangle, &hudTextColor);
				renderText(hudLevelString.c_str(), &hudLevelTextRectangle, &hudTextColor);

				SDL_RenderPresent(gRenderer);

				if (keyState[SDL_SCANCODE_ESCAPE]) {
					inOverworld = false;
					inPauseMenu = true;
				}

				int enemyToRemove = -1;
				for (auto z : allEnemies)
				{
					enemyToRemove++;
					if (check_collision(player1->rectangle, z->rectangle) && z->combatReady) {
						z->combatReady = false;
						z->readyTimeLeft = 3000;
						z->setTextureActive(z->getTextureIdleNotReady());
						CollidingCluster = z;
						combatants.clear();
						combatants.push_back(player1);
						for (auto i : z->characterGroup)
						{
							combatants.push_back(i);
						}
						allEnemies.erase(allEnemies.begin() + enemyToRemove);
						charactersOnScreen.erase(charactersOnScreen.begin() + enemyToRemove + 1);
						inOverworld = false;
						combatStarted = true;
						break;
					}
					if (z->readyTimeLeft > -1)
						z->readyTimeLeft -= 1;
					if (z->readyTimeLeft == 0) {
						z->combatReady = true;
						z->setTextureActive(z->getTextureIdle());
					}
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
				timeSinceLastMovement = SDL_GetTicks();
			}

			while (combatStarted) {
				combatTransition();
				CombatManager cm;
				//std::cout << combatants.size();
				//convert combatants vector of characters to pointer of characters
				//vector<Character *> c;
				//for (auto i : combatants)
					//c.push_back(&i);
				int combatResult = cm.combatMain(combatants);
				gMusic = Mix_LoadMUS("Audio/Walking_Test.wav");
				if (gMusic == NULL)
					std::cout << "Failed to load music" << std::endl;
				//Play the music
				Mix_PlayMusic(gMusic, -1);
				Mix_ResumeMusic();
				timeSinceLastMovement = SDL_GetTicks();
				std::cout << combatResult << std::endl;
				if (combatResult == ENEMY_WINS) {
					cout << "\nYOU HAVE DIED\nGAME OVER MAN, GAME OVER" << endl;
					exit(1);
				}
				else if (combatResult == PLAYER_WINS) {
					if (allEnemies.size() == 0)
					{
						keepPlaying = false;
						if (MAP_INDEX + 1 == ALL_MAPS.size())
						{
							EndTransition();
							SDL_Delay(4000);
							playCredits();
						}
						else
						{
							player1->xPosition = 0;
							player1->yPosition = 0;
							levelTransition();
						}
					}
				}
				else if (combatResult == PLAYER_ESCAPES) {
					allEnemies.push_back(CollidingCluster);
					charactersOnScreen.push_back(CollidingCluster);
				}
				combatStarted = false;
				inOverworld = true;
			}
			int backToMainMenu = 1;
			if (response == backToMainMenu) { //backToMainMenu
				//for (auto i : charactersOnScreen) {
				//	delete(&i);
				//}
				//destroy tiles etc?
				handleMain();
			}
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
	//Load the music
	gMusic = Mix_LoadMUS("Audio/Main_Test.wav");
	if (gMusic == NULL)
		std::cout << "Failed to load music" << std::endl;
	//Play the music
	Mix_PlayMusic(gMusic, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 5);
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
