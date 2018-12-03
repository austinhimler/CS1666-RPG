#include "../Headers/CombatManager.h"
#include <stdio.h>


QueueManager::QueueManager(vector<Character *> c)
{
	for (auto C : c) {
		currTurn.push_back(C);
		nextTurn.push_back(C);
	}
	insertionSort(currTurn, currTurn.size());
	insertionSort(nextTurn, nextTurn.size());
}

QueueManager::~QueueManager()
{

}


void QueueManager::createRounds(vector<Character*> c)
{

}

void QueueManager::changeRounds()
{
	vectorCopy(currTurn, nextTurn);
	insertionSort(nextTurn, nextTurn.size());
}

void QueueManager::insertionSort(std::vector<Character*>& turn, int n)
{
	int i, j;
	Character* key;
	for (i = 1; i < n; i++)
	{
		//key = turn[i];
		j = i - 1;
		/*
		using namespace std;
		Enemy* tempp = (Enemy*)turn[j];
		cout << tempp->toString() << endl;
		cout << turn.size() << " "<< j<<endl;
		cout << turn[j]->getDex() << endl;
		cout << turn[i]->toString() << endl;
		//*/
		while (j >= 0 && turn[j]->getDex() > turn[i]->getDex())
		{
			turn[j + 1] = turn[j];
			j = j - 1;
		}
		turn[j + 1] = turn[i];
	}
}

void QueueManager::vectorCopy(vector<Character*>& cT, vector<Character*>& nT)
{
	cT.erase(cT.begin(), cT.end());
	for (int i = 0; i < nT.size(); i++)
	{
		cT.push_back(nT[i]);
	}
}

CombatManager::CombatManager()
{

}

CombatManager::~CombatManager()
{
}

int CombatManager::updateStatus() {
	//First check if character is dead
	int lc[2] = {0,0};
	for(auto& c : participants)
	{
		if (c->getHPCurrent() <= 0) continue;
		c->ailmAffect();
		if (c->getHPCurrent() == 0) {
			if (c->is_Enemy()) lc[ENEMY]++;
			else lc[PLAYER]++;
		}
		else
		{
			c->updateEnergy(nullptr);
		}
	}
	if (lc[PLAYER] == livingCount[PLAYER]) return ENEMY_WINS;
	else if (lc[ENEMY] == livingCount[ENEMY]) return PLAYER_WINS;
}
//*/

enum BattleState
{
	BATTLE,			// When normal battle messages are occuring
	MOVE_SELECTION,	// When we are awaiting the move selection
	TARGET_SELECTION, // When we are awaiting a target selection
};

BattleState m_currentState = BATTLE;

