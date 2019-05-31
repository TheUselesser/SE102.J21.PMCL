#include "BirdBrown.h"
BirdBrown::BirdBrown()
{
	setSize(DEFAULT_BIRD_BROWN_WIDTH, DEFAULT_BIRD_BROWN_HEIGHT);
}

BirdBrown::BirdBrown(float x, float y)
{
	setSize(DEFAULT_BIRD_BROWN_WIDTH, DEFAULT_BIRD_BROWN_HEIGHT);
	spawnX = x;
	setX(x);
	setY(y + getHeight());
	maxY = getY();
	minY = maxY - 32;
	isExist = false;

	seekRange = 48;
	playerRange = 64;
	playerIsInSeekRange = false;
}

BirdBrown::~BirdBrown()
{
}

void BirdBrown::Init(GameObject * player)
{
	isExist = true;
	setCollisionType(COLLISION_TYPE_ENEMY);
	directionX = player->getMidX() <= getMidX() ? -1 : 1;
	directionY = -1;
	setVelX(DEFAULT_BIRD_BROWN_VELOCITY * directionX);
	setVelY(DEFAULT_BIRD_BROWN_VELOCITY * directionY);

	SetStatus(ENEMY_STANDING);
	sprite->SetAnimation(getWidth(), getHeight(), 2, 2, 0, 1);
	if (directionX > 0)
	{
		//sprite->Release();
		sprite->LoadTexture("images/enemies/BirdBrown_right.png", D3DCOLOR_XRGB(255, 255, 255));
	}
	else
	{
		//sprite->Release();
		sprite->LoadTexture("images/enemies/BirdBrown_left.png", D3DCOLOR_XRGB(255, 255, 255));
	}
}

void BirdBrown::SetStatus(ENEMY_STATUS status)
{
	//if (this->status != status || directionChanged)
	{
		this->status = status;

		switch (status)
		{
		case ENEMY_STANDING:
			startAnimation = false;

			break;
		case ENEMY_MOVING:
			startAnimation = true;
			sprite->SetAnimation(getWidth(), getHeight(), 2, 2, 0, 1);
			if (getVelX() > 0)
			{
				sprite->Release();
				sprite->LoadTexture("images/enemies/BirdBrown_right.png", D3DCOLOR_XRGB(255, 255, 255));
			}
			else
			{
				sprite->Release();
				sprite->LoadTexture("images/enemies/BirdBrown_left.png", D3DCOLOR_XRGB(255, 255, 255));
			}
			break;
		default:
			break;
		}
	}
}

void BirdBrown::Update(DWORD dt, GameObject &player)
{
	timer.tickPerAnim = dt;

	SetStatus(ENEMY_MOVING);
	autoMove(100, &player);
	Draw();
}

void BirdBrown::autoMove(float range, GameObject * player)
{
	if (!playerIsInSeekRange)
	{
		// bay qua lại ở điểm ban đầu phạm vi range  |<---range---spawnX---range--->|
		if (getMidX() <= spawnX - range || getMidX() >= spawnX + range)
		{
			setVelX(-getVelX());
		}

		// bay lên bay xuống một cách bình thường
		if (getY() <= minY || getY() > maxY)
		{
			setVelY(-getVelY());
		}

		if (player->getRight() >= getMidX() - seekRange && player->getLeft() <= getMidX() + seekRange)
		{
			playerIsInSeekRange = true;
		}
	}
	else
	{
		minY = player->getY();
		maxY = minY + getHeight();

		// bay qua lại quanh player
		if (getMidX() >= player->getMidX() - playerRange && getMidX() <= player->getMidX() + playerRange)
		{
			if (getMidX() < player->getMidX() - playerRange || getMidX() > player->getMidX() + playerRange)
			{
				setVelX(-getVelX());
			}
		}
		else if (getMidX() < player->getMidX() - playerRange)
		{
			setVelX(DEFAULT_BIRD_BROWN_VELOCITY);
		}
		else
		{
			setVelX(-DEFAULT_BIRD_BROWN_VELOCITY);
		}

		// bay lên bay xuống
		if (getMidY() >= minY && getMidY() <= maxY)
		{
			if (getMidY() == minY || getMidY() == maxY)
			{
				setVelY(-getVelY());
			}
		}
		else if (getMidY() < minY)
		{
			setVelY(DEFAULT_BIRD_BROWN_VELOCITY);
		}
		else
		{
			setVelY(-DEFAULT_BIRD_BROWN_VELOCITY);
		}
	}
	selfMovingX();
	selfMovingY();
}