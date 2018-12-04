#include <string>
#include <vector>

#include "../Headers/Enemy.h"
#include "../Headers/Item.h"

Enemy::Enemy()
{
	isEnemy = true;
}
Enemy::Enemy(std::string n, int s, int i, int d, int c, int f) : Character(n, s, i, d, c, f) {
	isEnemy = true;
}

