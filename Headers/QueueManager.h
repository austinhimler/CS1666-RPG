
#ifndef _____QUEMANAGER_H_____
#define _____QUEMANAGER_H_____
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
#endif