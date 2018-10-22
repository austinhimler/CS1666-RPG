#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Button.h"
#include "LoadTexture.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
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
	bool combatMain(std::vector<Character*> c);
	void setNewButtons(std::vector<Button*>& buttons, int t);

private:
	bool gameOn;
	vector<Character*> participants;
	SDL_Texture* loadImage(std::string fname);
	SDL_Rect scene_box = { 0,0,720,540 };
	SDL_Rect ui_box = { 17,529,685,167 };
	SDL_Rect info_box = { 240,529,480, 167 };

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
