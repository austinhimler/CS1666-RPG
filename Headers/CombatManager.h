#pragma once
#include "Headers/Player.h"
#include "Headers/Enemy.h"
#include "Headers/Button.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <iostream>
#include "Helper.h"
#include "Globals.h"

class CombatManager
{
public:
	CombatManager();
	~CombatManager();
	void updateStatus(Character* c);
	void takeAction(Character* c, std::vector<Button *> buttons, SDL_Event e);
	bool combatManager(std::vector<Character*> c);
	bool gameOn;
	vector<Character*> participants;
	SDL_Texture* loadImage(std::string fname);

};
class QueueManager
{
public:
	std::vector<Character*> currTurn;
	std::vector<Character*> nextTurn;
	QueueManager(vector<Character*> c);
	~QueueManager();
	void createRounds(vector<Character*> c);
	void changeRounds();
	void vectorCopy(vector<Character*>& cT, vector<Character*>& nT);
private:
	void insertionSort(std::vector<Character*>& turn, int n);
};
