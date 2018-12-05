#ifndef _____COMBAT_MANAGER_H_____
#define _____COMBAT_MANAGER_H_____
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <vector>
#include <iostream>
#include "Helper.h"
#include "Globals.h"
#include "Graphics.h"
#include "UI/CombatDialogManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Button.h"
#include "LoadTexture.h"
#include "AI/CombatAI.h"
#include "AI/Action.h"
#include "ctype.h"

class CombatManager
{
public:
	CombatManager();
	~CombatManager();

	int checkCombatStatus();
	int updateStatus();
	
	/*
	*	the folloiwng 3 fucntions return true if player wins the combat, return false if player dies and thus lose the combat
	*/
	int performEvent(Character *c, string option, int optNum);
	int takeAction(Character* c, std::vector<Button *> buttons, SDL_Event e);
	int textAction(Character* c);
	int combatMain(std::vector<Character*>& c);
	void textAttributes(Character *c, int optNum);
	void textMain(bool& printed, bool initialText);
	//void setNewButtons(std::vector<Button*>& buttons, int t);
	void outputEnemy();

	int takeActionByAI(Character* c, int EnemyActionOrderCount); //Enemy takes action
	Action ActionByAI(Character* c, int EnemyActionOrderCount);

private:
	string atk;
	Ability abil;
	int target;
	bool inCombat;
	bool allPlayersMoved;
	int turnOrder;
	int livingCount[2];
	std::vector<Character*> participants;
	std::vector<int> ParticipantsStatus;
	std::vector<int> enemy_index;
	//std::vector<Enemy*> Enemies;
	std::vector<int> player_index;
	//std::vector<Player*> Players;
	//SDL_Texture* loadImage(std::string fname);
	SDL_Rect scene_box = { 0,0,720,540 };
	SDL_Rect ui_box = { 17,529,685,167 };
	SDL_Rect info_box = { 240,529,480, 167 };
	
	Graphics m_combatGraphics;
	CombatDialogManager m_combatDialogManager;

	bool initialText;
	Mix_Chunk *gBSound = NULL;

	CombatAI AI;
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
#endif