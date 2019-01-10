#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <cmath>
#include <cstring>
#include <fstream>
#include <sstream>
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
#include "../Headers/mainGame.h"
#include "../Headers/Player.h"
#include "../Headers/Button.h"
#include "../Headers/Tile.h"
#include "../Headers/CombatManager.h"
#include "../Headers/Cluster.h"
#include "../Headers/LoadTexture.h"
#include "../Headers/aStar.h"
#include "../Headers/Globals.h"
#include "../Headers/ResourceManager/ResourceManager.h"

// Function declarations

bool init();//Starts up SDL, creates window, and initializes OpenGL

void close();//Frees media and shuts down SDL

SDL_Texture* loadImage(std::string fname);

SDL_Renderer* gRenderer = nullptr;
// Globals

SDL_Window* gWindow = nullptr;//The window rendering to

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
const int STARTING_ENEMIES = 15;
//const int STARTING_ENEMIES = 1;
const vector<string> ALL_MAPS = { "map1.txt", "map2.txt", "map3.txt" };
const int MAX_HORIZONTAL_TILES = 30;
const int MAX_VERTICAL_TILES = 30;
int MAP_INDEX = 0;

enum SCENE_CHANGE {
	GOTO_MAIN,
	GOTO_SOLO,
	GOTO_COOP,
	GOTO_CREDITS,
	GOTO_EXIT,
	GOTO_INGAME,
};

Mix_Music *gMusic = NULL;
Mix_Chunk *gBSound = NULL;
TTF_Font* font;

aStar pathing;

//Player ONE
Player* player1;
Player* player2;
std::vector<int> attr;

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

	//set all the required Options for GLFW, Use OpenGL 2.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);//Double-buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Create window
	gWindow = SDL_CreateWindow("CS1666-RPG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (gWindow == nullptr) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

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
	font = TTF_OpenFont("Fonts/Gameplay.ttf", 20);
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

	return true;
}
void getTilePosition(Character* c)
{
	int x_to_tile = (int)(c->xPosition + (c->rectangle.w / 2)) / TILE_WIDTH;
	int y_to_tile = (int)((c->yPosition + c->rectangle.h) / TILE_HEIGHT);
	cout << c->getName() << "(" << x_to_tile << "," << y_to_tile << ")" << endl;
}


SDL_Rect * loadMap(Tile* tiles[MAX_HORIZONTAL_TILES][MAX_VERTICAL_TILES],string mapToLoad)
{
	Tile::loadTiles();
	bool tilesLoaded = true;
	int x = 0, y = 0;
	std::vector<SDL_Rect> blockedTiles;
	std::ifstream map("Maps/"+mapToLoad);
	if (!map.is_open())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{
		for (int tempy = 0; tempy < MAX_VERTICAL_TILES; tempy++)
		{
			for (int tempx = 0; tempx < MAX_HORIZONTAL_TILES; tempx++)
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
					tiles[tempx][tempy] = new Tile(x, y, tileType);
					if (tiles[tempx][tempy]->solid = true)
						blockedTiles.push_back(tiles[tempx][tempy]->getBox());
				}
				//If we don't recognize the tile type
				else
				{
					//Stop loading map
					printf("Error loading map: Invalid tile type at (%d,%d)!\n", tempx,tempy);
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

int playCredits() {
	

	gTex.push_back(loadImage("Images/Credits/dsgCredits.jpg"));
	gTex.push_back(loadImage("Images/Credits/RyanKillenCreditImage.jpg")); //Ryan Killen - rek77
 	gTex.push_back(loadImage("Images/Credits/bmbCredits.jpg"));
	gTex.push_back(loadImage("Images/Credits/dank_farnan_meme.jpg")); //Austin Himler - arh121
	gTex.push_back(loadImage("Images/Credits/Kexin Wang.jpg"));
	gTex.push_back(loadImage("Images/Credits/justin.jpg"));
	gTex.push_back(loadImage("Images/Credits/my_greatest_creation.png")); // jake
	gTex.push_back(loadImage("Images/Credits/ilum.jpg")); // James Finkel
	gTex.push_back(loadImage("Images/Credits/SankethKolliCredit.jpg")); //Sanketh Kolli - ssk38
	gTex.push_back(loadImage("Images/Credits/mjl159Credits.png")); //Mitchell Leng - mjl159

	//Load the music
	gMusic = Mix_LoadMUS("Audio/BGM.wav");
	if (gMusic == NULL)
		std::cout << "Failed to load music" << std::endl;
	//Play the music
	Mix_PlayMusic(gMusic, -1);

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
				return GOTO_EXIT;
			}
			if (j == 0)
			{
				SDL_RenderCopy(gRenderer, i, NULL, NULL);
				SDL_RenderPresent(gRenderer);

			}
			SDL_Delay(15);
			j++;
		}
		j = 0;
	}
	//Stop the music
	Mix_HaltMusic();
	return GOTO_EXIT;
}


