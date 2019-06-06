#pragma once
#include "GameObject.h"


enum ENEMY_STATUS
{
	ENEMY_NULL,
	ENEMY_STANDING,
	ENEMY_MOVING,
};

class Enemy :
	public GameObject
{
protected: 
	ENEMY_STATUS status;
	float spawnX, spawnY;

	// Dành cho mấy con có di chuyển lên xuống
	float minY, maxY;
public:
	bool directionChanged;

	Enemy();
	~Enemy();

	void CheckCollisionStatus(GameObject * player);

	virtual void setSpawned(bool isSpawned);
	virtual void SetStatus(ENEMY_STATUS status);
	/*virtual*/ void Update(DWORD dt, GameObject &player);
	virtual void autoMove(float) {}
};

