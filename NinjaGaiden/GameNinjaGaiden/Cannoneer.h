#pragma once
#include "Enemy.h"

#define	DEFAULT_CANNONEER_WIDTH		24
#define DEFAULT_CANNONEER_HEIGHT	30

class Cannoneer :
	public Enemy
{
public:
	Cannoneer();
	Cannoneer(float x, float y);
	~Cannoneer();

	void Init(GameObject * player);

	//void SetStatus(ENEMY_STATUS status);
	void Update(DWORD dt, GameObject &player);

	//void autoMove(float range);
};

