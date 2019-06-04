#include "Runner.h"



Runner::Runner()
{
	setSize(DEFAULT_RUNNER_WIDTH, DEFAULT_RUNNER_HEIGHT);
}

Runner::Runner(float x, float y)
{
	setSize(DEFAULT_RUNNER_WIDTH, DEFAULT_RUNNER_HEIGHT);
	spawnX = x;
	spawnY = y;
	isExist = false;
}

Runner::~Runner()
{
}

void Runner::Init(GameObject * player)
{
	isExist = true;
	setCollisionType(COLLISION_TYPE_ENEMY);
	setX(spawnX);
	setY(spawnY + getHeight());
	directionX = player->getMidX() <= getMidX() ? -1 : 1;
	setVelX(DEFAULT_RUNNER_VELOCITY * directionX);

	SetStatus(ENEMY_STANDING);
	sprite->SetAnimation(getWidth(), getHeight(), 2, 2, 0, 1);
	if (directionX > 0)
	{
		sprite->Release();
		sprite->LoadTexture("images/enemies/Runner_right.png", D3DCOLOR_XRGB(255, 255, 255));
	}
	else
	{
		sprite->Release();
		sprite->LoadTexture("images/enemies/Runner_left.png", D3DCOLOR_XRGB(255, 255, 255));
	}
}

void Runner::SetStatus(ENEMY_STATUS status)
{//if (this->status != status || directionChanged)
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
				sprite->LoadTexture("images/enemies/Runner_right.png", D3DCOLOR_XRGB(255, 255, 255));
			}
			else
			{
				sprite->Release();
				sprite->LoadTexture("images/enemies/Runner_left.png", D3DCOLOR_XRGB(255, 255, 255));
			}
			break;
		default:
			break;
		}
	}
}

void Runner::Update(DWORD dt, GameObject & player)
{
	timer.tickPerAnim = dt;

	SetStatus(ENEMY_MOVING);
	autoMove(80);
	Draw();
}

void Runner::autoMove(float range)
{
	// đi qua lại ở điểm ban đâu phạm vi range  |<---range---spawnX---range--->|
	if (getX() <= spawnX - range || getX() >= spawnX + range - getWidth())
	{
		setVelX(-getVelX());
		directionChanged = true;
	}
	selfMovingX();
}