void renderText(const char* text, SDL_Rect* rect, SDL_Color* color)
{
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
void GameOverTransition() {
	gMusic = Mix_LoadMUS("Audio/Defeat_Test.wav");
	if (gMusic == NULL)
		std::cout << "Failed to load music" << std::endl;
	//Play the music
	Mix_PlayMusic(gMusic, -1);
	SDL_Rect wholeS = { 0,0,720,720 };
	SDL_Rect word1 = { 280,200,120,60 };
	SDL_Rect word2 = { 180, 240,120,60 };
	SDL_Rect word3 = { 110, 280,120,60 };
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderFillRect(gRenderer, &wholeS);
	SDL_RenderPresent(gRenderer);
	string line1 = "Game Over!";
	string line2 = "You have been defeated!";
	string line3 = "You were not up to the challenge!";
	SDL_Color TextColor = { 255, 255, 255, 0 };
	renderText(line1.c_str(), &word1, &TextColor);
	renderText(line2.c_str(), &word2, &TextColor);
	renderText(line3.c_str(), &word3, &TextColor);
	SDL_RenderPresent(gRenderer);
	SDL_Rect wipe = { 180,240,20,20 };
	SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
	SDL_Delay(200);
}
int networkingScreen()
{
	// Initialize Variables
	bool onNetworking = true;
	bool startReady = false;
	int currentState = -1;

	SDL_Rect IPTextRectangle = { 255, 195, 0,0 };
	SDL_Color textColor = { 112, 96, 80, 0 };
	std::string IPInputText;

	std::vector<Button*> buttons;
	SDL_Texture* host_false = loadImage("Images/UI/NetworkingScreen/Host_False.png");
	SDL_Texture* host_true = loadImage("Images/UI/NetworkingScreen/Host_True.png");
	SDL_Texture* join_false = loadImage("Images/UI/NetworkingScreen/Join_False.png");
	SDL_Texture* join_true = loadImage("Images/UI/NetworkingScreen/Join_True.png");
	SDL_Texture* startlocked = loadImage("Images/UI/NetworkingScreen/StartButton_Locked.png");
	SDL_Texture* startunlocked = loadImage("Images/UI/NetworkingScreen/StartButton_Unlocked.png");
	SDL_Texture* back = loadImage("Images/UI/NetworkingScreen/BackButton.png");

	//Establish Visuals
	buttons.push_back(new Button("start", 450, 425, 230, 56, "Images/UI/NetworkingScreen/StartButton_Locked.png", "", gRenderer));
	buttons.push_back(new Button("host", 450, 500, 230, 56, "Images/UI/NetworkingScreen/Host_False.png", "", gRenderer));
	buttons.push_back(new Button("join", 450, 575, 230, 56, "Images/UI/NetworkingScreen/Join_False.png", "", gRenderer));
	buttons.push_back(new Button("back", 450, 650, 230, 56, "Images/UI/NetworkingScreen/BackButton.png", "", gRenderer));

	// Load Background
	LoadTexture background;
	background.loadFromFile("Images/UI/NetworkingScreen/NetworkingNoButtons.png", gRenderer);
	background.renderBackground(gRenderer);

	//Start Music
	gMusic = Mix_LoadMUS("Audio/charactercreate.wav");
	if (gMusic == NULL)
		std::cout << "Failed to load music" << std::endl;
	gBSound = Mix_LoadWAV("Audio/BSound.wav");
	if (gBSound == NULL)
	{
		printf("Failed to load Button sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	//Play the music
	Mix_PlayMusic(gMusic, -1);



	SDL_Event e;
	while (onNetworking)
	{
		while (SDL_PollEvent(&e))
		{

			if (e.type == SDL_QUIT)
			{
				Mix_HaltMusic();
				return GOTO_EXIT; //end game
			}

			if (e.button.button == (SDL_BUTTON_LEFT) && e.type == SDL_MOUSEBUTTONDOWN)
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				for (auto i : buttons)
				{
					//if mouse is clicked inside a button
					if (((mouseX >= i->x) && (mouseX <= (i->x + i->w))) &&
						((mouseY >= i->y) && (mouseY <= (i->y + i->h))))
					{
						if (i->type == "back")
						{
							for (auto i : buttons)
							{
								delete(i);
							}
							background.free();
							Mix_HaltMusic();
							return GOTO_MAIN;
						}
						else if (i->type == "start")
						{
							if (startReady)
							{
								cout << "Beep";
							}
						}
						else if (i->type == "host")
						{
							currentState = 0;
						}
						else if (i->type == "join")
						{
							currentState = 1;
						}
					}
					
				}
			}
			else if (e.type == SDL_KEYDOWN)
			{
				//remove char if backspace
				if (e.key.keysym.sym == SDLK_BACKSPACE && IPInputText.length() > 0)
				{
					Mix_PlayChannel(-1, gBSound, 0);
					IPInputText.pop_back();
				}
				//Move on by pressing enter
				else if (e.key.keysym.sym == SDLK_RETURN)
				{
				}
			}
			else if (e.type == SDL_TEXTINPUT) {
				//add char
				//set length limit to arbitrariy be 11 (fits textbox about right, depends on what user enters)
				if (IPInputText.length() < 15) {
					Mix_PlayChannel(-1, gBSound, 0);
					IPInputText += e.text.text;
				}
			}


		}
		if (currentState == 0)
			startReady = true;
		else if (currentState == 1 && IPInputText.length() >= 9)
			startReady = true;
		else startReady = false;
		background.renderBackground(gRenderer);
		//Renders buttons and shows pressed image if pressed
		for (auto i : buttons)
		{
			if (i->type == "start")
			{
				if(startReady)
					SDL_RenderCopy(gRenderer, startunlocked, NULL, &i->rect);
				else SDL_RenderCopy(gRenderer, startlocked, NULL, &i->rect);
			}
			else if (i->type == "host")
			{
				if(currentState == 0)
					SDL_RenderCopy(gRenderer, host_true, NULL, &i->rect);
				else SDL_RenderCopy(gRenderer, host_false, NULL, &i->rect);
			}
			else if (i->type == "join")
			{
				if(currentState == 1)
					SDL_RenderCopy(gRenderer, join_true, NULL, &i->rect);
				else SDL_RenderCopy(gRenderer, join_false, NULL, &i->rect);
			}
			else if (i->type == "back")
				SDL_RenderCopy(gRenderer, back, NULL, &i->rect);
		}
		if (IPInputText.length() > 0)
		{
			renderText(IPInputText.c_str(), &IPTextRectangle, &textColor);
		}
		SDL_RenderPresent(gRenderer);
		SDL_Delay(16);
	}
	return GOTO_CREDITS;
}
int characterCreateScreen()
{
	// Initialize Variables
	bool onCharacterCreate = true;
	bool backToMain = false;
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
	SDL_Rect nameTextRectangle = { 150, 640, 0,0 };
	SDL_Rect errorTextRectangle = { 115, 580, 0, 0 };
	SDL_Rect errorTextRectangleLong = { 25, 580, 0, 0 };
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


	//Establish Visuals
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
	buttons.push_back(new Button("start", 450, 575, 230, 56, "Images/UI/CreateScreen/StartButton.png", "", gRenderer));
	buttons.push_back(new Button("back", 450, 650, 230, 56, "Images/UI/CreateScreen/BackButton.png", "", gRenderer));

	// Load Background
	LoadTexture background;
	background.loadFromFile("Images/UI/CreateScreen/characterCreateV2NoButtons.png", gRenderer);
	background.renderBackground(gRenderer);

	//Start Music
	gMusic = Mix_LoadMUS("Audio/charactercreate.wav");
	if (gMusic == NULL)
		std::cout << "Failed to load music" << std::endl;
	gBSound = Mix_LoadWAV("Audio/BSound.wav");
	if (gBSound == NULL)
	{
		printf("Failed to load Button sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	//Play the music
	Mix_PlayMusic(gMusic, -1);


	
	SDL_Event e;
	while (onCharacterCreate)
	{
		while (SDL_PollEvent(&e))
		{

			if (e.type == SDL_QUIT)
			{
				Mix_HaltMusic();
				return GOTO_EXIT; //end game
			}

			if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_KEYDOWN)
			{
				errorInputText = "";
			}
			if (e.button.button == (SDL_BUTTON_LEFT) && e.type == SDL_MOUSEBUTTONDOWN)
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				int deltaAttribute;
				for (auto i : buttons)
				{
					//if mouse is clicked inside a button
					if (((mouseX >= i->x) && (mouseX <= (i->x + i->w))) &&
						((mouseY >= i->y) && (mouseY <= (i->y + i->h))))
					{
					if (i->type == "back")
					{
						if (backToMain == true)
						{
							for (auto i : buttons) {
								delete(i);
							}
							background.free();
							Mix_HaltMusic();
							return GOTO_MAIN;
						}
						else
						{
							backToMain = true;
							errorInputText = "Progress Will Not Be Saved!";
						}
					}
					else backToMain = false;
					i->pressed = 5;
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
									return GOTO_INGAME;
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
						else if (i->type == "up") {
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
								if (nameInputText == "nfl4" || nameInputText == "nlf4"){
									player1 = new Player(nameInputText, 10, 10, 10, 10, 10);//player1->setAll(nameInputText, 10, 10, 10, 10, 10);
									attr.push_back(10);
									attr.push_back(10);
									attr.push_back(10);
									attr.push_back(10);
									attr.push_back(10);

								}else {
									attr.push_back(strength);
									attr.push_back(intelligence);
									attr.push_back(dexterity);
									attr.push_back(constitution);
									attr.push_back(faith);
									player1 = new Player(nameInputText, strength, intelligence, dexterity, constitution, faith);//player1->setAll(nameInputText, strength, intelligence, dexterity, constitution, faith);
								}
								std::cout << std::string(*player1); //displays player 1
								//make Character Object, validate, return to main
								for (auto i : buttons) {
									delete(i);
								}
								background.free();
								Mix_HaltMusic();
								return GOTO_INGAME;
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
				if (i->pressed <= 0 || i->attribute == "")
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
	return GOTO_CREDITS;
}

int handlePauseMenu(bool inPauseMenu, std::vector<Character*> charactersOnScreen, Tile *tiles[MAX_HORIZONTAL_TILES][MAX_VERTICAL_TILES], SDL_Rect camera) {
	std::vector<Button*> buttons;
	buttons.push_back(new Button("continue", 240, 200, 260, 64, "Images/UI/PauseMenu/ContinueButton.png", "", gRenderer));
	buttons.push_back(new Button("exit", 240, 300, 260, 64, "Images/UI/PauseMenu/ExitButton.png", "", gRenderer));
	SDL_Texture* background = loadImage("Images/UI/PauseMenu/PauseMenuNoButtons.png"); 

	SDL_Event e;
	while (inPauseMenu) {
		while (SDL_PollEvent(&e)) {
			const Uint8* key = SDL_GetKeyboardState(nullptr);
			if (e.type == SDL_QUIT) {
				inPauseMenu = false;
				return GOTO_EXIT;
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
							return GOTO_INGAME;
						}
						else if (i->type == "exit") {
							for (auto i : buttons) {
								delete(i);
							}
							SDL_DestroyTexture(background);
							inPauseMenu = false;
							return GOTO_EXIT;
						}

					}

				}

			}
			
		}
		//Set Black
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		
		for (int tempx = 0; tempx < MAX_HORIZONTAL_TILES; tempx++)
		{
			for (int tempy = 0; tempy < MAX_VERTICAL_TILES; tempy++)
			{
				tiles[tempx][tempy]->render(&camera);
			}
		}
		

		for (auto &i : charactersOnScreen)
		{
			if (i->xVelocity > 0 && i->flip == SDL_FLIP_HORIZONTAL)
				i->flip = SDL_FLIP_NONE;
			else if (i->xVelocity < 0 && i->flip == SDL_FLIP_NONE)
				i->flip = SDL_FLIP_HORIZONTAL;

			if (i->getTextureActive() == i->getTextureIdle())
			{
				if (SDL_GetTicks() - i->timeSinceLastAnimation > i->getTimeBetweenIdleAnimations())
				{
					i->currentFrame = (i->currentFrame + 1) % i->currentMaxFrame;
					i->timeSinceLastAnimation = SDL_GetTicks();
				}
			}
			else 
			{
				if (SDL_GetTicks() - i->timeSinceLastAnimation > i->getTimeBetweenRunAnimations())
				{
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
	return GOTO_EXIT;
}

/*
	if dest-x is greater and dest-y is greater:	right or down (random)
					 ... and dest-y is less: up or right (random)
.				     ... and dest-y is equal: right
	if dest-x is equal and dest-y is greater: down
				   ... and dest-y is less: up
				   ... and dest-y is equal: (@ destination Node) error with pathing algo
	if dest-x is less and dest-y is greater: down or left (random)
				  ... and dest-y is less: left or up (random)
				  ... and dest-y is equal: left
*/

int getDirection(Cluster* c) {
	double dx, dy;
	int out;
	dx =  c->seqX.front()- (int)c->xPosition / TILE_WIDTH;
	dy =  c->seqY.front() - (int)c->yPosition / TILE_HEIGHT;
	out = (int) (std::atan2(dy, dx) * 180 / -3.14);
	
	if (out >= -181 && out < -135) return 1;
	else if (out >= -135 && out < -45) return 2;
	else if (out >= -45 && out < 45) return 3;
	else if (out >= 45 && out < 135) return 0;
	else if (out >= 135 && out <= 181) return 1;
	return -1;
}


/*
move location of Cluster cl

a indicates movement type
up/down/left/right/random
*/
int moveCluster(std::vector<Cluster*> c, std::string move, double time, Tile* map[MAX_HORIZONTAL_TILES][MAX_VERTICAL_TILES]) {

	for (auto cl : c)
	{
		if (cl->combatReady)
		{
  			getTilePosition(cl);
			getTilePosition(player1);
			int a = -1;
			if (move == "pursuit")
			{

				if ((std::sqrt((cl->xPosition - player1->xPosition) * (cl->xPosition - player1->xPosition)
					+ (cl->yPosition - player1->yPosition) * (cl->yPosition - player1->yPosition)) < cl->aRange)) {

					int currentP = (int)(player1->xPosition + player1->rectangle.w / 2) / TILE_WIDTH;
					currentP += (int)((player1->yPosition + player1->rectangle.h) / TILE_HEIGHT) * 30;

					if (cl->seqX.empty() && cl->seqY.empty()) {
						/*
						std::cout << "x0: " << cl->xPosition / TILE_WIDTH << std::endl;
						std::cout << "y0: " << cl->yPosition / TILE_HEIGHT << std::endl;
						std::cout << "px0: " << player1->xPosition / TILE_WIDTH << std::endl;
						std::cout << "py0: " << player1->yPosition / TILE_HEIGHT << std::endl;
						*/
						aStar::map m;
						aStar::point s((int)(cl->xPosition / TILE_WIDTH), (int)(cl->yPosition / TILE_HEIGHT));
						aStar::point e((int)(player1->xPosition / TILE_WIDTH), (int)(player1->yPosition / TILE_HEIGHT));
						std::list<aStar::point> path;

						pathing.search(s, e, m);
						pathing.path(path);
						for (std::list<aStar::point>::iterator i = path.begin(); i != path.end(); i++) {
							if ((*i).x != (int)cl->xPosition / TILE_WIDTH && (*i).y != (int)cl->yPosition) {
								cl->seqX.push((*i).x);
								cl->seqY.push((*i).y);
							}

							//std::cout << "NOTE" << std::endl;
							//std::cout << "X: " << (*i).x << std::endl;
							//std::cout << "Y: " << (*i).y << std::endl;

						}
						
					}
					else if (currentP != pathing.lastPlayerLocation) {
						while (!cl->seqX.empty()) cl->seqX.pop();
						while (!cl->seqY.empty()) cl->seqY.pop();

						aStar::map m;
						aStar::point s((int)(cl->xPosition / TILE_WIDTH), (int)(cl->yPosition / TILE_HEIGHT));
						aStar::point e((int)(player1->xPosition / TILE_WIDTH), (int)(player1->yPosition / TILE_HEIGHT));
						std::list<aStar::point> path;

						//std::cout << pathing.search(s, e, m) << std::endl;

						pathing.path(path);
						for (std::list<aStar::point>::iterator i = path.begin(); i != path.end(); i++) {
							if ((*i).x != (int)cl->xPosition / TILE_WIDTH && (*i).y != (int)cl->yPosition) {
								cl->seqX.push((*i).x);
								cl->seqY.push((*i).y);
							}

							//std::cout << "BOTE" << std::endl;
							//std::cout << "X: " << (*i).x << std::endl;
							//std::cout << "Y: " << (*i).y << std::endl;
						}
						

					}

					a = getDirection(cl);
					if (a == -1)
						return -1;
					//std::cout << a << std::endl;
					//std::cout << "NOTE" << std::endl;
					//std::cout << a << std::endl;
					pathing.lastPlayerLocation = currentP;
					//std::cout << "NOTE" << std::endl;
				}
				else {
					//random
					//std::cout << "Boat" << std::endl;
					while (!cl->seqX.empty()) cl->seqX.pop();
					while (!cl->seqY.empty()) cl->seqY.pop();
				}
			}
			//std::cout << a << std::endl;
			cl->xDeltaVelocity = 0;
			cl->yDeltaVelocity = 0;

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
			if (beforeMoveX / TILE_WIDTH <= cl->seqX.front() && cl->seqX.front() <= (int)cl->xPosition / TILE_WIDTH) {
				if (beforeMoveY / TILE_HEIGHT <= cl->seqY.front() && cl->seqY.front() <= (int)cl->yPosition / TILE_HEIGHT) {
					cl->seqX.pop();
					cl->seqY.pop();
				}
			}
			int x_to_tile = (int)(cl->xPosition + cl->rectangle.w / 2) / TILE_WIDTH;
			int y_to_tile = (int)((cl->yPosition + cl->rectangle.h) / TILE_HEIGHT) ;


			//std::cout << cl->xPosition << std::endl;
			//std::cout << cl->yPosition << std::endl;


			if (map[x_to_tile][y_to_tile]->mType != 0) {
				cl->xPosition = beforeMoveX;
				cl->yPosition = beforeMoveY;
			}

			for (auto i : c) {
				if (i != cl) {
					int other_x = (int)(i->xPosition + i->rectangle.w / 2) / TILE_WIDTH;
					int other_y = (int)((i->yPosition + i->rectangle.h) / TILE_HEIGHT) ;
					if (other_x == x_to_tile && other_y == y_to_tile)
					{
						cl->xPosition = beforeMoveX;
						cl->yPosition = beforeMoveY;
					}
				}
			}
			//std::cout << cl->xPosition << std::endl;
			//std::cout << cl->yPosition << std::endl;
		}
	}
	return 0;
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
	return false;
}

int playGame() {
	std::vector<Character*> charactersOnScreen;
	std::vector<Character*> playersOnScreen;
	playersOnScreen.push_back(player1);
	std::vector<Character*> combatants;
	player1->refillEnergy();

	pathing = aStar();
	Uint32 timeSinceLastMovement = SDL_GetTicks();
	Uint32 timeSinceLastAnimation = SDL_GetTicks();
	Uint32 lastSync = SDL_GetTicks();
	player1->timeSinceLastMovement = timeSinceLastMovement;
	player1->timeSinceLastAnimation = timeSinceLastAnimation;
	vector<Cluster*> allEnemies;
	Cluster* CollidingCluster;
	std::string receiveString="";
	bool doNetworking = handleNetworkingSetup();
	if (doNetworking)
	{
		player2 = new Player("meme", 10, 10, 10, 10, 10);
		
		charactersOnScreen.push_back(player2);
		playersOnScreen.push_back(player2);
	}

	//Load the music
	gMusic = Mix_LoadMUS("Audio/Walking_Test.wav");
	if (gMusic == NULL)
		std::cout << "Failed to load music" << std::endl;
	//Play the music
	Mix_PlayMusic(gMusic, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
	for (MAP_INDEX = 0; MAP_INDEX < ALL_MAPS.size(); MAP_INDEX++)
	{
		Tile*  tiles[MAX_HORIZONTAL_TILES][MAX_VERTICAL_TILES];

		//tiles
		//Need to delete this to stop memory leak if we load more than one map
		SDL_Rect* BlockedTiles = loadMap(tiles, ALL_MAPS.at(MAP_INDEX));
		for (;;)
		{
			int x_to_tile = (int)(player1->xPosition + (player1->rectangle.w / 2)) / TILE_WIDTH;
			int y_to_tile = (int)((player1->yPosition + player1->rectangle.h) / TILE_HEIGHT) ;
			if (tiles[x_to_tile][y_to_tile]->mType == 0)
				break;
			player1->xPosition = rand() % (LEVEL_WIDTH - player1->getImageWidth());
			player1->yPosition = rand() % (LEVEL_HEIGHT - player1->getImageHeight());
		}

		if (doNetworking) {
			int numEnemies = STARTING_ENEMIES * (MAP_INDEX + 1); //2
			allEnemies = vector<Cluster*>();
			if (isHost) {
			
				for (int num_enemy = 0; num_enemy < numEnemies; num_enemy++)
				{
					int length = sizeof(int);
					int param = (rand() % (ENEMIES_PER_CLUSTER + MAP_INDEX)) + 1;
					Cluster* enemy = new Cluster(param);
					//printf("Host Sending %d\n", &param);
					//result = SDLNet_TCP_Send(clientSocket, &param, sizeof(int));
					//if (result < length) {
					//	printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
					//}
					//std::cout << "Host Done Sending\n" << std::endl;

					cout << "Enemy " << num_enemy + 1 << " Cluster Size: " << enemy->clusterSize << endl;
					allEnemies.push_back(enemy);
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
						int x_to_tile = (int)(player1->xPosition + (player1->rectangle.w / 2)) / TILE_WIDTH;
						int y_to_tile = (int)((player1->yPosition + player1->rectangle.h) / TILE_HEIGHT) ;
						if (tiles[x_to_tile][y_to_tile]->mType == 0)
							break;
					}
				}

				for (auto i : allEnemies)
				{
					cout << "Enemy Coordinates: (" << i->xPosition << "," << i->yPosition << ")" << endl;
					i->timeSinceLastAnimation = timeSinceLastAnimation;
					charactersOnScreen.push_back(i);

					//std::string cppString = player1->ptoString();
					//const char* myString = cppString.c_str();
					//length = strlen(myString) + 1;
					//printf("Host Sending %s\n", myString);
					//result = SDLNet_TCP_Send(clientSocket, myString, length);
					//if (result < length) {
					//	printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
					//}
					//std::cout << "Host Done Sending\n" << std::endl;
				}

			}
			else if (isClient) {
				//std::vector<int> params;
				//char temp[100];
				//for (int i = 0; i < numEnemies; i++) {					
				//	SDLNet_TCP_Recv(clientSocket, temp, sizeof(int));
				//}

				//int j = 0;
				//while (temp[j] != 0) {
				//	std::cout << "param buffer at index " << j << " with  " << temp[j] << std::endl;
				//	params.push_back(temp[j] - '0');
				//	j++;
				//}

				//for (int i = 0; i < params.size(); i++) {
				//	std::cout << params[i] << std::endl;
				//	Cluster* enemy = new Cluster(params[i]);
				//	cout << "Enemy " << i + 1 << " Cluster Size: " << enemy->clusterSize << endl;
				//	allEnemies.push_back(enemy);
				//}

				for (int i = 0; i < numEnemies; i++) {
					Cluster* enemy = new Cluster(1);
					cout << "Enemy " << i + 1 << " Cluster Size: " << enemy->clusterSize << endl;
					allEnemies.push_back(enemy);
				}


				for (auto i : allEnemies)
				{
					cout << "Enemy Coordinates: (" << i->xPosition << "," << i->yPosition << ")" << endl;
					i->timeSinceLastAnimation = timeSinceLastAnimation;
					i->setTextureActive(i->getTextureIdle());
					i->currentMaxFrame = i->getNumIdleAnimationFrames();
					charactersOnScreen.push_back(i);
				}

			}
		}
		else {
			allEnemies = vector<Cluster*>();
			for (int num_enemy = 0; num_enemy < STARTING_ENEMIES * (MAP_INDEX + 1); num_enemy++)
			{
				Cluster* enemy = new Cluster((rand() % (ENEMIES_PER_CLUSTER + MAP_INDEX)) + 1);
				cout << "Enemy " << num_enemy + 1 << " Cluster Size: " << enemy->clusterSize << endl;
				allEnemies.push_back(enemy);
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
						int x_to_tile = (int)(player1->xPosition + (player1->rectangle.w / 2)) / TILE_WIDTH;
						int y_to_tile = (int)((player1->yPosition + player1->rectangle.h) / TILE_HEIGHT) ;
						if (tiles[x_to_tile][y_to_tile]->mType == 0)
							break;
					}
			}

			for (auto i : allEnemies)
			{
				cout << "Enemy Coordinates: (" << i->xPosition << "," << i->yPosition << ")" << endl;
				i->timeSinceLastAnimation = timeSinceLastAnimation;
				charactersOnScreen.push_back(i);
			}


		}

		//SDL_RendererFlip flip = SDL_FLIP_NONE;

		int tile_test = -1;

		player1->setTextureActive(player1->getTextureIdle());
		player1->currentMaxFrame = player1->getNumIdleAnimationFrames();


		
		if (doNetworking)
		{
			player2->setTextureActive(player2->getTextureIdle());
			player2->currentMaxFrame = player2->getNumIdleAnimationFrames();
			player2->timeSinceLastMovement = timeSinceLastMovement;
			player2->timeSinceLastAnimation = timeSinceLastAnimation;
		}

		std::string hudHealthString = "Health: " + to_string(player1->getHPCurrent());
		std::string hudLevelString = "Level: " + to_string(player1->getLevel());
		SDL_Rect hudHealthTextRectangle = { 10, 35, 0, 0 };
		SDL_Rect hudLevelTextRectangle = { 10, 10, 0, 0 };
		SDL_Color hudTextColor = { 0, 0, 0, 0 };

		double timePassed = 0;
		int response = 0;

		charactersOnScreen.push_back(player1);

		std::cout << player1->xPosition;
		std::cout << "\n";
		std::cout << player1->yPosition;


		SDL_Event e;
		SDL_Rect camera = { 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
		bool inOverworld = true;
		bool combatStarted = false;
		bool inPauseMenu = false;
		bool keepPlaying = true;
		while (keepPlaying) {
			while (inOverworld) 
			{
				while (SDL_PollEvent(&e)) {
					if (e.type == SDL_QUIT) {
						inOverworld = false;
						return GOTO_EXIT;
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
				int beforeMoveX = (int)player1->xPosition;
				int beforeMoveY = (int)player1->yPosition;
				for (auto &i : playersOnScreen)
				{
					if (i->xVelocity != 0 || i->yVelocity != 0) {

						if (i->yVelocity == 0) {
							if (i->getTextureActive() != i->getTextureRun()) {
								i->setTextureActive(i->getTextureRun());
								i->currentFrame = 0;
								i->currentMaxFrame = i->getNumRunAnimationFrames();
							}
						}


						if (i->xVelocity == 0 && i->yVelocity > 0) {
							if (i->getTextureActive() != i->getTextureDownRun()) {
								i->setTextureActive(i->getTextureDownRun());
								i->currentFrame = 0;
								i->currentMaxFrame = i->getNumRunAnimationFrames();
							}
						}



						if (i->xVelocity != 0 && i->yVelocity > 0) {
							if (i->getTextureActive() != i->getTextureDownRightRun()) {
								i->setTextureActive(i->getTextureDownRightRun());
								i->currentFrame = 0;
								i->currentMaxFrame = i->getNumRunAnimationFrames();
							}
						}

						if (i->xVelocity != 0 && i->yVelocity < 0) {
							if (i->getTextureActive() != i->getTextureUpRightRun()) {
								i->setTextureActive(i->getTextureUpRightRun());
								i->currentFrame = 0;
								i->currentMaxFrame = i->getNumRunAnimationFrames();
							}
						}



						if (i->xVelocity == 0 && i->yVelocity < 0) {
							if (i->getTextureActive() != i->getTextureUpRun()) {
								i->setTextureActive(i->getTextureUpRun());
								i->currentFrame = 0;
								i->currentMaxFrame = i->getNumRunAnimationFrames();

							}
						}
					}

					else {
						if (i->getTextureActive() != i->getTextureIdle()) {
							i->setTextureActive(i->getTextureIdle());
							i->currentFrame = 0;
							i->currentMaxFrame = i->getNumIdleAnimationFrames();
						}
					}

					
					//Move vertically
					i->yPosition += (i->yVelocity * timePassed);
					if (i->yPosition < 0 || (i->yPosition + i->getImageHeight() > LEVEL_HEIGHT)) {
						//go back into window
						i->yPosition -= (i->yVelocity * timePassed);
					}

					//Move horizontally
					i->xPosition += (i->xVelocity * timePassed);
					if (i->xPosition < 0 || (i->xPosition + i->getImageWidth() > LEVEL_WIDTH)) {
						//go back into window
						i->xPosition -= (i->xVelocity * timePassed);
					}
				}
				//calculate tile player is currently standing on
				int x_to_tile = (int)(player1->xPosition + (player1->rectangle.w / 2)) / TILE_WIDTH;
				int y_to_tile = (int)((player1->yPosition + player1->rectangle.h) / TILE_HEIGHT) ;
				
				// Show which tile the character is standing on
				/*
				if (currentTile != tile_test) {
					cout << currentTile << endl;
					tile_test = currentTile;
				}
				*/

				if (tiles[x_to_tile][y_to_tile]->mType != 0) {
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

				camera.x = (int)((player1->xPosition + player1->rectangle.w / 2) - SCREEN_WIDTH / 2);
				camera.y = (int)((player1->yPosition + player1->rectangle.h / 2) - SCREEN_HEIGHT / 2);
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

				for (int tempx = 0; tempx < MAX_HORIZONTAL_TILES; tempx++)
				{
					for (int tempy = 0; tempy < MAX_VERTICAL_TILES; tempy++)
					{
						tiles[tempx][tempy]->render(&camera);
					}
				}


				if (doNetworking) {
					if (isHost) {
						int moveResult = moveCluster(allEnemies, "pursuit", timePassed, tiles);
						if (moveResult == -1)
							return GOTO_EXIT;
					}
				}
				else {
					int moveResult = moveCluster(allEnemies, "pursuit", timePassed, tiles);
					if (moveResult == -1)
						return GOTO_EXIT;
				}


				for (auto &i : charactersOnScreen)
				{
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

				int enemyToRemove = 0;
				for (auto z : allEnemies)
				{
					
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
						charactersOnScreen.erase(charactersOnScreen.begin() + enemyToRemove);
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
					enemyToRemove++;
				}
				if (doNetworking&&(SDL_GetTicks()-lastSync>25)) {
					int length;
					int result;
					lastSync = SDL_GetTicks();


					
					if (isHost)
					{
						std::string sendString;
						std::string enemyString;
						std::string cppString = player1->ptoString();
						std::stringstream ssfull;
						ssfull<< cppString;
	/*					const char* myString = cppString.c_str();
						length = strlen(myString) + 1;
						printf("Host Sending PLAYER %s\n", myString);
						result = SDLNet_TCP_Send(clientSocket, myString, length);
						if (result < length) {
							printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
						}
						std::cout << "Host Done Sending PLAYER\n" << std::endl;*/


						for (auto i : allEnemies) {
							enemyString = i->ptoString();
							
							ssfull<< enemyString;
							
							////const char* enemyStringChar = enemyString.c_str();
							////length = strlen(enemyStringChar) + 1;
							////printf("Host Sending ENEMY %s\n", enemyStringChar);
							////result = SDLNet_TCP_Send(clientSocket, enemyStringChar, length);
							////if (result < length) {
							////	printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
							////}
							////std::cout << "Host Done Sending ENEMY\n" << std::endl;
						}
						ssfull << "+";
						std::string ctemp=ssfull.str();
						const char* myString = ctemp.c_str();
						length = (int)strlen(myString) + 1;
						printf("Host Sending PLAYER and ENEMIES %s\n", myString);
						result = SDLNet_TCP_Send(clientSocket, myString, length);
						if (result < length) {
							printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
						}
						std::cout << "Host Done Sending PLAYER and ENEMIES\n" << std::endl;


						
						std::stringstream notyoStream;
						
						char temp[100];
						std::cout << "Host Recieving PLAYER\n" << std::endl;

						
							SDLNet_TCP_Recv(clientSocket, temp, 100);
							notyoStream << temp;
							receiveString += notyoStream.str();
							notyoStream.flush();
								while (receiveString.length()>1&&receiveString.find('*') != string::npos)
								{
									std::string notYourSTD = receiveString;
									notYourSTD = notYourSTD.substr(0, notYourSTD.find("*"));
									receiveString = receiveString.substr(receiveString.find("*") + 1, receiveString.length());
									std::cout << "Recieved PLAYER" << notYourSTD << std::endl;
									player2->fromString(notYourSTD);
								}

						
					}
					else
					{
						//recieve character and push back
						std::stringstream receiveStream;
						
						char buffer[100];
						std::cout << "Client Recieving PLAYER\n" << std::endl;
		
						
						SDLNet_TCP_Recv(clientSocket, buffer, 100);
						receiveStream << buffer;
						receiveString += receiveStream.str();
						receiveStream.flush();
						//std::cout << receiveStream.str() << endl;
						
							while (receiveString.length() > 1 && receiveString.find('+') != string::npos)
							{
								std::cout << receiveString << endl;
								std::string notYourSTD = receiveString.substr(0, receiveString.find("*"));
								std::cout << "client Recieved PLAYER " << notYourSTD << std::endl;
								player2->fromString(notYourSTD);
								std::string enemySTD = receiveString.substr(receiveString.find("*") + 1, receiveString.find("Z"));
								bool firstrun = true;
								for (auto i : allEnemies)
								{

									if (!firstrun)
									{
										enemySTD = receiveString.substr(0, receiveString.find("Z"));
									}
									else
										firstrun = false;
									std::cout << "client Recieved ENEMY" << enemySTD << std::endl;
									i->fromString(enemySTD);
									receiveString = receiveString.substr(receiveString.find("Z") + 1, receiveString.length());

								}
							}
						
						
						//Send Character
						std::string cppString = player1->ptoString();
						const char* myString = cppString.c_str();
						length = (int)strlen(myString) + 1;
						printf("Client Sending PLAYER%s\n", myString);
						result = SDLNet_TCP_Send(clientSocket, myString, length);
						if (result < length) {
							printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
						}
						std::cout << "Client Done Sending PLAYER\n" << std::endl;

						//for (auto i : allEnemies) {

						//	//std::string cppString = i->ptoString();
						//	//const char* myString = cppString.c_str();
						//	//length = strlen(myString) + 1;

						//	std::stringstream enemyStream;
						//	enemyStream << "#";
						//	char tempEnemy[100];
						//	std::cout << "Client Recieving ENEMY\n" << enemyStream.str().back() << std::endl;

						//	while (enemyStream.str().back() != 'Z')
						//	{
						//		std::cout << "ENEMY STREAM IN LOOP before << temp: " << enemyStream.str() << std::endl;
						//		SDLNet_TCP_Recv(clientSocket, tempEnemy, 100);
						//		enemyStream << tempEnemy;
						//		std::cout << "AFTER << temp " << enemyStream.str() << endl;
						//	}
						//	std::string enemySTD(enemyStream.str());
						//	enemySTD = enemySTD.substr(1, enemySTD.find("Z"));
						//	std::cout << "Recieved ENEMY" << enemySTD << std::endl;
						//	i->fromString(enemySTD);
						//}

						
					}
				}
			}

			if (inPauseMenu) {
				response = handlePauseMenu(inPauseMenu, charactersOnScreen, tiles, camera);
				switch (response)
				{
				case GOTO_INGAME:
					inOverworld = true;
					break;
				default:
					return GOTO_EXIT;
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
					GameOverTransition();
					SDL_Delay(8000);
					return GOTO_EXIT;
				}
				else if (combatResult == PLAYER_WINS) {
					if (allEnemies.size() == 0)
					{
						for (auto en : charactersOnScreen)
						{
							charactersOnScreen.erase(charactersOnScreen.begin());
						}
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
					charactersOnScreen.push_back(player1);
				}
				else if (combatResult == PLAYER_EXIT) {
					return GOTO_EXIT;
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
	return GOTO_CREDITS;
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

int mainMenu()
{
	//Load the music
	gMusic = Mix_LoadMUS("Audio/Main_Test.wav");
	if (gMusic == NULL)
		std::cout << "Failed to load music" << std::endl;
	//Play the music
	Mix_PlayMusic(gMusic, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 5);
	bool run = true;
	std::vector<Button*> buttons;

	SDL_Texture* singleplayer = loadImage("Images/UI/MainMenu/SingleplayerButton.png");
	SDL_Texture* multiplayer = loadImage("Images/UI/MainMenu/MultiplayerButton.png");
	SDL_Texture* credits = loadImage("Images/UI/MainMenu/CreditsButton.png");
	SDL_Texture* exit = loadImage("Images/UI/MainMenu/ExitButton.png");
	SDL_Texture* title = loadImage("Images/UI/MainMenu/title.png");
	SDL_Rect space = { 100, 50, 526, 72 };
	
	// Add all buttons to a vector
	buttons.push_back(new Button("singleplayer", 240, 200, 240, 64, "Images/UI/MainMenu/BlankButton.png", "", gRenderer));
	buttons.push_back(new Button("multiplayer", 240, 275, 240, 64, "Images/UI/MainMenu/BlankButton.png", "", gRenderer));
	buttons.push_back(new Button("credits", 240, 350, 240, 64, "Images/UI/MainMenu/BlankButton.png", "", gRenderer));
	buttons.push_back(new Button("exit", 240, 425, 240, 64, "Images/UI/MainMenu/BlankButton.png", "", gRenderer));

	SDL_Texture* background = loadImage("Images/UI/MainMenu/MainMenuNoButtons.png");

	SDL_Event e;
	SDL_RenderCopy(gRenderer, background, NULL, NULL);
	SDL_RenderCopy(gRenderer, title, NULL, &space);
	for (auto i : buttons)
	{
		if (i->type == "singleplayer") SDL_RenderCopy(gRenderer, singleplayer, NULL, &i->rect);
		else if (i->type == "multiplayer") SDL_RenderCopy(gRenderer, multiplayer, NULL, &i->rect);
		else if (i->type == "credits") SDL_RenderCopy(gRenderer, credits, NULL, &i->rect);
		else if (i->type == "exit")  SDL_RenderCopy(gRenderer, exit, NULL, &i->rect);
	}
	SDL_RenderPresent(gRenderer);
	while (run)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				close();
				return GOTO_EXIT;
			}
			if (e.button.button == (SDL_BUTTON_LEFT) && e.type == SDL_MOUSEBUTTONDOWN)
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				for (auto i : buttons) {
					//if mouse is clicked inside a button
					if (((mouseX >= i->x) && (mouseX <= (i->x + i->w))) && ((mouseY >= i->y) && (mouseY <= (i->y + i->h))))
					{
						if (i->type == "singleplayer")
						{
							for (auto i : buttons)
							{
								delete(i);
							}
							SDL_DestroyTexture(background);
							run = false;
							return GOTO_SOLO; // GO TO CHARACTER SELECT
						}
						else if (i->type == "multiplayer")
						{
							for (auto i : buttons)
							{
								delete(i);
							}
							SDL_DestroyTexture(background);
							run = false;
							return GOTO_COOP; // GO TO COOP
						}
						else if (i->type == "credits")
						{
							for (auto i : buttons)
							{
								delete(i);
							}
							SDL_DestroyTexture(background);
							run = false;
							return GOTO_CREDITS; // GO TO CREDITS
						}
						else if (i->type == "exit") {
							for (auto i : buttons) {
								delete(i);
							}
							SDL_DestroyTexture(background);
							run = false;
							return GOTO_EXIT; // EXIT
						}
						break;
					}
				}
			}
		}
	}
	return GOTO_CREDITS;
}

int main(int argc, char *argv[])
{
	srand((unsigned int)time(NULL));
	if (!init())
	{
		std::cout << "Failed to initialize!" << std::endl;
		close();
		exit(1);
	}
	handleMain();
	close();
	exit(1);
}
void handleMain()
{
	player1 = new Player("nlf4", 1, 1, 1, 1, 1);
	int currentMode = GOTO_MAIN;
	while (true)
	{
		switch (currentMode)
		{
		case GOTO_MAIN:
			currentMode = mainMenu();
			break;
		case GOTO_SOLO:
			currentMode = characterCreateScreen();
			break;
		case GOTO_COOP:
			currentMode = networkingScreen();
			break;
		case GOTO_INGAME:
			currentMode = playGame();
			break;
		case GOTO_CREDITS:
			currentMode = playCredits();
			break;
		case GOTO_EXIT:
			return;
		default:
			cout << "Option not set" << endl;
			return;
		}
	}	
}
