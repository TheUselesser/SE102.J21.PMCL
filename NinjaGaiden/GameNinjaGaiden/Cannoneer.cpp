#include "Cannoneer.h"

#include <string>

Cannoneer::Cannoneer()
{
	setSize(DEFAULT_CANNONEER_WIDTH, DEFAULT_CANNONEER_HEIGHT);
}

Cannoneer::Cannoneer(float x, float y)
{
	setSize(DEFAULT_CANNONEER_WIDTH, DEFAULT_CANNONEER_HEIGHT);
	spawnX = x;
	spawnY = y;
	isExist = false;
}

Cannoneer::~Cannoneer()
{
}

void Cannoneer::Init(GameObject * player)
{
	isExist = true;
	setCollisionType(COLLISION_TYPE_ENEMY);
	directionChanged = false;
	setX(spawnX);
	setY(spawnY + getHeight());
	directionX = player->getMidX() <= getMidX() ? -1 : 1;

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

