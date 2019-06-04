#include "Player.h"
#include "Game.h"

#include "DXInput.h"
#include <string>


Player * Player::instance = NULL;
Player * Player::getInstance()
{
	if (instance == NULL)
		instance = new Player();
	return instance;
}

Player::Player()
{
}


Player::~Player()
{
}

void Player::InitPlayer(float x, float y)
{
	//CreateObject("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255), 22, 32);
	SetStatus(PLAYER_NULL);
	SetStatus(PLAYER_STANDING);
	setX(x);
	setY(y + getHeight());
	setVelX(DEFAULT_VELOCITY_X);
	setVelY(DEFAULT_VELOCITY_Y);

	minHeight = getY();
	maxHeight = minHeight + DEFAULT_JUMP_HEIGHT;

	isMovable = true;
	isMoving = directionChanged = startAnimation = isJumping = false;
	maxHeightReached = false;
	isOnGround = true;

	isInvincible = false;
	isKnockback = false;

	isDead = false;

	X_moved = false;
}

void Player::setMinJumpHeight(float minHeight)
{
	this->minHeight = minHeight + DEFAULT_HEIGHT;
	if (getY() > this->minHeight)
	{
		if (!isJumping) isOnGround = false;
	}
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
			realWidth = 20;
			sprite->SetAnimation(getWidth(), getHeight(), 1, 1, 0, 0);
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
			SetStatus(PLAYER_STANDING, directionX);
			this->status = PLAYER_MOVING;
			startAnimation = true;
			sprite->SetAnimation(getWidth(), getHeight(), 4, 4, 1, 3);
			break;
		case PLAYER_JUMPING:
			if (isClimbing) isClimbing = false;
			isJumping = true;

			startAnimation = true;
			setSize(24, 24);
			realWidth = 24;
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
			isMovable = false;
			isMoving = false;
			isClimbing = false;
			isJumping = false;
			isOnGround = true;
			startKnockback = GetTickCount();
			directionY = 1;
			// trở nên bất tử
			SetStatus(PLAYER_INVINCIBLE, direction);

			setSize(24, 24);
			realWidth = 24;
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
			if (!isJumping && isOnGround) isMovable = false;
			if (!isAttacking) startAttack = GetTickCount();
			isAttacking = true;
			
			startAnimation = true;
			setSize(40, 32);
			realWidth = 20;
			sprite->SetAnimation(getWidth(), getHeight(), 3, 3, 0, 2);
			if (direction > 0)
			{
				if (X_moved)
				{
					moveX(16);
					X_moved = false;
				}
				sprite->Release();
				sprite->LoadTexture("images/Ryu_attack_right.png", D3DCOLOR_XRGB(255, 163, 177));
			}
			else
			{
				sprite->Release();
				if (!X_moved)
				{
					moveX(-16);
					X_moved = true;
				}
				sprite->LoadTexture("images/Ryu_attack_left.png", D3DCOLOR_XRGB(255, 163, 177));
			}
			break;
		case PLAYER_JUMP_ATTACK:
			if (!isAttacking) startAttack = GetTickCount();
			isAttacking = true;

			startAnimation = true;
			setSize(32, 32);
			realWidth = 24;
			sprite->SetAnimation(getWidth(), getHeight(), 4, 2, 0, 3);
			if (direction > 0)
			{
				sprite->Release();
				sprite->LoadTexture("images/Ryu_jump_attack_right.png", D3DCOLOR_XRGB(255, 163, 177));
			}
			else
			{
				sprite->Release();
				sprite->LoadTexture("images/Ryu_jump_attack_left.png", D3DCOLOR_XRGB(255, 163, 177));
			}
			break;
		case PLAYER_CLINGING:
			isClimbing = true;
			if (isMoving) isMoving = false;
			if (isJumping) isJumping = false;
			isOnGround = true;
			if (isAttacking) isAttacking = false;

			startAnimation = false;
			setSize(16, 32);
			realWidth = 16;
			sprite->SetAnimation(getWidth(), getHeight(), 1, 1, 0, 0);
			if (direction > 0)
			{
				sprite->Release();
				sprite->LoadTexture("images/Ryu_climb_right.png", D3DCOLOR_XRGB(255, 0, 255));
			}
			else
			{
				sprite->Release();
				sprite->LoadTexture("images/Ryu_climb_left.png", D3DCOLOR_XRGB(255, 0, 255));
			}
			break;
		case PLAYER_CLIMBING:
			setVelY(DEFAULT_CLIMB_VELOCITY * directionY);
			SetStatus(PLAYER_CLINGING, directionX);
			this->status = PLAYER_CLIMBING;
			startAnimation = true;
			sprite->SetAnimation(getWidth(), getHeight(), 2, 2, 0, 1);
			break;
		case PLAYER_DIE:
			isDead = true;
			break;
		default: // chẳng làm gì cả
			break;
		}
	}
}

