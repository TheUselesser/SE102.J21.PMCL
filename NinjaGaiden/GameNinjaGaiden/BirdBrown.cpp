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
}

BirdBrown::~BirdBrown()
{
}

void BirdBrown::Init()
{
	isExist = true;
	directionX = -1;	// mai mốt xét direction tùy theo vị trí của player
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
	autoMove(100);
	Draw();
}

void BirdBrown::autoMove(float range)
{
	// đi qua lại ở điểm ban đâu phạm vi range  |<---range---spawnX---range--->|
	if (getX() <= spawnX - range || getX() >= spawnX + range - getWidth())
	{
		setVelX(-getVelX());
		directionChanged = true;
	}
	if (getY() <= minY || getY() > maxY)
	{
		setVelY(-getVelY());
	}
	selfMovingX();
	selfMovingY();
}