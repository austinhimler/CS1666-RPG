#include "../Headers/CombatManager.h"
#include <stdio.h>

QueueManager::QueueManager(vector<Character *> c)
{
	for (auto C : c) {
		currTurn.push_back(C);
		nextTurn.push_back(C);
	}
	insertionSort(currTurn, (int)currTurn.size());
	insertionSort(nextTurn, (int)nextTurn.size());
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

int CombatManager::checkCombatStatus() {
	if (livingCount[PLAYER] == 0) return ENEMY_WINS;
	else if (livingCount[ENEMY] == 0) return PLAYER_WINS;
	else return IN_COMBAT;
}

int CombatManager::updateStatus() {
	//First check if character is dead
	int lc[2] = {0,0};
	for(int i = 0; i < participants.size(); i++)
	{
		Character* c = participants[i];
		if (c->getHPCurrent() <= 0 || ParticipantsStatus[i] != IN_COMBAT) continue;
		c->ailmAffect();
		if (c->getHPCurrent() <= 0) {
			if (c->is_Enemy()) livingCount[ENEMY]--;
			else livingCount[PLAYER]--;
		}
		else
		{
			c->updateEnergy(nullptr);
		}
	}
	return checkCombatStatus();
}

Action CombatManager::ActionByAI(Character* c, int EnemyActionOrderCount) {
	std::vector<Player*> Players;
	std::vector<Enemy*> Friends;
	for (int i = 0; i < participants.size(); i++) {
		if (i != enemy_index[EnemyActionOrderCount] && participants[i]->getHPCurrent() > 0) {
			if (participants[i]->is_Enemy()) Friends.push_back((Enemy*)participants[i]);
			else Players.push_back((Player*)participants[i]);
		}
	}
	AI = CombatAI((Enemy*)c, Players, Friends);
	return AI.getBestAction();
}
enum BattleState
{
	BATTLE,			// When normal battle messages are occuring
	MOVE_SELECTION,	// When we are awaiting the move selection
	TARGET_SELECTION, // When we are awaiting a target selection
};

BattleState m_currentState = BATTLE;

int CombatManager::takeActionByAI(Character* c, int EnemyActionOrderCount) {
	while (c->getEnergyCurrent() != 0 && ParticipantsStatus[enemy_index[EnemyActionOrderCount]] == IN_COMBAT) {
		// AI decides which action to take
		Action ActionToTake = ActionByAI(c, EnemyActionOrderCount);
		std::cout << c->toString() << std::endl;
		// Carry out action and out put result for every target
		std::vector<Character*> tars = ActionToTake.getTar();
		int TarNum = tars.size();
		Ability* abil = ActionToTake.getAbil();
		std::cout << c->getEnergyCurrent() << " " << abil->getEnergyCost() << std::endl;
		if (c->getEnergyCurrent() < abil->getEnergyCost()) {
			m_combatDialogManager.ClearEvents();
			return IN_COMBAT;
		}
		else {
			for (int i = 0; i < TarNum; i++) { // act on every target and output result
				int result = tars[i]->beingTarget(abil);
				c->updateEnergy(abil);
				// output ability name
				//output target
				switch (abil->getType()) {
					using namespace AbilityResource;
				case tSUMMON:
				case tDEFENSE:
				case tESCAPE:
					m_combatDialogManager.AddMessage(c->getName() + " uses " + AbilityResource::abilityNames[abil->getName()]);
					break;
				default:
					m_combatDialogManager.AddMessage(c->getName() + " uses " + AbilityResource::abilityNames[abil->getName()] + " to " + tars[i]->getName());
					break;
				}
				// display text
				m_combatDialogManager.Update(1.0f / 60.0f);
				m_combatGraphics.addTextsToRender(m_combatDialogManager.GetTextToRender());
				m_combatGraphics.idle();
				SDL_Delay(60);
				// output impact
				stringstream stmp;
				switch (abil->getType()) {
				case AbilityResource::tDAMAGE:
					stmp << tars[i]->getName() + "'s HP is decreased by " + std::to_string(result) + "! ";
					stmp << tars[i]->getName() + " now has " + std::to_string(participants[0]->getHPCurrent()) + " HP left.";
					m_combatDialogManager.AddMessage(stmp.str());
					break;
				case AbilityResource::tSUMMON:
					stmp << "NLF4 is lecturing, can't make it.";
					m_combatDialogManager.AddMessage(stmp.str());
					break;
				case AbilityResource::tESCAPE:
					if (result == -2) {
						stmp << c->getName() + " has escaped from combat!";
						m_combatDialogManager.AddMessage(stmp.str());
						ParticipantsStatus[enemy_index[EnemyActionOrderCount]] = ESCAPED;
						livingCount[ENEMY]--;
						if (livingCount[ENEMY] <= 0) {
							// display text before returning
							m_combatDialogManager.Update(1.0f / 60.0f);
							m_combatGraphics.addTextsToRender(m_combatDialogManager.GetTextToRender());
							m_combatGraphics.idle();
							SDL_Delay(60);
							m_combatDialogManager.ClearEvents();
							return PLAYER_WINS;
						}
					}
					else {
						stmp << c->getName() + " tried to escape but failed.";
						m_combatDialogManager.AddMessage(stmp.str());
					}
					break;
				case AbilityResource::tDEFENSE:
					// display text before returning
					stmp << c->getName() + "'s Energy Regeneration for next round will be increased.";
					m_combatDialogManager.AddMessage(stmp.str());
					m_combatDialogManager.Update(1.0f / 60.0f);
					m_combatGraphics.addTextsToRender(m_combatDialogManager.GetTextToRender());
					m_combatGraphics.idle();
					SDL_Delay(60);
					m_combatDialogManager.ClearEvents();
					return IN_COMBAT;
					break;
				default:
					break;
				}
				m_combatDialogManager.Update(1.0f / 60.0f);
				m_combatGraphics.addTextsToRender(m_combatDialogManager.GetTextToRender());
				m_combatGraphics.idle();

				// check if the target is dead
				if (tars[i]->getHPCurrent() == 0) {
					stmp << tars[i]->getName() + " is dead!";
					m_combatDialogManager.AddMessage(stmp.str());
					m_combatDialogManager.Update(1.0f / 60.0f);
					m_combatGraphics.addTextsToRender(m_combatDialogManager.GetTextToRender());
					m_combatGraphics.idle();
					SDL_Delay(60);

					if (!tars[i]->is_Enemy()) {
						livingCount[PLAYER]--;
						for (auto& pi : player_index) {
							if (participants[pi] == tars[i]) {
								ParticipantsStatus[pi] = DEAD;
								break;
							}
						}

					}
					else {
						livingCount[ENEMY]--;
						for (auto& pi : enemy_index) {
							if (participants[pi] == tars[i]) {
								ParticipantsStatus[pi] = DEAD;
								break;
							}
						}
					}

					int temp_status = checkCombatStatus();
					if (temp_status != IN_COMBAT) {
						m_combatDialogManager.ClearEvents();
						return temp_status;
					}
				}
			}
		}
		//*/
			/*
			//Enemy attack player
			std::vector<Ability> temp = c->getAbilities();
			int target = rand() % player_index.size();
			int result = participants[player_index[target]]->beingTarget(&temp[0]);
			stringstream ss;
			ss << c->getName() << " damages you by " << result << " HP!" << " You now still have " << participants[0]->getHPCurrent() << " HP left.";
			m_combatDialogManager.AddMessage(ss.str());
			//*/
			/*if (ailments.size() == 0)
			{
				//std::cout << "Their attack did not have any status effect on you." << std::endl;
				m_combatDialogManager.AddMessage("Their attack did not have any status effect on you.");
			}
			else {

			}*/
	}

	m_combatDialogManager.ClearEvents();
	return IN_COMBAT;
}

int CombatManager::textAction(Character* c) {

	vector<int> ailments;
	if (c->is_Enemy() == true)
	{
		/*while (c->getEnergyCurrent() != 0 && ParticipantsStatus[enemy_index[EnemyActionOrderCount]] == IN_COMBAT) {
			//Enemy takes action

			// AI decides which action to take 
			Action ActionToTake = ActionByAI(c, EnemyActionOrderCount);
			// Carry out action and out put result for every target 
			std::vector<Character*> tars = ActionToTake.getTar();
			int TarNum = tars.size();
			Ability* abil = ActionToTake.getAbil();
			for (int i = 0; i < TarNum; i++) { // act on every target and output result
				int result = tars[i]->beingTarget(abil);
				c->updateEnergy(abil);
				// output to console 
				// output ability name
				std::cout << c->getName() << " uses " << AbilityResource::abilityNames[abil->getName()] << std::endl;
				//output target
				switch (abil->getType()) {
					using namespace AbilityResource;
				case tSUMMON:
				case tDEFENSE:
				case tESCAPE:
					break;
				default:
					std::cout << " to " << tars[i]->getName() << std::endl;
					break;
				}
				// output impact
				switch (abil->getType()) {
				case AbilityResource::tDAMAGE:
					std::cout << tars[i]->getName() << "'s HP is decreased by " << result << "!" << std::endl;
					std::cout << tars[i]->getName() << " now has " << participants[0]->getHPCurrent() << " HP left." << std::endl;
					break;
				case AbilityResource::tSUMMON:
					std::cout << "NLF4 is lecturing, can't make it." << std::endl;
					break;
				case AbilityResource::tESCAPE:
					if (result == -2) {
						std::cout << c->getName() << " has escaped from combat!" << std::endl;
						ParticipantsStatus[enemy_index[EnemyActionOrderCount]] = ESCAPED;
						livingCount[ENEMY]--;
						if (livingCount[ENEMY] <= 0) {
							return PLAYER_WINS;
						}
					}
					else {
						std::cout << c->getName() << " tried to escape but failed." << std::endl;
					}
					break;
				case AbilityResource::tDEFENSE:
					std::cout << c->getName() << "'s Energy Regeneration for next round will be increased." << std::endl;
					return IN_COMBAT;
					break;
				default:
					break;
				}
				// check if the target is dead
				if (tars[i]->getHPCurrent() == 0) {
					std::cout << tars[i]->getName() << " is dead!" << std::endl;
					if (!tars[i]->is_Enemy()) {
						livingCount[PLAYER]--;
						for (auto& pi : player_index) {
							if (participants[pi] == tars[i]) {
								ParticipantsStatus[pi] = DEAD;
								break;
							}
						}
					}
					else {
						livingCount[ENEMY]--;
						for (auto& pi : enemy_index) {
							if (participants[pi] == tars[i]) {
								ParticipantsStatus[pi] = DEAD;
								break;
							}
						}
					}
					int temp_status = checkCombatStatus();
					if (temp_status != IN_COMBAT) return temp_status;
				}
			}*/
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
				if (event.options[event.selectedOption].length() > 10 || event.options[event.selectedOption] == "Escape")
				{
					int status = performEvent(c, event.options[event.selectedOption], event.selectedOption);
					if (status != 1)
						return status;
				}
				
			}
		}
		// Clear the events
		m_combatDialogManager.ClearEvents();

		/*
		bool takingAction = true;
		bool ReturningToMainMenu;
		while (takingAction) {
			ReturningToMainMenu = false;
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
		if (movementAction == -1 && input received)
		{
			//m_combatDialog.AddMessage("Which one? Select by option number NUM 2");
			std::cout << "Which one? Select by option number" << std::endl;
			int char_selection;
			switch (action) {
			case 6:
				m_combatDialogManager.AddMessage("Just kidding. You don't have SHEETz coffee here");
				std::cout << "Just kidding. You don't have SHEETz coffee here\n" << std::endl;
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
						k++;
					}
				}
				std::cout << k << ". Return to main menu" << std::endl;
				helper.push_back(abil_temp.size());
				// get ability selection
				int abil_selection = -1;
				Ability& a_cur = abil_temp[0];
				while (true) {
					std::cin >> abil_selection;
					abil_selection--;

					std::cout << AbilityResource::abilityNames[action].size() << std::endl;

					if (abil_selection >= 0 && abil_selection < helper.size()) { // if a valid selection
						if (abil_selection == helper.size() - 1) {
							ReturningToMainMenu = true;
							break;
						}
						a_cur = abil_temp[helper[abil_selection]];
						if (c->getEnergyCurrent() < a_cur.getEnergyCost()) { // if not enough energy
							std::cout << AbilityResource::abilityNames[a_cur.getName()] << " needs " << a_cur.getEnergyCost() << ", you only have " << c->getEnergyCurrent() << " YOU FOOL" << std::endl;
						}
						else break;
					}
					else {
						std::cout << "Not An Option!" << std::endl;
					}
					k = 1;
					for (int i = 0; i < abil_temp.size(); i++) {
						if (action == AbilityResource::abilityAttr[abil_temp[i].getName()][0] + 1) { //if the ability is of current attribute
							std::cout << k << ". " << AbilityResource::abilityNames[abil_temp[i].getName()] << std::endl; // print as an option
							k++;
						}
					}
					std::cout << k << ". Return to main menu" << std::endl;
				}

				if (ReturningToMainMenu) break;
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
						for (auto& pi : player_index) {
							if (participants[pi] == c) ParticipantsStatus[pi] = ESCAPED;
						}
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
					while (target < 0) {
						while (target < 0 || target >= enemy_index.size())
						{
							std::cin >> target;
							target--;
							if (target >= enemy_index.size() - 1)
								cout << "Enter a valid enemy number\n";
						}
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
			if (!ReturningToMainMenu) {
				if (c->getEnergyCurrent() == 0) {
					takingAction = false;
					return IN_COMBAT;
				}
				bool temp = false;
				for (auto& i : enemy_index) { // check whether at least 1 enemy survives
					if (participants[i]->getHPCurrent() != 0) {
						temp = true;
						break;
					}
				}
				if (!temp) {
					std::cout << "You win" << std::endl;
					takingAction = false;
					inCombat = false;
					break;
				}
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
	m_combatDialogManager.ClearEvents();
	int returnVal = 1;
	// Simple output message
	stringstream ss;
	ss << "You selected " << option;
	m_combatDialogManager.AddMessage(ss.str());
	switch (optNum) {
	case 5:
		returnVal = PLAYER_ESCAPES;
		break;
	case 3:
		//Do defense buff
		returnVal = 1;
		turnOrder = 6;
		break;
	default:
		textAttributes(c, optNum);
		turnOrder = 2;
		//outputEnemy();
		break;
	}
	return returnVal;
}

void CombatManager::outputEnemy() {
	m_combatDialogManager.ClearEvents();

	std::vector<std::string> options;
	for (int j = 0; j < participants.size(); j++) {
		if ((Enemy*)participants[j]->is_Enemy())
			options.push_back(participants[j]->getName());
	}
	m_combatDialogManager.AddSelectableOption("Choose your target", options);
	turnOrder = 4;
}

void CombatManager::textAttributes(Character *c, int optNum)
{
	//m_combatDialogManager.ClearEvents();
	std::vector<std::string> options;
	std::vector<Ability> abil_temp = c->getAbilities(); // get ability lists of the character
	std::vector<int> helper; // stores relative index of the abilites within the same attribute category
	int k = 1;
	for (int i = 0; i < abil_temp.size(); i++) {
		if (optNum == AbilityResource::abilityAttr[abil_temp[i].getName()][0]) { //if the ability is of current attribute
			abil = abil_temp[i];
			//std::cout << k << ". " << AbilityResource::abilityNames[abil_temp[i].getName()] << std::endl; // print as an option
			options.push_back(AbilityResource::abilityNames[abil_temp[i].getName()]);
			helper.push_back(i); // stores index to helper vector
		}
	}
	options.push_back("Back");
	m_combatDialogManager.AddSelectableOption("Choose your attack", options);

}

void CombatManager::textMain(bool& printed, bool initialText) {
	if (printed) return;
	if (initialText)
	{
		m_combatDialogManager.AddMessage("You've encountered the Owls!"); 
	}
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
		ParticipantsStatus.push_back(IN_COMBAT);
		if (participants[i]->is_Enemy()) {
			enemy_index.push_back(i);
			//Enemies.push_back((Enemy*)participants[i]);
			livingCount[ENEMY]++;
		}
		else {
			player_index.push_back(i);
			//Players.push_back((Player*)participants[i]);
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
	//glewInit();

	// We shouldn't initialize the graphics class every time we run into a battle. Id recommend initializing it once, then using it
	// Same goes for the dialog manager.
	// I'll leave this up to you to do
	m_combatGraphics.init();

	//Create Player with Player Texture, translate it, then set it to animate the sprite
	int player = m_combatGraphics.genQuadTexture(144, 144, "Images/Player/Idle_Down.png", "player", 0, 6);
	m_combatGraphics.translateObjectByPixel(player, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3, 0.0);
	m_combatGraphics.setIdleAnimationType(player, 1);
	//Create Enemy with Player Texture, translate it, retexture it to owl, then set it to animate the sprite
	int enemy[2];
	for (int i = 0; i < enemy_index.size(); i++)
	{
		enemy[i] = m_combatGraphics.genQuadTexture(144, 144, "Images/Player/Idle_Down.png", "player", 0, 6);
		m_combatGraphics.translateObjectByPixel(enemy[i], ((i/2) + 4) * SCREEN_WIDTH / 5, SCREEN_HEIGHT / (3 - (i)), 0.0);
		m_combatGraphics.retextureQuad(enemy[i], "Images/Enemies/shadow_cluster/OWL_BROWN_READY.png", "owl");
		m_combatGraphics.setIdleAnimationType(enemy[i], 1);
	}

	/*
	//Create cone with similar colors, then remove it
	int cone = m_combatGraphics.genCone(0.5, 1.0, 36, 2, glm::vec4(1.0, 1.0, 0.0, 0.3));
	m_combatGraphics.setIdleAnimationType(cone, 2);
	m_combatGraphics.setIdleAnimationMotion(cone, glm::rotate(0.01f, m_combatGraphics.rotateRandom()));
	m_combatGraphics.removeObject(cone);
	//Create sphere with similar colorsm then set to animate with motion
	int sphere = m_combatGraphics.genSphere(0.5, 36, 1, glm::vec4());
	m_combatGraphics.setIdleAnimationType(sphere, 2);
	m_combatGraphics.setIdleAnimationMotion(sphere, glm::rotate(0.01f, m_combatGraphics.rotateRandom()));
	//m_combatGraphics.removeObject(sphere);
	//Create a cube with random colors
	int cube = m_combatGraphics.genCube(0, glm::vec4());
	m_combatGraphics.setIdleAnimationType(cube, 2);
	m_combatGraphics.setIdleAnimationMotion(cube, glm::rotate(0.01f, m_combatGraphics.rotateRandom()));
	m_combatGraphics.removeObject(cube);
	
	int attack = m_combatGraphics.genCone(0.01, 0.1, 36, 2, glm::vec4(0.0, 0.0, 0.0, 1.0));
	m_combatGraphics.translateObjectByPixel(attack, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3, 0.0);
	m_combatGraphics.setAnimation(attack, 2);
	m_combatGraphics.setAnimationFrameMax(attack, 30);
	m_combatGraphics.transformCtm(attack, glm::rotate((float)(-M_PI / 2.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	m_combatGraphics.setAnimationMotion(attack, glm::rotate((float)(M_PI / 2.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(glm::mat4(), (1.0f / 30.f) * m_combatGraphics.getVectorFromTo(player, enemy[0])) * glm::rotate((float)(-M_PI / 2.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	*/
	// Set up the combat dialog manager
	m_combatDialogManager = CombatDialogManager();
	m_combatDialogManager.SetTimePerCharacter(0.005f);
	m_combatDialogManager.SetWaitTime(.2);
	m_combatDialogManager.SetColor(glm::vec4(0.0, 0.0, 0.0, 1.0));
	m_combatDialogManager.SetSelectionColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
	m_combatDialogManager.SetFont(ResourceManager::getFontData("stacked_pixel"));

	m_combatGraphics.display();
	m_combatGraphics.rotateRandom();
	

	int width, height;
	int allPlayers = 0;
	while (inCombat) {
		//printf("%d", turnOrder);
		// Update the combat dialog manager
		// We need to know the time between frames so we can update things accordingly. We'll just pass in a set number for now. 
		// In the future you should pass in the update delta time.
		
		
		m_combatDialogManager.Update(1.0f / 60.0f);

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
				m_combatGraphics.clean();
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
					m_combatDialogManager.RegisterKeyInput(CombatDialogManager::DialogInput::SELECT);
					Mix_PlayChannel(-1, gBSound, 0);
				}
				
			}
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
					switch (int result_temp = textAction(participants[allPlayers])) {
					case IN_COMBAT:
						break;
					default:
						stringstream ss;
						ss << "YOU LOST!";
						m_combatDialogManager.AddMessage(ss.str());
						for (int i = 0; i < 60; i++) {
							SDL_Delay(60);
							m_combatDialogManager.Update(1.0f / 60.0f);
							m_combatGraphics.addTextsToRender(m_combatDialogManager.GetTextToRender());
							m_combatGraphics.idle();
						}
						Mix_FreeChunk(gBSound);
						m_combatGraphics.clean();
						return result_temp;
					}
					switch (int result_temp = updateStatus()) {
					case IN_COMBAT:
						break;
					default:
						stringstream ss;
						ss << "YOU LOST!";
						m_combatDialogManager.AddMessage(ss.str());
						for (int i = 0; i < 60; i++) {
							SDL_Delay(60);
							m_combatDialogManager.Update(1.0f / 60.0f);
							m_combatGraphics.addTextsToRender(m_combatDialogManager.GetTextToRender());
							m_combatGraphics.idle();
						}
						Mix_FreeChunk(gBSound);
						m_combatGraphics.clean();
						return result_temp;
					}
				
			}
		}
		else if (turnOrder == 2)
		{
			std::queue<CombatDialogManager::SelectionEvent> events = m_combatDialogManager.GetEvents();
			if (events.size() > 0)
			{
				
				// Get the first event
				auto event = events.front();
				// Pop the event 
				events.pop();
				atk = event.options[event.selectedOption];
				if (atk == "Back")
				{
					turnOrder = 0;
					printed = false;
				}
				else turnOrder = 3;
			}
		}
		else if (turnOrder == 3)
		{
			stringstream ss;
			ss << "You selected " << atk;
			m_combatDialogManager.AddMessage(ss.str());
			outputEnemy();
			turnOrder = 4;
		}
		else if (turnOrder == 4)
		{
			std::queue<CombatDialogManager::SelectionEvent> events = m_combatDialogManager.GetEvents();
			if (events.size() > 0)
			{
				// Get the first event
				auto event = events.front();
				// Pop the event 
				events.pop();
				target = event.selectedOption;
				turnOrder = 5;
			}
		}
		else if (turnOrder == 5)
		{
			stringstream ss;
			//ss << "You selected " << target;
			//m_combatDialogManager.AddMessage(ss.str());

			int result = participants[enemy_index[target]]->beingTarget(&abil);
		
			if (participants[enemy_index[target]]->getHPCurrent() == 0) {
				livingCount[ENEMY]--;
			}
			
			//PLACE THE ATTACK ANIMATIONS HERE USING THE atk VARIABLE
			//std::cout << atk;
			if (atk == "Attack") {
				glm::vec3 playerToEnemyVector = m_combatGraphics.getVectorFromTo(player, enemy[target]);
				glm::vec3 enemyToPlayerVector = m_combatGraphics.getVectorFromTo(enemy[target], player);
				glm::mat4 motion;
				glm::mat4 motion2;
				motion = glm::translate(motion, (1.0f / 20.0f)* playerToEnemyVector);
				motion2 = glm::translate(motion2, (1.0f / 20.0f)*enemyToPlayerVector);
				m_combatGraphics.setAnimation(player, 4);
				glm::mat4 multiMotion[2] = { motion, motion2 };
				int maxFrames[2] = { 20,20 };
				m_combatGraphics.setAnimationMultiStep(player,2, multiMotion,maxFrames);
				for (int i = 0; i < 40; i++) {
					SDL_Delay(60);
					m_combatDialogManager.Update(1.0f / 60.0f);
					m_combatGraphics.addTextsToRender(m_combatDialogManager.GetTextToRender());
					m_combatGraphics.idle();
				}
			}
			else if (atk == "Fireball") {
				//std::cout << "Fireball Reached";
				int attack = m_combatGraphics.genSphere(0.1, 36, 2, glm::vec4(1.0, 0.345, 0.133, 0.4));
				m_combatGraphics.translateObjectByPixel(attack, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3, 0.0);
				glm::vec3 playerToEnemyVector = m_combatGraphics.getVectorFromTo(player, enemy[target]);
				glm::mat4 motion;
				motion = glm::translate(motion, (1.0f / 15.0f)* playerToEnemyVector);
				m_combatGraphics.setAnimation(attack, 2);
				m_combatGraphics.setAnimationFrameMax(attack, 15);
				m_combatGraphics.setAnimationMotion(attack, motion);
				for (int i = 0; i < 15; i++) {
					SDL_Delay(60);
					m_combatDialogManager.Update(1.0f / 60.0f);
					m_combatGraphics.addTextsToRender(m_combatDialogManager.GetTextToRender());
					m_combatGraphics.idle();
				}
			}
			else if (atk == "Smite") {
				//std::cout << "Smite Reached";
				int attack = m_combatGraphics.genSphere(0.4, 36, 2, glm::vec4(1.0, 0.9, 0.0, 0.7));
				m_combatGraphics.setIdleAnimationMotion(attack, glm::rotate(0.4f, m_combatGraphics.rotateRandom()));
				m_combatGraphics.translateObjectByPixel(attack, 0.0f, 0.0f, 0.0);
				glm::vec3 playerToEnemyVector = m_combatGraphics.getVectorFromTo(attack, enemy[target]);
				glm::mat4 motion;
				motion = glm::translate(motion, (1.0f / 90.0f)*playerToEnemyVector);
				//motion = glm::translate(glm::mat4(), )
				m_combatGraphics.setAnimation(attack, 2);
				m_combatGraphics.setAnimationFrameMax(attack, 90);
				m_combatGraphics.setAnimationMotion(attack, motion);
				for (int i = 0; i < 90; i++) {
					SDL_Delay(60);
					m_combatDialogManager.Update(1.0f / 60.0f);
					m_combatGraphics.addTextsToRender(m_combatDialogManager.GetTextToRender());
					m_combatGraphics.idle();
				}
				
			}
			else if (atk == "Arrow Shot") {
				//std::cout << "Arrow Reached";
				int attack = m_combatGraphics.genQuadTexture(144, 144, "Images/Items/arrow.png", "arrow", 0, 1);
				m_combatGraphics.translateObjectByPixel(attack, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3, 0.0);
				glm::vec3 playerToEnemyVector = m_combatGraphics.getVectorFromTo(player, enemy[target]);
				glm::mat4 motion;
				motion = glm::translate(motion, (1.0f / 8.0f)*playerToEnemyVector);
				m_combatGraphics.setAnimation(attack, 2);
				m_combatGraphics.setAnimationFrameMax(attack, 8);
				m_combatGraphics.setAnimationMotion(attack, motion);
				for (int i = 0; i < 8; i++) {
					SDL_Delay(60);
					m_combatDialogManager.Update(1.0f / 60.0f);
					m_combatGraphics.addTextsToRender(m_combatDialogManager.GetTextToRender());
					m_combatGraphics.idle();
				}
			}
			ss << "You damage " << participants[enemy_index[target]]->getName() << " by " << result << " HP!" << " " << participants[enemy_index[target]]->getName() << " now has only " << participants[enemy_index[target]]->getHPCurrent() << " HP left.";
			m_combatDialogManager.AddMessage(ss.str());
			//m_combatGraphics.setAnimation(attack, 2);
			//m_combatGraphics.setAnimationMotion(attack, motion);
			//m_combatGraphics.setAnimationFrameMax(attack, 120);

			if (allPlayers == player_index.size() - 1)
			{
				turnOrder = 6;
				allPlayers = 0;
			}
				
			else
			{
				turnOrder = 1;
				allPlayers++;
			}
		
		}
		else if (turnOrder == 6)
		{
			for (int i = player_index.size(); i < participants.size(); i++)
			{
				//updateStatus(participants[i]);
				if (participants[i]->getHPCurrent() != 0 && participants[i]->getEnergyCurrent() != 0 )
					switch (int result_temp = takeActionByAI(participants[i], i-player_index.size())) {
					case IN_COMBAT:
						break;
					default:
						stringstream ss;
						ss << "YOU WON!!!!";
						m_combatDialogManager.AddMessage(ss.str());
						for (int i = 0; i < 60; i++) {
							SDL_Delay(60);
							m_combatDialogManager.Update(1.0f / 60.0f);
							m_combatGraphics.addTextsToRender(m_combatDialogManager.GetTextToRender());
							m_combatGraphics.idle();
						}
						m_combatGraphics.clean();
						Mix_FreeChunk(gBSound);
						return result_temp;
					}
				switch (int result_temp = updateStatus()) {
				case IN_COMBAT:
					break;
				default:
					stringstream ss;
					ss << "YOU WON!!!!";
					m_combatDialogManager.AddMessage(ss.str());
					for (int i = 0; i < 60; i++) {
						SDL_Delay(60);
						m_combatDialogManager.Update(1.0f / 60.0f);
						m_combatGraphics.addTextsToRender(m_combatDialogManager.GetTextToRender());
						m_combatGraphics.idle();
					}
					m_combatGraphics.clean();
					Mix_FreeChunk(gBSound);
					return result_temp;
				}
			}
			printed = false;
			turnOrder = 0;
			qm.changeRounds();
		}
		SDL_Delay(60);
	}
	Mix_FreeChunk(gBSound);
	m_combatGraphics.clean();
	return -100;

}
