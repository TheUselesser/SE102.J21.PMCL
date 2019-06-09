﻿#pragma once
#include "Enemy.h"
#include "Boss3Bullet.h"

#define DEFAULT_BOSS_3_WIDTH	40
#define DEFAULT_BOSS_3_HEIGHT	54
#define DEFAULT_BOSS_3_VELOCITY	8

// delay jump của boss 3
#define COOLDOWN_JUMP 2400

#define MAX_HP 16

class Boss3 :
	public Enemy
{	
	// chính xác là 3 viên luôn ấy
	Boss3Bullet	* bullet[3];
	// Tấn công vào mỗi phát nhảy thứ 3
	int jumpCount;

	int HP;

public:
	Boss3();
	Boss3(float x, float y);
	~Boss3();

	void Init(GameObject * player);
	void SetStatus(ENEMY_STATUS status);
	void Update(DWORD dt, GameObject &player);

	void autoMove(float range);
	void periodAttack(DWORD cooldown);

	bool isBoss() { return true; }
	int getMaxHP() { return MAX_HP; }
	int getHP() { return HP; }
	void decrease_HP(int HP = 1) { this->HP -= HP; }
	void resetAllStats();
};

