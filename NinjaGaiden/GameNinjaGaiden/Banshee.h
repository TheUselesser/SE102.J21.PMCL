#pragma once
#include "Enemy.h"

#define DEFAULT_BANSHEE_WIDTH	24
#define DEFAULT_BANSHEE_HEIGHT	32

class Banshee :
	public Enemy
{


public:
	Banshee();
	Banshee(float x, float y);
	~Banshee();

	void SetStatus(ENEMY_STATUS status);
	void Update(DWORD dt);

	void autoMove(float range);
};