int CombatManager::textAction(Character* c) {

	vector<int> ailments;
	if (c->is_Enemy() == true)
	{
		//Enemy attack player
		std::vector<Ability> temp = c->getAbilities();
		int target = rand() % player_index.size();
		int result = participants[player_index[target]]->beingTarget(&temp[0]);
		stringstream ss;
		ss << c->getName() << " damages you by " << result << " HP!" << " You now still have " << participants[0]->getHPCurrent() << " HP left.";
		m_combatDialogManager.AddMessage(ss.str());
		
		/*if (ailments.size() == 0) 
		{ 
			//std::cout << "Their attack did not have any status effect on you." << std::endl;
			m_combatDialogManager.AddMessage("Their attack did not have any status effect on you.");
		}
		else {

		}*/
		if (participants[player_index[target]]->getHPCurrent() == 0) {
			//std::cout << "Why are your so weak? You are dead, dude!" << std::endl;
			m_combatDialogManager.AddMessage("Why are your so weak? You are dead, dude!");
			inCombat = false;
			return ENEMY_WINS;
		}
		m_combatDialogManager.ClearEvents();
	}
	else
	{

			// Get the recent events from the combat dialog manager
			std::queue<CombatDialogManager::SelectionEvent> events = m_combatDialogManager.GetEvents();
			if (events.size() > 0)
			{
				// A selection has been made
				while(events.size() > 0)
				{
					// Get the first event
					auto event = events.front();
					// Pop the event 
					events.pop();
					int status = performEvent(c, event.options[event.selectedOption], event.selectedOption);
					if (status != 1)
						return status;
					// We'll just reprint the main move selection for now

					bool print = false;
					textMain(print, initialText);
				}
			}
			// Clear the events
			m_combatDialogManager.ClearEvents();

			/*
			int action = -1;
			std::vector<int> index_helper = enemy_index;

			// Originally you had a while loop here that waited for user input.
			// You cannot idle like this. We need to keep rendering while we wait for input, otherwise
			// the OS will think that the application has become unresponsive.
			// Potential solutions would be to have some sort of battle state machine
			// You will need to rewrite the following battle code in this function to do this.
			// I started and realised you'd be better off doing it yourself, you understand it more :)

			while (action <= 0 || action > 8) {
				std::cin >> action;
			}

			int movementAction = 0;//m_combatDialog.GetCurrentlySelectedOption(MOVE_SELECTION); // returns -1 if hasnt displayed yet. returns action num if displayed
			
			
			if (movementAction == -1 && input received)
			{
				//m_combatDialog.AddMessage("Which one? Select by option number NUM 2");
				//std::cout << "Which one? Select by option number" << std::endl;
				int char_selection;
				switch (action) {
				case 6:
					m_combatDialogManager.AddMessage("Just kidding. You don't have SHEETz coffee here");
					//std::cout << "Just kidding. You don't have SHEETz coffee here\n" << std::endl;
					break;
				case 7:
				case 8:
					if (action == 7) index_helper = player_index;
					for (int i = 0; i < index_helper.size(); i++) {
						std::cout << i + 1 << ". " << participants[index_helper[i]]->getName() << std::endl;
					}
					char_selection = -1;
					while (char_selection <= 0 || char_selection > index_helper.size()) {
						std::cin >> char_selection;
					}
					char_selection--;
					std::cout << participants[index_helper[char_selection]]->toString() << std::endl;
					break;
				default: // if an attribute is selected
					std::vector<Ability> abil_temp = c->getAbilities(); // get ability lists of the character
					std::vector<int> helper; // stores relative index of the abilites within the same attribute category
					int k = 1;
					for (int i = 0; i < abil_temp.size(); i++) {
						if (action == AbilityResource::abilityAttr[abil_temp[i].getName()][0] + 1) { //if the ability is of current attribute
							std::cout << k << ". " << AbilityResource::abilityNames[abil_temp[i].getName()] << std::endl; // print as an option
							helper.push_back(i); // stores index to helper vector
						}
					}
					// get ability selection
					int abil_selection = -1;
					Ability& a_cur = abil_temp[0];
					while (true) {
						std::cin >> abil_selection;
						abil_selection--;

						std::cout << AbilityResource::abilityNames[action].size() << std::endl;

						if (abil_selection >= 0 && abil_selection < helper.size()) { // if a valid selection
							a_cur = abil_temp[helper[abil_selection]];
							if (c->getEnergyCurrent() < a_cur.getEnergyCost()) { // if not enough energy
								std::cout << AbilityResource::abilityNames[a_cur.getName()] << " needs " << a_cur.getEnergyCost() << ", you only have " << c->getEnergyCurrent() << " YOU FOOL" << std::endl;
							}
							else break;
						}
					}

					c->updateEnergy(&a_cur);


					switch (abil_temp[helper[abil_selection]].getType()) {
					case AbilityResource::tSUMMON:
						std::cout << "NLF4 is lecturing, can't make it." << std::endl;
						break;
					case AbilityResource::tESCAPE:
						if (c->beingTarget(&abil_temp[helper[abil_selection]]) == -2) {
							takingAction = false;
							inCombat = false;
							std::cout << "You escape succesfully COWARD!" << std::endl;
							livingCount[PLAYER]--;
							return PLAYER_ESCAPES;
						}
						else {
							std::cout << "You are not going anywhere." << std::endl;
							takingAction = false;
						}
						break;
					case AbilityResource::tDEFENSE:
						c->beingTarget(&abil_temp[helper[abil_selection]]);
						std::cout << "Your Energy Regeneration for next round will be increased you tactical fool" << std::endl;
						takingAction = false;
						break;
					default:
						std::cout << "Pick your target" << std::endl;
						outputEnemy(); // output all surviving enemies
						// get target selection
						int target = -1;
						while (target <= 0 || target > enemy_index.size()) {
							std::cin >> target;
							if (participants[enemy_index[target]]->getHPCurrent() == 0) {
								std::cout << "That eneny is dead." << std::endl;
								target = -1;
							}
						}
						target--;
						int result = participants[enemy_index[target]]->beingTarget(&abil_temp[helper[abil_selection]]);
						if (participants[enemy_index[target]]->getHPCurrent() == 0) {
							livingCount[ENEMY]--;
						}
						std::cout << "You damage " << participants[enemy_index[target]]->getName() << " amazingly by " << result << " HP!" << " " << participants[enemy_index[target]]->getName() << " now has only " << participants[enemy_index[target]]->getHPCurrent() << " HP left." << std::endl;
						break;
					}
					break;
				}
				if (c->getEnergyCurrent() == 0) {
					takingAction = false;
					return IN_COMBAT;
				}
				bool temp = false;
				for (auto& i : enemy_index) { // check whether at least 1 enemy survives
					if (participants[i]->getHPCurrent() != 0) temp = true;
				}
				if (!temp) {
					std::cout << "You win" << std::endl;
					takingAction = false;
					inCombat = false;
					break;
				}
				if (takingAction) {
					std::cout << "MENU 2: What else do you want to do? Select by option number." << std::endl;
					std::cout << "What do you want to do? Select by option number" << std::endl;
					std::cout << "1. Strength Abilities" << std::endl;
					std::cout << "2. Intelligent Abilities" << std::endl;
					std::cout << "3. Dexerity Abilities" << std::endl;
					std::cout << "4. Constitution Abilities" << std::endl;
					std::cout << "5. Faith Abilities" << std::endl;
					std::cout << "6. Inventory" << std::endl;
					std::cout << "7. Player Stats" << std::endl;
					std::cout << "8. Enemy Stats" << std::endl;
				}
			}
			*/
	}

	return IN_COMBAT;
}

