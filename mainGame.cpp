#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <cmath>
#include <fstream>

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;


// Function declarations
bool init();
SDL_Texture* loadImage(std::string fname);
void close();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
std::vector<SDL_Texture*> gTex;
// Music var
Mix_Music *gMusic = NULL;
TTF_Font* font; 
class Player
{
public:
	int strength=1;
	int intelligence=1;
	int dexterity=1;
	int constitution=1;
	int faith=1;
	std::string name;
	//stores stats
	Player(){}
	Player(int s, int i, int d, int c, int f, std::string n) 
	{
		strength=s;
		intelligence=i;
		dexterity=d;
		constitution=c;
		faith=f;
		name=n;
	}
	void setAll(int s, int i, int d, int c, int f, std::string n)
	{
		strength = s;
		intelligence = i;
		dexterity = d;
		constitution = c;
		faith = f;
		name = n;
		return;
	}
	operator std::string() 
	{ 
		std::string strengthString = std::to_string(strength);
		std::string intelligenceString = std::to_string(intelligence);
		std::string dexterityString = std::to_string(dexterity);
		std::string constitutionString = std::to_string(constitution);
		std::string faithString = std::to_string(faith);
		std::string s = "Name: " + name + " Strength: " + strengthString + " Intelligence: " + intelligenceString + " Dexterity: " + dexterityString + " Constitution: " + constitutionString + " Faith: " + faithString; ;
		return s;
	}
	

private:
	
};
//Player ONE
Player player1;

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

SDL_Texture* loadImage(std::string fname) {
	SDL_Texture* newText = nullptr;

	SDL_Surface* startSurf = IMG_Load(fname.c_str());
	if (startSurf == nullptr) {
		std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

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


class Button {
private:

public:
	std::string type; //up stat, down stat, or start
	int x;
	int y;
	int h;
	int w;
	std::string imageResource;
	std::string attribute; //change to Attribute object later
	SDL_Rect rect;
	SDL_Texture* texture;
	Button(std::string type, int x, int y, int w, int h, std::string imageResource, std::string attribute) {
		this->type = type;
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->imageResource = imageResource;
		this->rect = { x, y, w, h };
		this->attribute = attribute;
		this->texture = loadImage(imageResource);
	}
};

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
	SDL_Rect pointsAllocatedRectangle = { 225, 32, 0, 0};
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
																		//need attr objects
	buttons.push_back(new Button("up", 340, 80, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "strength"));
	buttons.push_back(new Button("down", 340, 130, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "strength"));
	buttons.push_back(new Button("up", 340, 175, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "intelligence"));
	buttons.push_back(new Button("down", 340, 225, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "intelligence"));
	buttons.push_back(new Button("up", 340, 270, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "dexterity"));
	buttons.push_back(new Button("down", 340, 320, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "dexterity"));
	buttons.push_back(new Button("up", 340, 365, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "constitution"));
	buttons.push_back(new Button("down", 340, 415, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "constitution"));
	buttons.push_back(new Button("up", 340, 460, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "faith"));
	buttons.push_back(new Button("down", 340, 510, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "faith"));
	buttons.push_back(new Button("start", 450, 600, 244, 95, "Images/UI/CreateScreen/StartButton.png", ""));

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
						((mouseY >= i->y) && (mouseY <= (i->y + i->h)))) {
						if (i->type == "start") {
							if (pointsToAllocate == 0) {
								if (nameInputText != "") {
									onCharacterCreate = false;
									player1.setAll(strength, intelligence, dexterity, constitution, faith, nameInputText);
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
								deltaAttribute = 1;
							}
							else {
								errorInputText = "No Points Remaining!";
								deltaAttribute = 0;
							}
						}
						else if (i->type == "down") {
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
					}
				}
			}

			else if (e.type == SDL_KEYDOWN) {
				//remove char if backspace
				if (e.key.keysym.sym == SDLK_BACKSPACE && nameInputText.length() > 0){
					nameInputText.pop_back();
				}
			}

			else if (e.type == SDL_TEXTINPUT) {
				//add char
				//set length limit to arbitrariy be 13 (fits textbox about right, depends on what user enters)
				if (nameInputText.length() < 13) {
					nameInputText += e.text.text;
				}
			}
		}

		SDL_RenderCopy(gRenderer, background, NULL, NULL);
		for (auto i : buttons) {
			SDL_RenderCopy(gRenderer, i->texture, NULL, &i->rect);
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

	SDL_Rect characterBox = { 50, 50, 200, 37*4 };
	SDL_Rect enemyBox = { 200, 200, 50, 50 };
	SDL_Texture* characterTexture = loadImage("Images/Player/Character.png");
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
	int charImageH = 37 * 4;
	int delaysPerFrame = 0;
	int frame = 0;


	SDL_Event e;
	bool inOverworld = true;
	while (inOverworld) {
		while (SDL_PollEvent(&e)) {
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

		// Try to move vertically
		characterBox.y += yVelocity;
		if (characterBox.y < 0 || (characterBox.y + characterBox.h > SCREEN_HEIGHT)) {
			//go back into window
			characterBox.y -= yVelocity;
		}

		// Try to move horizontally
		characterBox.x += xVelocity;
		if (characterBox.x < 0 || (characterBox.x + characterBox.w > SCREEN_WIDTH)) {
			//go back into window
			characterBox.x -= xVelocity;
		}

		// Clear black
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);

		charImageX = frame * 200;
		SDL_Rect charactersRectangle = { charImageX, charImageY, charImageW, charImageH};
		SDL_RenderCopy(gRenderer, characterTexture, &charactersRectangle, &characterBox);


		SDL_RenderPresent(gRenderer);
		delaysPerFrame++;
		if (delaysPerFrame >= 10) {
			frame++;
			delaysPerFrame = 0;
		}
		if (frame == 4) {
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
	return 0;
}
