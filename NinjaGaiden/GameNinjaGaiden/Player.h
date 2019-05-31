﻿#pragma once
#include "GameObject.h"

#define DEFAULT_VELOCITY_X	4
#define DEFAULT_VELOCITY_Y	8
#define DEFAULT_WIDTH 20
#define DEFAULT_HEIGHT	32

// bị đẩy lùi delay 0.5s
#define KNOCKBACK_TIME 500
// mỗi lần bất tử sẽ kéo dài 3s từ lúc bắt đầu bị knockback do va chạm enemy
#define INVINCIBLE_TIME 3000
// mỗi đòn tấn công kéo dài 0.3s
#define STAND_ATTACK_TIME 500
#define JUMP_ATTACK_TIME 500

class Player :
	public GameObject
{
	static Player * instance;

	PLAYER_STATUS status;

	DWORD startKnockback;
	DWORD startInvincible;
	DWORD startAttack;

	float minHeight, maxHeight;
	bool maxHeightReached;

	// thêm các chỉ số máu, số mạng, vũ khí . . .
	int HP, maxHP;
	int life;

	Player();
	~Player();
public:
	bool isMovable;
	bool isMoving;
	bool directionChanged;

	// attack animation trick
	bool X_moved;

	static Player * getInstance();
	
	void setMinJumpHeight(float minHeight);
	float getMinJumpHeight() { return minHeight; }
	void resetMaxJumpHeight() { maxHeight = minHeight + 64; }
	float getMaxJumpHeight() { return maxHeight; }
	float getDefaultPlayerWidth() { return DEFAULT_WIDTH; }
	float getDefaultPlayerHeight() { return DEFAULT_HEIGHT; }
	float getRealWidth() { return realWidth; }

	// Nhập x, y là điểm dưới trái của nhân vật
	void InitPlayer(float x, float y);
	void SetStatus(PLAYER_STATUS status, int direction = 1);
	void Update(DWORD dt);
};

