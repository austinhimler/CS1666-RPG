#include "Headers/CombatManager.h"
//#include "Headers/QueueManager.h"
#include "Headers/Character.h"
#include "Headers/Player.h"
#include "Headers/Enemy.h"
#include <vector>
	QueueManager::QueueManager(vector<Character> c)
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


	void QueueManager::createRounds(vector<Character> c)
	{
		
	}

	void QueueManager::changeRounds()
	{
		vectorCopy(currTurn, nextTurn);
		insertionSort(nextTurn, nextTurn.size());
	}

	void QueueManager::insertionSort(std::vector<Character>& turn, int n)
	{
		int i, j;
		Character key;
		for (i = 1; i < n; i++)
		{
			key = turn[i];
			j = i - 1;
			while (j >= 0 && turn[j].getDex() > key.getDex())
			{
				turn[j + 1] = turn[j];
				j = j - 1;
			}
			turn[j + 1] = key;
		}
	}

	void QueueManager::vectorCopy(vector<Character>& cT, vector<Character>& nT)
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


void updateStatus(Character& c) {
	/**
	*	Turn - pre-turn:
						pre-turn effect calculation (if we wanna add status effects that carry over from one battle to the next we can but for now let's just make all effects wiped at the end of combat)
						check survival
						start in-turn
						*/
	if (c.getHPCur() <= 0) {
		gameOn = false;
	}
	// Game on!
}
void takeAction(Character& c) {
	/*
		If c is an emeny, do enemy attack
		Else, wait for user input
		If mouse hovers over player or enemy, display stats in info screen
		If attribute/inventory buttons are clicked, display options in info screen
		If a skill is clicked wait for user to click on character to use the skill on
		If energy = 0 or player clicks End Turn, return
	*/
	



}
bool gameOn = true;
void combatManager(Player& p) 
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
	Enemy e1;
	// Set up a Character array and populate it (not sorted by dex) 
	//Character participants[2];
	vector<Character> participants;
	participants[0] = Character(p); 
	participants[1] = Character(e1);
	// Create QueueManager obj which contains sorting of participant array. 
	QueueManager qm = QueueManager(participants);
	while (gameOn)
	{
		for (int i = 0; i < participants.size(); i++)
		{
			updateStatus(participants[i]);
			takeAction(participants[i]);

		}
		qm.changeRounds();
	}

	
	 
	

}









