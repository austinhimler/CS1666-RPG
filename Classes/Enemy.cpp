#include "../Headers/Enemy.h"
#include "../Headers/Item.h"
#include <string>
#include <vector>
Enemy::Enemy()
{
	level = 1;
	isEnemy = true;
}
Enemy::Enemy(std::string n, int s, int i, int d, int c, int f) : Character(n, s, i, d, c, f) {
	level = 1;
	isEnemy = true;
}

