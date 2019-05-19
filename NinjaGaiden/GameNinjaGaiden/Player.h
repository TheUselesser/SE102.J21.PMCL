#pragma once
#include "GameObject.h"


enum PLAYER_STATUS
{
	PLAYER_NULL,
	PLAYER_STANDING,
	PLAYER_MOVING,
	PLAYER_JUMPING,
	PLAYER_END_JUMPING,
	PLAYER_FALLING,
	// PLAYER_ATTACKING,
	// Player bất tử
	// Player chết
};

class Player :
	public GameObject
{
	//static Player * instance;

	PLAYER_STATUS status;

	// thêm các chỉ số, máu, nước, số mạng, sức mạnh . . .

public:
	float currentHeight, maxHeight;
	bool isMoving;
	bool directionChanged;
	bool isJumping;
	bool maxHeightReached;
	bool isFalling;

	//static Player * getInstance();

	Player();
	~Player();
	
	void InitPlayer(float x, float y);
	void SetStatus(PLAYER_STATUS status, int direction = 1);
	void Update(DWORD dt);
};

