#pragma once
#include "GameObject.h"


enum ENEMY_STATUS
{
	ENEMY_NULL,
	ENEMY_STANDING,
	ENEMY_MOVING,
	ENEMY_ATTACKING,
};

enum ENEMY_MOVE_TYPE
{
	MT_ON_ONE_GROUND,
	MT_FROM_HIGHER_TO_LOWER_GROUND,
};

class Enemy :
	public GameObject
{
protected: 
	ENEMY_STATUS status;
	float spawnX, spawnY;

	ENEMY_MOVE_TYPE moveType;
	float currentGroundY;	// hihihi

	// Dành cho mấy con có di chuyển lên xuống
	float minY, maxY;

	// Dành cho mấy con có đòn đánh riêng
	DWORD startCooldown;

	bool isAlive = true;

	void getHitByPlayer();
public:
	bool directionChanged;

	Enemy();
	~Enemy();

	void CheckCollisionStatus(GameObject * player);
	void Die();

	virtual void setSpawned(bool isSpawned);
	virtual void SetStatus(ENEMY_STATUS status);
	/*virtual*/ void Update(DWORD dt, GameObject &player);
	virtual void autoMove(float) {}
	virtual void periodAttack(DWORD cooldown) {}

	void MindTheGroundBlocks();
};

