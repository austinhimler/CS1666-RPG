#include "../Headers/QueueManager.h"
#include "../Headers/Character.h"

QueueManager::QueueManager()
{

}

QueueManager::~QueueManager()
{

}
Character[] currTurn;
Character[] nextTurn;

public void createTurns(Character[] c)
{
	Character[] currTurn = c.size();
	Character[] nextTurn = c.size();
	insertionSort(&currTurn, currTurn.size());
	insertionSort(&nextTurn, nextTurn.size());
}

public void changeTurns()
{
	std::copy(nextTurn, nextTurn[nextTurn.size], currTurn);
	insertionSort(&nextTurn, nextTurn.size());
}

void insertionSort(Character[] * turn, int n)
{
	int i, key, j;
	for (i = 1; i < n; i++)
	{
		key = arr[i];
		j = i - 1;
		while (j >= 0 && turn[j].dexterity > key)
		{
			turn[j + 1] = turn[j];
			j = j - 1;
		}
		turn[j + 1] = key;
	}
}