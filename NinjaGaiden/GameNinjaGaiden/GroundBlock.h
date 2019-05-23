#pragma once
#include "GameObject.h"
class GroundBlock :
	public GameObject
{
	// 2 loại ground: 1.Có va chạm ngang, 2.Không va chạm ngang
	int groundType;
public:
	GroundBlock(float x, float y, float width, float height, int groundType);
	~GroundBlock();

	bool blockMovingX();

	void Update(DWORD dt, GameObject &player);
};