int CombatManager::performEvent(Character *c, string option, int optNum)
{
	int returnVal = 1;
	// Simple output message
	stringstream ss;
	ss << "You selected " << option;
	m_combatDialogManager.AddMessage(ss.str());
	switch (optNum) {
	case 5:
		returnVal = PLAYER_ESCAPES;
		break;
	default:
		textAttributes(c, optNum);
		outputEnemy();
		break;
	}
	
	m_combatDialogManager.ClearEvents();
	return returnVal;
}


int CombatManager::takeAction(Character* c, std::vector<Button *> buttons, SDL_Event e) {
	/*
		If c is an emeny, do enemy attack
		Else, wait for user input
		If mouse hovers over player or enemy, display stats in info screen
		If attribute/inventory buttons are clicked, display options in info screen
		If a skill is clicked wait for user to click on character to use the skill on
		If energy = 0 or player clicks End Turn, return
	

	int charImageX = 0;
	int charImageY = 0;
	int charImageW = 200;
	int charImageH = 148;

	if (c->is_Enemy() == true)
	{
		//Enemy attack player
	}
	else
	{
		bool takingAction = true;
		while (takingAction) {
			

			while (SDL_PollEvent(&e))
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				
				for (auto i : buttons)
				{
					if (((mouseX >= i->x) && (mouseX <= (i->x + i->w))) &&
						((mouseY >= i->y) && (mouseY <= (i->y + i->h))))
					{
						if (i->type == "character")
						{
							//Display character info in info sheet
						}
						else if (e.button.button == (SDL_BUTTON_LEFT) && e.type == SDL_MOUSEBUTTONDOWN)
						{
							bool abil_button = true;
							
							/*
							while (abil_button) {
								std::vector<Button*> Abil_Buttons;
								setNewButtons(Abil_Buttons, i->attribute);
								
								
								if(Escape button is clicked) {
								Ability* a = c->abilities[c->getHelp(ESCAPE)];
									if(c->beingTarget(a) == -1) {
										// escape successfully
										// play animation
										// end combat
									}
									else {
										//escape failed
										//play animation
									}
								}
								if(Ability buttons are clicked) {
									Ability* a = c->abilities[c->getHelp(the abilities whose button is clicked)];
									// create/activate character buttons
									while(Character buttons are clicked) {
										Character* target = the character whose button is clicked
										int result = target->beingTarget(a); // result gives the how many hp increased or decresed
									}
								}
							
							}//
						}
					}
				}
			}//

			if (c->getEnergyCurrent() == 0) {
				takingAction = false;
				break;
			}
			bool temp = false;
			for (auto& i : enemy_index) { // check whether at least 1 enemy survives
				if (participants[i]->getHPCurrent() != 0) temp = true;
			}
			if (!temp) {
				takingAction = false;
				inCombat = false;
				break;
			}
		}
	}

	return IN_COMBAT; 
	*/
	return IN_COMBAT;

}

