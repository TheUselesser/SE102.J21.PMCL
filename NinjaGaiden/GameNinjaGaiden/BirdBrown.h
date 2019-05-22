#pragma once
#include "Enemy.h"

#define DEFAULT_BIRD_BROWN_WIDTH	18
#define DEFAULT_BIRD_BROWN_HEIGHT	30
#define DEFAULT_BIRD_BROWN_VELOCITY	4

class BirdBrown :
	public Enemy
{


public:
	BirdBrown();
	BirdBrown(float x, float y);
	~BirdBrown();

	void Init();

	void SetStatus(ENEMY_STATUS status);
	void Update(DWORD dt, GameObject &player);

	void autoMove(float range);
};

