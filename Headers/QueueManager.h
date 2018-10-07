#pragma once
#include "../Headers/Character.h"
class QueueManager
{
public:
	QueueManager();
	~QueueManager();
	Character[] currTurn;
	Character[] nextTurn;
	void createTurns();
};