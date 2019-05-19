#include "Banshee.h"
Banshee::Banshee()
{
	setSize(DEFAULT_BANSHEE_WIDTH, DEFAULT_BANSHEE_HEIGHT);
}

Banshee::Banshee(float x, float y)
{
	setSize(DEFAULT_BANSHEE_WIDTH, DEFAULT_BANSHEE_HEIGHT);
	spawnX = x;
	setX(x);
	setY(y + getHeight());
}

Banshee::~Banshee()
{
}

void Banshee::SetStatus(ENEMY_STATUS status)
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
			sprite->SetAnimation(getWidth(), getHeight(), 3, 3, 0, 1);
			if (getVelX() > 0)
			{
				sprite->Release();
				sprite->LoadTexture("images/enemies/Banshee_right.png", D3DCOLOR_XRGB(255, 255, 255));
			}
			else
			{
				sprite->Release();
				sprite->LoadTexture("images/enemies/Banshee_left.png", D3DCOLOR_XRGB(255, 255, 255));
			}
			break;
		default:
			break;
		}
	}
}

void Banshee::Update(DWORD dt)
{
	timer.tickPerAnim = dt;

	SetStatus(ENEMY_MOVING);
	autoMove(10);
	Draw();
}

void Banshee::autoMove(float range)
{
	// ?i qua l?i ? ?i?m ban ??u ph?m vi range  |<---range---spawnX---range--->|
	if (getX() <= spawnX - range || getX() >= spawnX + range - getWidth())
	{
		setVelX(-getVelX());
		directionChanged = true;
	}
	selfMovingX();
}