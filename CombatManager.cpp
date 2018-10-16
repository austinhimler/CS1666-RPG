#include "Headers/CombatManager.h"
#include "Headers/Player.h"
#include "Headers/Enemy.h"
#include "Headers/Button.h"
#include <vector>
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
			key = turn[i];
			j = i - 1;
			while (j >= 0 && turn[j]->getDex() > key->getDex())
			{
				turn[j + 1] = turn[j];
				j = j - 1;
			}
			turn[j + 1] = key;
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
void CombatManager::takeAction(Character* c) {
	/*
		If c is an emeny, do enemy attack
		Else, wait for user input
		If mouse hovers over player or enemy, display stats in info screen
		If attribute/inventory buttons are clicked, display options in info screen
		If a skill is clicked wait for user to click on character to use the skill on
		If energy = 0 or player clicks End Turn, return
	*/
	if (c->isEnemy == true)
	{
		//Enemy attack player
	}
	else
	{
		std::vector<Button *> buttons;
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			for (auto i : buttons)
			{
				if (((mouseX >= i->x) && (mouseX <= (i->x + i->w))) &&
					((mouseY >= i->y) && (mouseY <= (i->y + i->h))) && (i->type == "character"))
				{
					//Display in info screen
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

bool gameOn = true;
void CombatManager::combatManager(std::vector<Character*>& p) 
{
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
	participants = p;
	vector<int> ailments;
	// Create QueueManager obj which contains sorting of participant array. 
	QueueManager qm = QueueManager(participants);
	while (gameOn)
	{
		for (int i = 0; i < participants.size(); i++)
		{
			//updateStatus(participants[i]);
			takeAction(participants[i]);
		}
		qm.changeRounds();
	}
	
	 
	

}




