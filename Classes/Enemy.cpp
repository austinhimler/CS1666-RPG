#include "../Headers/Enemy.h"

Enemy::Enemy()
{
	isEnemy = true;
}
Enemy::Enemy(std::string n, int s, int i, int d, int c, int f) : Character(n, s, i, d, c, f) {
	isEnemy = true;
	learnAbility(HEAL);
	learnAbility(ESCAPE);
}

