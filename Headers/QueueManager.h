#pragma once

#include "Character.h"

class QueueManager
{
public:
	QueueManager();
	~QueueManager();
	Character[] currTurn;
	Character[] nextTurn;
	void createTurns();
};