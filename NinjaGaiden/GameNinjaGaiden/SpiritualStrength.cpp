#include "SpiritualStrength.h"



SpiritualStrength::SpiritualStrength()
{
}


SpiritualStrength::~SpiritualStrength()
{
}

void SpiritualStrength::setType(SPIRITUAL_STRENGTH_TYPE type)
{
	this->type = type;
}

SPIRITUAL_STRENGTH_TYPE SpiritualStrength::getType()
{
	return type;
}

void SpiritualStrength::Init(float x, float y)
{
	isExist = true;
	setX(x);
	setY(y);
	setSize(15, 15);
	setCollisionType(COLLISION_TYPE_ITEM);

	startAnimation = false;
	sprite->SetAnimation(getWidth(), getHeight(), 1, 1, 0, 0);
	switch (getItemType())
	{
	case ITEM_SPIRITUAL_STR_5_PTS:
		type = SPIRITUAL_STRENGTH_5;
		sprite->LoadTexture("images/items/item_spiritual_strength_5.png", D3DCOLOR_XRGB(255, 255, 255));
		break;
	case ITEM_SPIRITUAL_STR_10_PTS:
		type = SPIRITUAL_STRENGTH_10;
		sprite->LoadTexture("images/items/item_spiritual_strength_10.png", D3DCOLOR_XRGB(255, 255, 255));
		break;
	}
}

void SpiritualStrength::Update(DWORD dt, GameObject & player)
{
	Draw();
}

void SpiritualStrength::CheckCollisionStatus(GameObject * player)
{
	if (this->isExist && !player->isInvincible)
	{
		if (collisionTime < 1.0f)
		{
			// Xử lý khi player nhặt item
			// cái thứ này chỉ có 2 loại là +5 và +10
			if (type == SPIRITUAL_STRENGTH_5)
			{
				
			}
			else
			{
				
			}
		}
	}
}
