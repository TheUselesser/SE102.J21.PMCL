#include "Player.h"
#include "Game.h"

#include "DXInput.h"
#include <string>

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
}


Player::~Player()
{
}

void Player::InitPlayer(float x, float y)
{
	CreateObject("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255), 22, 32);
	setX(x);
	setY(y + getHeight());
	setVelX(DEFAULT_VELOCITY_X);
	setVelY(DEFAULT_VELOCITY_Y);

	minHeight = getY();
	maxHeight = minHeight + 64;

	isMoving = directionChanged = startAnimation = isJumping = false;
	maxHeightReached = false;
	isOnGround = true;

	isInvincible = false;
	isKnockback = false;
}

void Player::SetStatus(PLAYER_STATUS status, int direction)
{
	if (this->status != status || directionChanged)
	{
		PLAYER_STATUS temp = this->status;
		this->status = status;
		directionX = direction;

		switch (status)
		{
		case PLAYER_STANDING:
			startAnimation = false;
			setSize(22, 32);
			sprite->SetAnimation(getWidth(), getHeight(), 4, 4, 0, 3);
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
			setVelX(DEFAULT_VELOCITY_X * direction);
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
			isJumping = true;

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
			isJumping = false;
			setY(getY() + 8);
			SetStatus(PLAYER_STANDING, direction);
			break;
		case PLAYER_KNOCKBACK:
			isKnockback = true;
			isMoving = false;
			isJumping = false;
			startKnockback = GetTickCount();
			directionY = 1;
			// trở nên bất tử
			SetStatus(PLAYER_INVINCIBLE, direction);

			setSize(24, 24);
			sprite->SetAnimation(getWidth(), getHeight(), 1, 1, 0, 0);
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
			this->status = temp;
			break;
		case PLAYER_INVINCIBLE:
			isInvincible = true;
			startInvincible = GetTickCount();
			this->status = temp;
			break;
		case PLAYER_ATTACK:

			break;
		case PLAYER_JUMP_ATTACK:

			break;
		case PLAYER_DIE:

			break;
		default: // chẳng làm gì cả
			break;
		}
	}
}

void Player::Update(DWORD dt)
{
	timer.tickPerAnim = dt;

	// Lấy một vài thông tin từ class Game
	int mapStart = Game::getInstance()->getStage()->getMapStart();
	int mapEnd = Game::getInstance()->getStage()->getMapEnd();
	float cameraX = Camera::getInstance()->getX();
	float cameraWidth = Camera::getInstance()->getWidth();

	// Chỉ xử lý các trạng thái chủ động khi không bị knockback
	if (!isKnockback)
	{
		// Xử lý di chuyển
		if (isMoving)
		{
			if (!collideGroundX)
			{
				selfMovingX();
			}
		}

		//
		if (directionChanged)
		{
			SetStatus(this->status, directionX);
			directionChanged = false;
		}

		// Xử lý nhảy
		if (isJumping)
		{
			// Tăng giảm độ cao
			if (directionY > 0)
			{
				setVelY(DEFAULT_VELOCITY_Y);
				selfMovingY();
			}

			// Nhảy lên điểm cao nhất thì ngưng
			if (getY() >= maxHeight)
			{
				directionY = -1;
				isOnGround = false;
			}

			if (getY() <= minHeight)
			{
				if (getY() < minHeight)
					setY(minHeight);
				isJumping = false;
				SetStatus(PLAYER_MOVING, directionX);
			}
		}
	}
	// Xử lý khi bị knockback
	else
	{
		// knockback trong khoảng thời gian KNOCKBACK_TIME
		if (GetTickCount() - startKnockback >= KNOCKBACK_TIME)
		{
			isKnockback = false;
			SetStatus(PLAYER_NULL, directionX);
			SetStatus(PLAYER_STANDING, directionX);
		}
		else
		{
			moveX(-DEFAULT_VELOCITY_X * directionX);

			if (directionY > 0)
			{
				moveY(DEFAULT_VELOCITY_Y * 1.6f);
			}
			if (getY() > (maxHeight - minHeight) / 2 + minHeight)
			{
				directionY = -1;
			}
		}
	}

	// Invincible
	if (isInvincible)
	{
		// Chỉ bất tử trong khoảng thời gian INVINCIBLE_TIME
		if (GetTickCount() - startInvincible >= INVINCIBLE_TIME)
		{
			isInvincible = false;
		}

		// làm hiệu ứng chớp chớp
	}

	// Gravity: nhân vật rớt xuống khi không đứng trên mặt đất chạm đất
	if (!isOnGround)
	{
		moveY(-DEFAULT_VELOCITY_Y);
	}
	// Kiểm tra xem nhân vật có đang đứng trên mặt đất hay không
	if (!isJumping && getY() > minHeight)
	{
		isOnGround = false;
	}
	if (getY() <= minHeight)
	{
		if (getY() < minHeight)
			setY(minHeight);
		isOnGround = true;
		isJumping = false;
	}


	// Update camera theo vị trí nhân vật
	if (getMidX() > mapStart + cameraWidth / 2 &&
		getMidX() < mapEnd - cameraWidth / 2)
	{
		Camera::getInstance()->trackPlayer(*this);
	}
	// Kiểm tra giới hạn di chuyển
	// giới hạn nhân vật trong map
	if (this->getX() < mapStart)
	{
		this->setX(mapStart);
	}
	// giới hạn camera trong map
	if (cameraX < mapStart)
	{
		Camera::getInstance()->setX(mapStart);
	}
	if (cameraX > mapEnd - cameraWidth)
	{
		Camera::getInstance()->setX(mapEnd - cameraWidth);
	}

	// Vẽ lên camera
	Draw();
}