/*
// t gives which of the intial 6 buttons are pressed
void CombatManager::setNewButtons(std::vector<Button*>& buttons, int t) {
	int bw = 100, bh = 50;
	int num_per_row = 1, num_per_col = 1;
	if (t >= STR && t <= FAI) { // if the button pressed is an attribute button
		for (int i = 0; i < AbilityResource::abilityNames[t].size(); i++) {
			buttons.push_back(new Button("Abilty", info_box.x + 5 * (i%num_per_row * 50), info_box.y + 10 * ((i / num_per_row) % num_per_col * 50), bw, bh, "Images/UI/CombatScene/Button.png", AbilityResource::abilityNames[t][i], gRenderer));
		}
	}
	else { // if the button is the inventory button
		// create button for every item in inveotry
	}
}	//*/

void CombatManager::outputEnemy() {
	/*int i = 1;
	for (int j = 0; j < participants.size(); j++) {
		if ((Enemy*)participants[j]->is_Enemy())
			std::cout << i++ << ". " << participants[j]->getName() << std::endl;
	}*/
	std::vector<std::string> options;
	for (int j = 0; j < participants.size(); j++) {
		if ((Enemy*)participants[j]->is_Enemy())
			options.push_back(participants[j]->getName());
	}
	m_combatDialogManager.AddSelectableOption("Choose your target", options);
	turnOrder = 2;
}
void CombatManager::textAttributes(Character *c, int optNum)
{
	std::vector<std::string> options;
	std::vector<Ability> abil_temp = c->getAbilities(); // get ability lists of the character
	std::vector<int> helper; // stores relative index of the abilites within the same attribute category
	int k = 1;
	for (int i = 0; i < abil_temp.size(); i++) {
		if (optNum == AbilityResource::abilityAttr[abil_temp[i].getName()][0]) { //if the ability is of current attribute
			//std::cout << k << ". " << AbilityResource::abilityNames[abil_temp[i].getName()] << std::endl; // print as an option
			options.push_back(AbilityResource::abilityNames[abil_temp[i].getName()]);
			helper.push_back(i); // stores index to helper vector
		}
	}
	m_combatDialogManager.AddSelectableOption("Choose your attack", options);
	
}
void CombatManager::textMain(bool& printed, bool initialText) {
	if (printed) return;
	if (initialText)
		m_combatDialogManager.AddMessage("You've encountered the colored cone of death!");
	std::vector<std::string> options;
	options.push_back("Strength Abilities");
	options.push_back("Intelligence Abilities");
	options.push_back("Dexerity Abilities");
	options.push_back("Constitution Abilities");
	options.push_back("Faith Abilities");	
	options.push_back("Escape");
	
	m_combatDialogManager.AddSelectableOption("What do you want to do?", options);
	printed = true;
}

