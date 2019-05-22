#include "Bat.h"
Bat::Bat()
{
	setSize(DEFAULT_BAT_WIDTH, DEFAULT_BAT_HEIGHT);
}

Bat::Bat(float x, float y)
{
	setSize(DEFAULT_BAT_WIDTH, DEFAULT_BAT_HEIGHT);
	spawnX = x;
	setX(x);
	setY(y + getHeight());
}

Bat::~Bat()
{
}

void Bat::SetStatus(ENEMY_STATUS status)
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
				sprite->LoadTexture("images/enemies/Bat_right.png", D3DCOLOR_XRGB(255, 255, 255));
			}
			else
			{
				sprite->Release();
				sprite->LoadTexture("images/enemies/Bat_left.png", D3DCOLOR_XRGB(255, 255, 255));
			}
			break;
		default:
			break;
		}
	}
}

void Bat::Update(DWORD dt, GameObject &player)
{
	timer.tickPerAnim = dt;

	SetStatus(ENEMY_MOVING);
	autoMove(10);
	Draw();
}

void Bat::autoMove(float range)
{
	// ?i qua l?i ? ?i?m ban ??u ph?m vi range  |<---range---spawnX---range--->|
	if (getX() <= spawnX - range || getX() >= spawnX + range - getWidth())
	{
		setVelX(-getVelX());
		directionChanged = true;
	}
	selfMovingX();
}