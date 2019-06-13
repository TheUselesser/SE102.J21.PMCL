#include "MachineBullet.h"



MachineBullet::MachineBullet()
{
	setSize(DEFAULT_MACHINEGUN_BULLET_WIDTH, DEFAULT_MACHINEGUN_BULLET_HEIGHT);
	isExist = false;
	startFire = -1;
}

MachineBullet::~MachineBullet()
{
}

void MachineBullet::Init(float x, float y)
{
	setX(x);
	setY(y);

	startFire = GetTickCount();
	SetStatus(ENEMY_STANDING);
	isExist = true;
	setObjectType(ENEMY_MACHINE_BULLET);
	setCollisionType(COLLISION_TYPE_ENEMY);
}

void MachineBullet::SetStatus(ENEMY_STATUS status)
{
	switch (status)
	{
	case ENEMY_STANDING:
		setVelX(0);
		sprite->SetAnimation(getWidth(), getHeight(), 3, 3, 0, 1);
		if (directionX > 0)
		{
			sprite->Release();
			sprite->LoadTexture("images/enemies/MachineGunGuy_fire_left.png", D3DCOLOR_XRGB(0, 128, 128));
		}
		else
		{
			sprite->Release();
			sprite->LoadTexture("images/enemies/MachineGunGuy_fire_right.png", D3DCOLOR_XRGB(0, 128, 128));
		}
		break;
	case ENEMY_MOVING:
		setVelX(DEFAULT_MACHINEGUN_BULLET_VELOCITY * directionX);
		sprite->SetAnimation(getWidth(), getHeight(), 3, 3, 2, 2);
		if (directionX > 0)
		{
			sprite->Release();
			sprite->LoadTexture("images/enemies/MachineGunGuy_fire_right.png", D3DCOLOR_XRGB(0, 128, 128));
		}
		else
		{
			sprite->Release();
			sprite->LoadTexture("images/enemies/MachineGunGuy_fire_left.png", D3DCOLOR_XRGB(0, 128, 128));
		}
		break;
	default:
		break;
	}
}

void MachineBullet::Update(DWORD dt, GameObject & player)
{
	if (startFire != -1)
		if (GetTickCount() - startFire >= FIRE_TIME)
		{
			SetStatus(ENEMY_MOVING);
			startFire = -1;
		}

	autoMove(32);

	Draw();
}

void MachineBullet::autoMove(float range)
{
	selfMovingX();
}
