#include "Enemy.h"



Enemy::Enemy()
{
	isExist = false;
}


Enemy::~Enemy()
{
}

void Enemy::setSpawned(bool isSpawned)
{
	isExist = isSpawned;
}

void Enemy::SetStatus(ENEMY_STATUS status, int direction)
{
	this->status = status;
}

void Enemy::Update()
{
}

void Enemy::autoMove()
{
}