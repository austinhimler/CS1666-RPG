#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;
//TEST CHANGE - BRANDON BADGER
// Function declarations
bool init();
SDL_Texture* loadImage(std::string fname);
void close();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
std::vector<SDL_Texture*> gTex;
// Music var
Mix_Music *gMusic=NULL;
bool init() {
	// Flag what subsystems to initialize
	// For now, just video
	//added audio init
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set texture filtering to linear
	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
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
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );

	// Initialize PNG loading via SDL_image extension library
	int imgFlags = IMG_INIT_PNG;
	imgFlags = imgFlags | IMG_INIT_JPG;//add jpg support
	int retFlags = IMG_Init(imgFlags);
	if(retFlags != imgFlags) {
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}
	//initialize the sdl mixer
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT,2,2048)<0)
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
	gMusic=NULL;
	// Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main() {
	if (!init()) {
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}
	//Load the music
	gMusic= Mix_LoadMUS("CREDIT_IMG/BGM.wav");
	if(gMusic==NULL)
		std::cout <<  "Failed to load music" << std::endl;
	//Play the music
	Mix_PlayMusic(gMusic,-1);
	// Everyone needs to add a line like this to add their image
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
	for(auto i: gTex)
	{
		SDL_RenderCopy(gRenderer, i, NULL, NULL);
		SDL_RenderPresent(gRenderer);
		SDL_Delay(3000);
	}
	//Stop the music
	Mix_HaltMusic();
	close();
}
