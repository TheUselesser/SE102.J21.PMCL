#include "Cannoneer.h"

#include <string>

Cannoneer::Cannoneer()
{
	setSize(DEFAULT_CANNONEER_WIDTH, DEFAULT_CANNONEER_HEIGHT);
}

Cannoneer::Cannoneer(float x, float y)
{
	setSize(DEFAULT_CANNONEER_WIDTH, DEFAULT_CANNONEER_HEIGHT);
	setX(x);
	setY(y + getHeight());
	isExist = false;
}

Cannoneer::~Cannoneer()
{
}

void Cannoneer::Init()
{
	isExist = true;
	directionChanged = false;
	directionX = -1;	// mai mốt xét direction tùy theo vị trí của player

	// SetStatus(ENEMY_STANDING);
	status = ENEMY_STANDING;
	sprite->SetAnimation(getWidth(), getHeight(), 1, 1, 0, 0);
	if (directionX > 0)
	{
		//sprite->Release();
		sprite->LoadTexture("images/enemies/Cannoneer_right.png", D3DCOLOR_XRGB(255, 163, 177));
	}
	else
	{
		//sprite->Release();
		sprite->LoadTexture("images/enemies/Cannoneer_left.png", D3DCOLOR_XRGB(255, 163, 177));
	}
}

void Cannoneer::Update(DWORD dt, GameObject & player)
{
	timer.tickPerAnim = dt;

	if (directionChanged)
	{
		if (directionX > 0)
		{
			sprite->Release();
			sprite->LoadTexture("images/enemies/Cannoneer_right.png", D3DCOLOR_XRGB(255, 163, 177));
		}
		else
		{
			sprite->Release();
			sprite->LoadTexture("images/enemies/Cannoneer_left.png", D3DCOLOR_XRGB(255, 163, 177));
		}
	}

	Draw();
}

