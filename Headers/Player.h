#include <string>
#include <vector>

class Player /*: public Character*/
{
public:
	int strength;
	int intelligence;
	int dexterity;
	int constitution;
	int faith;
	int currentExprience;
	//Inventory inventory;
	std::string name;
	//stores stats
	Player();
	Player(int s, int i, int d, int c, int f, std::string n);
	void setAll(int s, int i, int d, int c, int f, std::string n);
	operator std::string();
private:

};