int CombatManager::combatMain(std::vector<Character*>& p) 
{
	initialText = true;
	participants = p;

	std::cerr << participants.size() << std::endl;
	if (participants[0] != nullptr) {
		std::cerr << participants[0]->toString() << std::endl;
	}

	//initialize enemy_index and player_index
	for (int i = 0; i < participants.size(); i++) {
		if (participants[i]->is_Enemy()) {
			enemy_index.push_back(i);
			livingCount[ENEMY]++;
		}
		else {
			player_index.push_back(i);
			livingCount[PLAYER]++;
		}
	}

	int charImageX = 0;
	int charImageY = 0;
	int charImageW = 128;
	int charImageH = 128;
	SDL_Rect characterBox = { 472, 225, 128, 128 };
	int charAnimationPixelShift = 128;
	int delaysPerFrame = 0;
	int frame = 0;
	
	/**
	*	Combat Manager - Start Battle:
						get participants
						display combat UI
						pre-combat effect calcuation
						create queue
						start pre-turn of first from queue
						*/

	// Create Enemy within combat class. Could be subject to change 
	// Set up a Character array and populate it (not sorted by dex) 
	//Character participants[2];
	//participants[0] = (Character*)(new Player("nlf4", 1, 1, 1, 1,1));
	//participants[1] = (Character*)(new Enemy());
	
	inCombat = true;

	vector<int> ailments;
	// Create QueueManager obj which contains sorting of participant array. 
	QueueManager qm = QueueManager(participants);
	//LoadTexture background;
	SDL_Event e;
	//background.loadFromFile("Images/UI/CombatScene/combatScene.png");
	
	TTF_Font* font = Helper::setFont("Fonts/Stacked pixel.ttf", 25);
	SDL_Color txt_color = {0,0,0,0};
	
	int bw = 100;
	int bh = 50;	

	bool printed = false; // for text combat ui

	//OpenGL Setup
	glewInit();

	// We shouldn't initialize the graphics class every time we run into a battle. Id recommend initializing it once, then using it
	// Same goes for the dialog manager.
	// I'll leave this up to you to do
	m_combatGraphics.init();
	int player = m_combatGraphics.genQuadTexture(144, 144, "Images/Player/Idle_Down.png", "player", 0, 0, 6, 1);
	m_combatGraphics.translateObjectByPixel(player, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3, 0.0);
	int enemy = m_combatGraphics.genQuadTexture(144, 144, "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png", "owl", 0, 0, 6, 1);
	m_combatGraphics.translateObjectByPixel(enemy, 4 * SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3, 0.0);
	m_combatGraphics.genCone(0.25, 0.5, 36, 1, glm::vec4(0.7, 0.7, 0.7, 0.0));
	//m_combatGraphics.genSphere(0.5, 36, 0, glm::vec4());
	//m_combatGraphics.genCube(0, glm::vec4());

	// Set up the combat dialog manager
	m_combatDialogManager = CombatDialogManager();
	m_combatDialogManager.SetTimePerCharacter(0.02);
	m_combatDialogManager.SetWaitTime(1.0);
	m_combatDialogManager.SetColor(glm::vec4(0.0, 0.0, 0.0, 1.0));
	m_combatDialogManager.SetSelectionColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
	m_combatDialogManager.SetFont(ResourceManager::getFontData("stacked_pixel"));

	m_combatGraphics.display();
	m_combatGraphics.rotateRandom();
	

	int width, height;
	
	while (inCombat) {
		
		// Update the combat dialog manager
		// We need to know the time between frames so we can update things accordingly. We'll just pass in a set number for now. 
		// In the future you should pass in the update delta time.
		m_combatDialogManager.Update(1.0 / 60.0);

		// Add the renderable texts generated from the combat dialog manager to the renderer
		m_combatGraphics.addTextsToRender(m_combatDialogManager.GetTextToRender());

		// This isnt a great way to idle with 3d graphics, you'll run into a host of issues.
		// You should only be updating the screen every 1/60 of a second (for a 60fps game)
		// and updating and interpolating based on the delta time
		// You can keep this for now but it will probably have to change down the road
		m_combatGraphics.idle();
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				Mix_FreeChunk(gBSound);
				//Random integer for quitting the game
				return -69; 
			}
			gBSound = Mix_LoadWAV("Audio/BSound.wav");

			// Sent inputs to the combat dialog manager
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d)
				{
					m_combatDialogManager.RegisterKeyInput(CombatDialogManager::DialogInput::RIGHT);
					Mix_PlayChannel(-1, gBSound, 0);
				}
				
				if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a)
				{
					m_combatDialogManager.RegisterKeyInput(CombatDialogManager::DialogInput::LEFT);
					Mix_PlayChannel(-1, gBSound, 0);
				}
				
				if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w)
				{
					m_combatDialogManager.RegisterKeyInput(CombatDialogManager::DialogInput::UP);
					Mix_PlayChannel(-1, gBSound, 0);
				}
				
				if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s)
				{
					m_combatDialogManager.RegisterKeyInput(CombatDialogManager::DialogInput::DOWN);
					Mix_PlayChannel(-1, gBSound, 0);
				}
				
				if (e.key.keysym.sym == SDLK_RETURN)
				{
					allPlayersMoved = true;
					m_combatDialogManager.RegisterKeyInput(CombatDialogManager::DialogInput::SELECT);
					Mix_PlayChannel(-1, gBSound, 0);
				}
				
			}


			/*
			background.renderBackground(gRenderer);
			delaysPerFrame++;
			if (delaysPerFrame >= 6) {
				frame++;
				delaysPerFrame = 0;
			}
			if (frame == 4) {
				frame = 0;
			}
			charImageX = frame * charAnimationPixelShift;
			for (auto i : buttons) {
				if (i->type == "character")	// render participants
				{
					SDL_Rect charactersRectangle = { charImageX, charImageY, charImageW, charImageH };
					SDL_RenderCopy(gRenderer, i->texture, &charactersRectangle, &(i->rect));
				}
				else // render UI in initial state
				{
					// render buttons on the buttom
					SDL_RenderCopy(gRenderer, i->texture, NULL, &(i->rect));
					Helper::renderText(i->type.c_str(), &(i->rect), &txt_color, font);
				}
			}
			//SDL_RenderPresent(gRenderer);

			SDL_Delay(16);*/
		}
		
		if (turnOrder == 0)
		{
			textMain(printed, initialText); // text combat ui initialization
			turnOrder = 1;
		}
		
		initialText = false;
		// We need to fix the idle for the following part
		// You will need to rewrite to not have an while loops
		// We need to constantly render to the screen
		if (turnOrder == 1)
		{
			if (e.key.keysym.sym == SDLK_RETURN)
			{
				switch (int result_temp = textAction(participants[0])) {
				case IN_COMBAT:
					break;
				default:
					Mix_FreeChunk(gBSound);
					return result_temp;
					//takeAction(participants[i], buttons, e);
				}
				updateStatus();
			
			}
		}
		else if (turnOrder == 2)
		{
			for (int i = 1; i < participants.size(); i++)
			{
				//updateStatus(participants[i]);
				if (participants[i]->getHPCurrent() != 0 && participants[i]->getEnergyCurrent() != 0 && allPlayersMoved)
					switch (int result_temp = textAction(participants[i])) {
					case IN_COMBAT:
						break;
					default:
						Mix_FreeChunk(gBSound);
						return result_temp;
						//takeAction(participants[i], buttons, e);
					}
				updateStatus();
			}
			allPlayersMoved = false;
			turnOrder = 0;
		}
		
		//printed = false; // for text combat ui
		qm.changeRounds();

	}
	Mix_FreeChunk(gBSound);
	return -100;

}
