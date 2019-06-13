#include "MachineGunGuy.h"
#include "Player.h"
#include "Collision.h"
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

	//bullet = new MachineBullet();
}

MachineGunGuy::~MachineGunGuy()
{
}

void MachineGunGuy::Init(GameObject * player)
{
	isExist = true;
	setCollisionType(COLLISION_TYPE_ENEMY);
	setX(spawnX);
	setY(spawnY + getHeight());

	directionX = player->getMidX() > getMidX() ? 1 : -1; // <<<<<<< Thêm cái này nè
	setVelX(DEFAULT_MACHINE_GUN_GUY_VELOCITY * directionX);

	isAttacking = true;
	startCooldown = GetTickCount() + 800;

	SetStatus(ENEMY_STANDING);
	sprite->SetAnimation(getWidth(), getHeight(), 4, 4, 0, 1);
	if (directionX > 0)
	{
		sprite->Release();
		sprite->LoadTexture("images/enemies/MachineGunGuy_right.png", D3DCOLOR_XRGB(0, 128, 128));
	}
	else
	{
		sprite->Release();
		sprite->LoadTexture("images/enemies/MachineGunGuy_left.png", D3DCOLOR_XRGB(0, 128, 128));
	}
}

void MachineGunGuy::SetStatus(ENEMY_STATUS status)
{
	this->status = status;

	switch (status)
	{
	case ENEMY_STANDING:
		startAnimation = false;

		break;
	case ENEMY_MOVING:
		startAnimation = true;
		sprite->SetAnimation(getWidth(), getHeight(), 4, 4, 0, 1);
		if (getVelX() > 0)
		{
			sprite->Release();
			sprite->LoadTexture("images/enemies/MachineGunGuy_right.png", D3DCOLOR_XRGB(0, 128, 128));
		}
		else
		{
			sprite->Release();
			sprite->LoadTexture("images/enemies/MachineGunGuy_left.png", D3DCOLOR_XRGB(0, 128, 128));
		}
		break;
	case ENEMY_ATTACKING:
		startAnimation = true;
		sprite->SetAnimation(getWidth(), getHeight(), 4, 4, 2, 3);
		if (getVelX() > 0)
		{
			sprite->Release();
			sprite->LoadTexture("images/enemies/MachineGunGuy_right.png", D3DCOLOR_XRGB(0, 128, 128));
		}
		else
		{
			sprite->Release();
			sprite->LoadTexture("images/enemies/MachineGunGuy_left.png", D3DCOLOR_XRGB(0, 128, 128));
		}
		break;
	default:
		break;
	}
}

void MachineGunGuy::Update(DWORD dt, GameObject &player)
{
	timer.tickPerAnim = dt;
	SetStatus(ENEMY_MOVING);
	autoMove(32);

	/*if (bullet->startFire != -1)
	{
		SetStatus(ENEMY_ATTACKING);
	}
	else
	{
		SetStatus(ENEMY_MOVING);
	}

	if (!isFreezing)
	{
		autoMove(32);
		periodAttack(2000);
	}
	else
	{
		if (GetTickCount() - startFreezeTime >= freezeTime)
		{
			isFreezing = false;
		}
	}*/

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

//void MachineGunGuy::periodAttack(DWORD cooldown)
//{
//	// isAttacking = false <=> sẵn sàng tấn công => thực hiện tấn công
//	if (!isAttacking)
//	{
//		isAttacking = true;
//		startCooldown = GetTickCount();
//
//		// Xử lý tấn công
//		bullet->directionX = directionX;
//		if (directionX > 0)
//		{
//			bullet->Init(getRight(), getY());
//		}
//		else
//		{
//			bullet->Init(getLeft() - bullet->getWidth(), getY());
//		}
//	}
//	// đã attack thì bắt đầu chờ cd
//	else
//	{
//		if (GetTickCount() - startCooldown >= cooldown)
//		{
//			isAttacking = false;
//			bullet->isExist = false;
//		}
//
//		// update viên đạn bay
//		if (bullet->isExist)
//		{
//			Collision::CollisionHandle(*Player::getInstance(), *bullet);
//			bullet->Update(0, *Player::getInstance());
//		}
//	}
//}