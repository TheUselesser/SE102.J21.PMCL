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
	// Dành cho mấy con có di chuyển lên xuống
	float minY, maxY;
public:
	bool directionChanged;
	float spawnX;

	Enemy();
	~Enemy();

	virtual void setSpawned(bool isSpawned);
	virtual void SetStatus(ENEMY_STATUS status, int direction);
	virtual void Update();
	virtual void autoMove();
};

