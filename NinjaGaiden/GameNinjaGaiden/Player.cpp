#include "Player.h"
#include "DXInput.h"
#include "Game.h"

/*
Player * Player::instance = NULL;
Player * Player::getInstance()
{
	if (instance == NULL)
		instance = new Player();
	return instance;
}*/

Player::Player()
{
	isMoving = directionChanged = startAnimation = isJumping = isFalling = false;
	currentHeight = 0;
	maxHeight = 48;
	maxHeightReached = false;
}


Player::~Player()
{
}

void Player::InitPlayer(float x, float y)
{
	CreateObject("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255), 22, 32);
	setX(x);
	setY(y + getHeight());
	setVelX(4);
	setVelY(8);
}

void Player::SetStatus(PLAYER_STATUS status, int direction)
{
	if (this->status != status || directionChanged)
	{
		this->status = status;

		switch (status)
		{
		case PLAYER_STANDING:
			startAnimation = false;
			setSize(22, 32);
			sprite->SetAnimation(getWidth(), getHeight(), 4, 4, 0, 0);
			if (direction > 0)
			{
				sprite->Release();
				sprite->LoadTexture("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255));
			}
			else
			{
				sprite->Release();
				sprite->LoadTexture("images/Ryu_left.png", D3DCOLOR_XRGB(255, 0, 255));
			}
			break;
		case PLAYER_MOVING:
			startAnimation = true;
			setSize(22, 32);
			sprite->SetAnimation(getWidth(), getHeight(), 4, 4, 1, 3);
			if (direction > 0)
			{
				sprite->Release();
				sprite->LoadTexture("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255));
			}
			else
			{
				sprite->Release();
				sprite->LoadTexture("images/Ryu_left.png", D3DCOLOR_XRGB(255, 0, 255));
			}
			break;
		case PLAYER_JUMPING:
			startAnimation = true;
			setSize(24, 24);
			sprite->SetAnimation(getWidth(), getHeight(), 4, 2, 0, 3);
			if (direction > 0)
			{
				sprite->Release();
				sprite->LoadTexture("images/Ryu_jump_right.png", D3DCOLOR_XRGB(255, 0, 255));
			}
			else
			{
				sprite->Release();
				sprite->LoadTexture("images/Ryu_jump_left.png", D3DCOLOR_XRGB(255, 0, 255));
			}
			break;
		case PLAYER_END_JUMPING:
			break;
		case PLAYER_FALLING:
			
			break;
		default:
			break;
		}
	}
}

void Player::Update(DWORD dt)
{
	timer.tickPerAnim = dt;

	if (isMoving)
	{
		float cameraX = Camera::getInstance()->getX();
		float cameraWidth = Camera::getInstance()->getWidth();
		int mapStart = Game::getInstance()->getStage()->getMapStart();
		int mapEnd = Game::getInstance()->getStage()->getMapEnd();

		if (!isOnCollisionX)
		{
			// Di chuyển nhân vật khi camera chạm biên
			if ((cameraX >= mapEnd - cameraWidth && this->getX() >= mapEnd - (cameraWidth + this->getWidth()) / 2) ||
				(cameraX <= mapStart && this->getX() <= mapStart + (cameraWidth - this->getWidth()) / 2))
			{
				selfMovingX();
			}
			// Di chuyển camera
			else
			{
				selfMovingX();
				if (cameraX >= mapStart - this->getVelX() && cameraX <= mapEnd + this->getVelX())
					Camera::getInstance()->moveX(this->getVelX());
			}
		}

		// quy định giới hạn nhân vật trong camera
		if (this->getX() < mapStart)
		{
			this->setX(mapStart);
		}
		// quy định giới hạn camera trong map
		if (cameraX < mapStart)
		{
			Camera::getInstance()->setX(mapStart);
		}
		if (cameraX > mapEnd - cameraWidth)
		{
			Camera::getInstance()->setX(mapEnd - cameraWidth);
		}
	}

	if (directionChanged)
	{
		SetStatus(this->status, directionX);
		directionChanged = false;
	}

	// Vẽ lên camera
	Draw();
}
