#ifndef _____PLAYER_H_____
#define _____PLAYER_H_____

#include <string>
#include <vector>
#include "../Headers/Inventory.h"
class Player /*: public Character*/
{
public:
	int strength;
	int intelligence;
	int dexterity;
	int constitution;
	int faith;
	int currentExprience;
	Inventory inventory;
	std::string name;

	//stores stats
	Player();
	Player(int s, int i, int d, int c, int f, std::string n);
	void setAll(int s, int i, int d, int c, int f, std::string n);
	operator std::string();
private:

};
#endif // !_____PLAYER_H_____
