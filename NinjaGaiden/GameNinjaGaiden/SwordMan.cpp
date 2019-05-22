#include "SwordMan.h"
#include "Collision.h"


SwordMan::SwordMan()
{
	setSize(DEFAULT_SWORD_MAN_WIDTH, DEFAULT_SWORD_MAN_HEIGHT);
}

SwordMan::SwordMan(float x, float y)
{
	setSize(DEFAULT_SWORD_MAN_WIDTH, DEFAULT_SWORD_MAN_HEIGHT);
	spawnX = x;
	setX(x);
	setY(y + getHeight());
	isExist = false;
}

SwordMan::~SwordMan()
{
}

void SwordMan::Init()
{
	isExist = true;
	directionX = -1;	// mai mốt xét direction tùy theo vị trí của player
	setVelX(DEFAULT_SWORD_MAN_VELOCITY * directionX);
	
	SetStatus(ENEMY_STANDING);
	sprite->SetAnimation(getWidth(), getHeight(), 2, 2, 0, 1);
	if (directionX > 0)
	{
		sprite->Release();
		sprite->LoadTexture("images/enemies/SwordMan_right.png", D3DCOLOR_XRGB(255, 255, 255));
	}
	else
	{
		sprite->Release();
		sprite->LoadTexture("images/enemies/SwordMan_left.png", D3DCOLOR_XRGB(255, 255, 255));
	}
}

void SwordMan::SetStatus(ENEMY_STATUS status)
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
				sprite->LoadTexture("images/enemies/SwordMan_right.png", D3DCOLOR_XRGB(255, 255, 255));
			}
			else
			{
				sprite->Release();
				sprite->LoadTexture("images/enemies/SwordMan_left.png", D3DCOLOR_XRGB(255, 255, 255));
			}
			break;
		default:
			break;
		}
	}
}

void SwordMan::Update(DWORD dt, GameObject &player)
{
	timer.tickPerAnim = dt;

	SetStatus(ENEMY_MOVING);
	Collision::CollisionHandle(*this, player);
	Collision::CollisionHandle(player, *this);
	
	// Khi chạm vào player
	if (isOnCollisionX)
	{
		// thì làm cái gì đó
		// player.knockBack() chẳng hạn
	}
	else
	{
		autoMove(80);
	}
	
	Draw();
}

void SwordMan::autoMove(float range)
{
	// đi qua lại ở điểm ban đâu phạm vi range  |<---range---spawnX---range--->|
	if (getX() <= spawnX - range || getX() >= spawnX + range - getWidth())
	{
		setVelX(-getVelX());
		directionChanged = true;
	}
	selfMovingX();
}