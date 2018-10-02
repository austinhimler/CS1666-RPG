#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

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
	//Free music
	Mix_FreeMusic(gMusic);
	gMusic = NULL;
	// Quit SDL subsystems
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

class Stat {

};

void characterCreateScreen() {
	bool onCharacterCreate = true;
	int pointsToAllocate = 20;
	int maxStat = 10;
	int minStat = 1;
	int strength = 1;
	int intelligence = 1;
	int dexterity = 1;
	int constitution = 1;
	int faith = 1;
	std::vector<Button*> buttons;
	//220 60 textpos for points
																			//need attr objects
	buttons.push_back(new Button("up", 340, 90, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "strength"));
	buttons.push_back(new Button("down", 340, 135, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "strength"));
	buttons.push_back(new Button("up", 340, 185, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "intelligence"));
	buttons.push_back(new Button("down", 340, 230, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "intelligence"));
	buttons.push_back(new Button("up", 340, 275, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "dexterity"));
	buttons.push_back(new Button("down", 340, 310, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "dexterity"));
	buttons.push_back(new Button("up", 340, 360, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "constitution"));
	buttons.push_back(new Button("down", 340, 405, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "constitution"));
	buttons.push_back(new Button("up", 340, 455, 46, 51, "Images/UI/CreateScreen/pointUpArrow.png", "faith"));
	buttons.push_back(new Button("down", 340, 500, 46, 51, "Images/UI/CreateScreen/pointDownArrow.png", "faith"));
	buttons.push_back(new Button("start", 450, 600, 244, 95, "Images/UI/CreateScreen/StartButton.png", ""));
	SDL_Texture* background = loadImage("Images/UI/CreateScreen/characterCreateV2NoButtons.png"); //Moved to fix memory leak
	SDL_Event e;
	
	while (onCharacterCreate) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				onCharacterCreate = false;
				
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
								onCharacterCreate = false;
								//make Character Object, validate, return to main
								return;
							}
							else {
								break; //not valid to start, break out of for loop
							}
						}

						if (i->type == "up") {
							if (pointsToAllocate > 0) {
								deltaAttribute = 1;
							}
							else {
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
						}
						else if (i->attribute == "intelligence") {
							if ((deltaAttribute + intelligence) <= maxStat && (deltaAttribute + intelligence) >= minStat) {
								intelligence += deltaAttribute;
								pointsToAllocate -= deltaAttribute;
							}
						}
						else if (i->attribute == "dexterity") {
							if ((deltaAttribute + dexterity) <= maxStat && (deltaAttribute + dexterity) >= minStat) {
								dexterity += deltaAttribute;
								pointsToAllocate -= deltaAttribute;
							}
						}
						else if (i->attribute == "constitution") {
							if ((deltaAttribute + constitution) <= maxStat && (deltaAttribute + constitution) >= minStat) {
								constitution += deltaAttribute;
								pointsToAllocate -= deltaAttribute;
							}
						}
						else if (i->attribute == "faith") {
							if ((deltaAttribute + faith) <= maxStat && (deltaAttribute + faith) >= minStat) {
								faith += deltaAttribute;
								pointsToAllocate -= deltaAttribute;
							}
						}
					}
				}
			}
		}

		SDL_RenderCopy(gRenderer, background, NULL, NULL);
	for (auto i : buttons) {
		SDL_RenderCopy(gRenderer, i->texture, NULL, &i->rect);
	}
	SDL_RenderPresent(gRenderer);
	
		SDL_Delay(16);

		//onCharacterCreate = false;
	}
	//return when player hit creates and does it correctly (has valid playerName and attributesAllocated)
	// 30(?) points to allocate, for each attribute minimum points is 1, max is 10 
}

void playGame() {
	// while(gameOn) gameloop
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
	characterCreateScreen();
	playGame();
	playCredits();
	close();
	return 0;
}
