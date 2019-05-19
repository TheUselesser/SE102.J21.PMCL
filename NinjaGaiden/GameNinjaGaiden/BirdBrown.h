#pragma once
#include "Enemy.h"

#define DEFAULT_BIRDBROWN_WIDTH		18
#define DEFAULT_BIRDBROWN_HEIGHT	30

class BirdBrown :
	public Enemy
{


public:
	BirdBrown();
	BirdBrown(float x, float y);
	~BirdBrown();

	void SetStatus(ENEMY_STATUS status);
	void Update(DWORD dt);

	void autoMove(float range);
};

