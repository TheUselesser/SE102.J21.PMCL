#include "BirdBrown.h"
BirdBrown::BirdBrown()
{
	setSize(DEFAULT_BIRDBROWN_WIDTH, DEFAULT_BIRDBROWN_HEIGHT);
}

BirdBrown::BirdBrown(float x, float y)
{
	setSize(DEFAULT_BIRDBROWN_WIDTH, DEFAULT_BIRDBROWN_HEIGHT);
	spawnX = x;
	setX(x);
	setY(y + getHeight());
}

BirdBrown::~BirdBrown()
{
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

void BirdBrown::Update(DWORD dt)
{
	timer.tickPerAnim = dt;

	SetStatus(ENEMY_MOVING);
	autoMove(60);
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
	selfMovingX();
}