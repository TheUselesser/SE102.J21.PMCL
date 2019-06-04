#include "MachineGunGuy.h"
MachineGunGuy::MachineGunGuy()
{
	setSize(DEFAULT_MACHINE_GUN_GUY_WIDTH, DEFAULT_MACHINE_GUN_GUY_HEIGHT);
}

MachineGunGuy::MachineGunGuy(float x, float y)
{
	setSize(DEFAULT_MACHINE_GUN_GUY_WIDTH, DEFAULT_MACHINE_GUN_GUY_HEIGHT);
	spawnX = x;
	spawnY = y;
	isExist = false;
}

MachineGunGuy::~MachineGunGuy()
{
}

void MachineGunGuy::Init(GameObject * player)
{
	isExist = true;
	setCollisionType(COLLISION_TYPE_ENEMY); setX(spawnX);
	setY(spawnY + getHeight());
	directionX = player->getMidX() <= getMidX() ? -1 : 1;
	setVelX(DEFAULT_MACHINE_GUN_GUY_VELOCITY * directionX);

	SetStatus(ENEMY_STANDING);
	sprite->SetAnimation(getWidth(), getHeight(), 2, 2, 0, 1);
	if (directionX > 0)
	{
		sprite->Release();
		sprite->LoadTexture("images/enemies/MachineGunGuy_right.png", D3DCOLOR_XRGB(255, 255, 255));
	}
	else
	{
		sprite->Release();
		sprite->LoadTexture("images/enemies/MachineGunGuy_left.png", D3DCOLOR_XRGB(255, 255, 255));
	}
}

void MachineGunGuy::SetStatus(ENEMY_STATUS status)
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
				sprite->LoadTexture("images/enemies/MachineGunGuy_right.png", D3DCOLOR_XRGB(255, 255, 255));
			}
			else
			{
				sprite->Release();
				sprite->LoadTexture("images/enemies/MachineGunGuy_left.png", D3DCOLOR_XRGB(255, 255, 255));
			}
			break;
		default:
			break;
		}
	}
}

void MachineGunGuy::Update(DWORD dt, GameObject &player)
{
	timer.tickPerAnim = dt;

	SetStatus(ENEMY_MOVING);
	autoMove(32);
	Draw();
}

void MachineGunGuy::autoMove(float range)
{
	// ?i qua l?i ? ?i?m ban ??u ph?m vi range  |<---range---spawnX---range--->|
	if (getX() <= spawnX - range || getX() >= spawnX + range - getWidth())
	{
		setVelX(-getVelX());
		directionChanged = true;
	}
	selfMovingX();
}