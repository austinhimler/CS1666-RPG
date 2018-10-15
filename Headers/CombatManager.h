#pragma once
#include "Character.h"
class CombatManager
{
public:
	CombatManager();
	~CombatManager();
	void combatManager();
	bool gameOn;
};
class QueueManager
{
public:
	QueueManager();
	~QueueManager();
	void createRounds();
	void changeRounds();
private:
	void insertionSort();
};