void Player::Update(DWORD dt)
{
	timer.tickPerAnim = dt;
	if (isOnGround && !isJumping && isAttacking)
	{
		timer.tickPerAnim = STAND_ATTACK_TIME / 3;
	}

	// Lấy một vài thông tin từ class Game
	int mapStart = Game::getInstance()->getStage()->getMapStart();
	int mapEnd = Game::getInstance()->getStage()->getMapEnd();
	float cameraX = Camera::getInstance()->getX();
	float cameraWidth = Camera::getInstance()->getWidth();

	// Chỉ xử lý các trạng thái chủ động khi không bị knockback
	if (!isKnockback)
	{
		// lên xuống qua lại
		if (directionChanged)
		{
			SetStatus(this->status, directionX);
			directionChanged = false;
		}

		// Xử lý di chuyển
		if (isMovable)	// chủ yếu để ép player đứng yên khi đánh lúc đang đứng
		{
			if (isMoving)
			{
				if (isClimbing)
				{
					selfMovingY();
				}
				else
				{
					selfMovingX();
				}
			}
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
			if ((!isAttacking && getY() >= maxHeight) ||
				(isAttacking && getY() >= maxHeight + getWidth() - getRealWidth()))
			{
				directionY = -1;
				isOnGround = false;

				// tạm để đây
				//isJumpable = false;
				//startCooldownJump = GetTickCount();
			}

			if (getY() <= minHeight)
			{
				isJumping = false;
				SetStatus(PLAYER_MOVING, directionX);
			}
		}

		// Xử lý tấn công
		if (isAttacking)
		{
			// Khi đứng
			if (isOnGround && !isJumping && !isClimbing)
			{
				//timer.tickPerAnim = STAND_ATTACK_TIME / 3;
				if (GetTickCount() - startAttack >= STAND_ATTACK_TIME)
				{
					isAttacking = false;
					isMovable = true;
					if (X_moved)
					{
						moveX(16);
						X_moved = false;
					}
					SetStatus(PLAYER_STANDING, directionX);
				}
				else
				{
					SetStatus(PLAYER_ATTACK, directionX);
				}
			}
			// Khi trên không
			else
			{
				if (GetTickCount() - startAttack >= JUMP_ATTACK_TIME || (isOnGround && !isJumping))
				{
					isAttacking = false;

					if (isOnGround && !isJumping)
					{
						SetStatus(PLAYER_MOVING, directionX);
					}
					else
					{
						SetStatus(PLAYER_JUMPING, directionX);
					}
				}
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
			isMovable = true;
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
			if (getY() > (maxHeight + minHeight) / 2)
			{
				directionY = -1;
			}
		}
	}

	// ?
	if (!isJumpable)
	{
		if (GetTickCount() - startCooldownJump >= COOLDOWN_JUMP)
		{
			isJumpable = true;
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
	if (!isJumping && !isClimbing && getY() > minHeight)
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
		getMidX() < mapEnd - cameraWidth / 2 && 
		(status != PLAYER_ATTACK))
	{
		Camera::getInstance()->trackPlayer(this);
	}
	// Kiểm tra giới hạn di chuyển
	// giới hạn nhân vật trong map
	if (this->getRight() - this->getRealWidth() < mapStart && this->getRight() > mapStart)
	{
		this->setX(mapStart - this->getWidth() + this->getRealWidth());
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

	// Chết thì làm sao
	if (isDead)
	{
		// thêm điều kiện nếu còn mạng thì mới init
		if (isOnGround) Game::getInstance()->init();
	}
}
