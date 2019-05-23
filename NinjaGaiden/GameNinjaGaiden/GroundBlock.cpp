#include "GroundBlock.h"
#include "Collision.h"


GroundBlock::GroundBlock(float x, float y, float width, float height, int groundType)
{
	setObjectType(OBJECT_GROUND);
	setCollisionType(COLLISION_TYPE_GROUND);
	this->groundType = groundType;
	setSize(width, height);
	setX(x);
	setY(y + getHeight());
}

GroundBlock::~GroundBlock()
{
}

bool GroundBlock::blockMovingX()
{
	return groundType == 1 ? true : false;
}

void GroundBlock::Update(DWORD dt, GameObject &player)
{
	// Xét va chạm ?
	Collision::GroundCollisionCheck(player, *this);

	if (1)
	{
	}
}
