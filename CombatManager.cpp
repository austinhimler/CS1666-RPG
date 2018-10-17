#include "Headers/CombatManager.h"
#include "Headers/LoadTexture.h"



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

/*
void updateStatus(Character& c) {
	//First check if character is dead
	if (c.getHPCur() <= 0) {
		gameOn = false;
	}
	// Next check the ailments of given character
	else {
		if (ailments.size() != 0) {
			for (int i = 0; i < ailments.size(); i++) {
				if (ailments[i] == 0) {
					silenced(c);
				}
				else if (ailments[i] == 1) {
					poisoned(c);
				}
			}
		}
	}
}
*/

void CombatManager::takeAction(Character* c, std::vector<Button *> buttons, SDL_Event e) {
	/*
		If c is an emeny, do enemy attack
		Else, wait for user input
		If mouse hovers over player or enemy, display stats in info screen
		If attribute/inventory buttons are clicked, display options in info screen
		If a skill is clicked wait for user to click on character to use the skill on
		If energy = 0 or player clicks End Turn, return
	*/

	int charImageX = 0;
	int charImageY = 0;
	int charImageW = 200;
	int charImageH = 148;

	if (c->isEnemy == true)
	{
		//Enemy attack player
	}
	else
	{
		bool takingAction = true;
		while(takingAction) {
			if (c->getEnergyCurrent() == 0) {
				takingAction = false;
				break;
			}
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

						if (e.button.button == (SDL_BUTTON_LEFT) && e.type == SDL_MOUSEBUTTONDOWN && (i->type == "button"))
						{
							bool abil_button = true;
							/*while (abil_button) {
								//Display buttons in info sheet

								
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
							
							}*/
						}
					}
				}
			}
		}
	}

	



}
void silenced(Character& c) {
	// silence effects
}

void poisoned(Character& c) {
	//poison effects
}

bool CombatManager::combatManager(std::vector<Character*> p) 
{
	
	using namespace std;
	Enemy* tempp = (Enemy*)p[1];
	cout << tempp->toString() << endl;
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
	//p[0] = (Character*)(new Player("nlf4", 1, 1, 1, 1,1));
	//p[1] = (Character*)(new Enemy());
	
	gameOn = true;

	vector<int> ailments;
	// Create QueueManager obj which contains sorting of participant array. 
	QueueManager qm = QueueManager(p);
	LoadTexture background;
	SDL_Event e;
	background.loadFromFile("Images/UI/CombatScene/combatScene.png");
	SDL_Rect scene_box = { 0,0,720,540 };
	SDL_Rect ui_box = { 17,529,685,167 };

	int bw = 100;
	int bh = 50;
	std::vector<Button*> buttons;
		buttons.push_back(new Button("player", scene_box.x + 10, scene_box.y + 200, bw, bh, "Images/Player/Player_Idle.png", "Player", gRenderer));
		buttons.push_back(new Button("player", scene_box.x + 500, scene_box.y + 200, bw, bh, "Images/Enemies/shadow_cluster/owl.png", "Player", gRenderer));
		buttons.push_back(new Button("attack", ui_box.x , ui_box.y + 10, bw, bh, "Images/UI/CombatScene/Button.png", "Enemy", gRenderer));
		buttons.push_back(new Button("attack", ui_box.x , ui_box.y + 60, bw, bh, "Images/UI/CombatScene/Button.png", "Attack", gRenderer));
		buttons.push_back(new Button("defend", ui_box.x , ui_box.y + 110, bw, bh, "Images/UI/CombatScene/Button.png", "Defend", gRenderer));
		
		
		
	while (gameOn) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				background.free();
				return false; 
			}
		
		background.renderBackground();
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
			if (i->type == "player")
			{
				SDL_Rect charactersRectangle = { charImageX, charImageY, charImageW, charImageH };
				SDL_RenderCopy(gRenderer, i->texture, &charactersRectangle, &(i->rect));
			}
			else
				SDL_RenderCopy(gRenderer, i->texture, NULL, &(i->rect));
		}
		SDL_RenderPresent(gRenderer);
		SDL_Delay(16);
		}
		for (int i = 0; i < participants.size(); i++)
		{
			//updateStatus(participants[i]);
			takeAction(participants[i], buttons, e);
		}
		qm.changeRounds();
	}

	//Test to check that the background appears
	/*
	while (gameOn)
	{
		printf("Keep Fighting");
	}//*/
	/*
	while (gameOn)
	{
		for (int i = 0; i < participants.size(); i++)
		{
			//updateStatus(participants[i]);
			takeAction(participants[i], buttons, e);
		}
		qm.changeRounds();
	}//*/
	return false;
	
	 
	

}




