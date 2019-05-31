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

void SwordMan::Init(GameObject * player)
{
	isExist = true;
	setCollisionType(COLLISION_TYPE_ENEMY);
	
	directionX = player->getMidX() <= getMidX() ? -1 : 1;

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
	autoMove(0);
	Draw();
}

void SwordMan::autoMove(float range)
{
	selfMovingX();
}