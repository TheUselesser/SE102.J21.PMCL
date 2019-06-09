#include "Enemy.h"
#include "Game.h"
#include "Player.h"

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
					if (isBoss())
					{
						decrease_HP();
					}
					else this->isExist = false;
				}
			}
			// player tấn công khi đang nhảy (xoay vòng vòng nên khỏi xét hướng)
			else
			{
				if (isBoss())
				{
					decrease_HP();
				}
				else this->isExist = false;
			}
		}
	}

	// đập player (khi hắn không bất tử)
	if (this->isExist && !player->isInvincible
		&& Game::getInstance()->allowHurtingPlayer)
	{
		if (collisionTime < 1.0f)
		{
			// hướng player = quái bên phải ? bên phải : bên trái
			player->directionX = this->getLeft() >= player->getLeft() ? 1 : -1;
			player->SetStatus(PLAYER_KNOCKBACK, player->directionX);

			// Giảm máu (Lúc đầu build dở nên giờ nhìn nó lủng củng vầy)
			Player::getInstance()->decrease_HP();

			if (getObjectType() == ENEMY_CANNON_BULLET ||
				getObjectType() == ENEMY_BOSS_3_BULLET)
			{
				// giảm thêm 1 máu nữa là 2
				Player::getInstance()->decrease_HP();
			}
		}
	}
}

void Enemy::setSpawned(bool isSpawned)
{
	isExist = isSpawned;
}

void Enemy::SetStatus(ENEMY_STATUS status)
{
	this->status = status;
}

void Enemy::Update(DWORD dt, GameObject & player)
{
	timer.tickPerAnim = dt;

	SetStatus(ENEMY_MOVING);
	autoMove(10);
	Draw();
}

void Enemy::MindTheGroundBlocks()
{
	switch (moveType)
	{
	case MT_ON_ONE_GROUND:
		if (getLeft() < currentBlock->getLeft())
		{
			setX(currentBlock->getLeft());
			directionX = 1;
		}
		if (getRight() > currentBlock->getRight())
		{
			setX(currentBlock->getRight() - getWidth());
			directionX = -1;
		}
		break;
	case MT_FROM_HIGHER_TO_LOWER_GROUND:	// [incompleted]
		if (getLeft() < currentBlock->getLeft())
		{
			if (getLeft() < 0)
			{ 
				setX(0);
				directionX = 1;
			}
			else
			{
				if (getRight() < currentBlock->getLeft())
					isOnGround = false;
			}
		}
		if (getRight() > currentBlock->getRight())
		{
			if (getLeft() > currentBlock->getRight())
				isOnGround = false;
		}

		if (!isOnGround)
		{
			moveY(-8);
			if (getY() <= currentGroundY - 32)
			{
				isOnGround = true;
				currentGroundY -= 32;
				setY(currentGroundY);
			}
		}

		break;
	default: break;
	}
}
