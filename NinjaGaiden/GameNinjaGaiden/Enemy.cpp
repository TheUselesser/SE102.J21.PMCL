#include "Enemy.h"
#include "Game.h"

#include "Collision.h"

Enemy::Enemy()
{
	isExist = false;
}


Enemy::~Enemy()
{
}

void Enemy::CheckCollisionStatus(GameObject * player)
{
	// bị player tấn công
	if (player->isAttacking)
	{
		if (collisionTime < 1.0f)
		{
			// player tấn công khi đang đứng
			if (!player->isJumping)
			{
				if (player->directionX * (this->getLeft() >= player->getLeft() ? 1 : -1) > 0)
				{
					this->isExist = false;
				}
			}
			// player tấn công khi đang nhảy (xoay vòng vòng nên khỏi xét hướng)
			else
			{
				this->isExist = false;
			}
		}
	}

	// đập player (khi hắn không bất tử)
	if (this->isExist && !player->isInvincible
		&& Game::getInstance()->allowHurtingPlayer
		)
	{
		if (collisionTime < 1.0f)
		{
			// hướng player = quái bên phải ? bên phải : bên trái
			player->directionX = this->getLeft() >= player->getLeft() ? 1 : -1;
			player->SetStatus(PLAYER_KNOCKBACK, player->directionX);
		}
	}
}

void Enemy::setSpawned(bool isSpawned)
{
	isExist = isSpawned;
}

void Enemy::SetStatus(ENEMY_STATUS status, int direction)
{
	this->status = status;
}