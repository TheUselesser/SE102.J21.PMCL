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
	
public:
	bool directionChanged;
	bool isExist;
	float spawnX;

	Enemy();
	~Enemy();

	virtual void setSpawned(bool isSpawned);
	virtual void SetStatus(ENEMY_STATUS status, int direction);
	virtual void Update();
	virtual void autoMove();
};

