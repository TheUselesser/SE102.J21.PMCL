#include "GroundBlock.h"
#include "Collision.h"
#include <math.h>

#include "DXInput.h"
#include <string>

GroundBlock::GroundBlock(float x, float y, float width, float height, int groundType)
{
	setObjectType(OBJECT_GROUND);
	setCollisionType(COLLISION_TYPE_GROUND);
	this->groundType = groundType;
	setSize(width, height);
	setX(x);
	setY(y + getHeight());
}

GroundBlock::~GroundBlock()
{
}

bool GroundBlock::blockMovingX()
{
	return groundType == 1 ? true : false;
}

void GroundBlock::CheckCollisionStatus(GameObject * player)
{
	if (collisionTime < 1.0f)
	{
		if (this->blockMovingX())
		{
			if (player->getBottom() < this->getTop())
			{
				// va chạm bên trái block
				if (player->getX() + player->getRealWidth() > this->getLeft() && player->getLeft() < this->getLeft())
				{
					player->setX(this->getLeft() - player->getRealWidth());
				}
				// va chạm bên phải block
				if (player->getRight() - player->getRealWidth() < this->getRight() && player->getRight() > this->getRight())
				{
					player->setX(this->getRight() - player->getWidth() + player->getRealWidth());
				}
			}
		}

		// bám thang
		if (groundType == 3 && player->isJumping)
		{
			if (player->getBottom() < this->getTop())
			{
				// va chạm bên trái block
				if (player->getX() + player->getRealWidth() > this->getLeft() && player->getLeft() < this->getLeft())
				{
					player->setX(this->getLeft() - player->getRealWidth());
				}

				player->SetStatus(PLAYER_CLINGING, player->directionX);
			}
		}
	}
}

void GroundBlock::Update(DWORD dt, GameObject &player)
{
	Collision::CollisionHandle(player, *this);
	CheckCollisionStatus(&player);
}
