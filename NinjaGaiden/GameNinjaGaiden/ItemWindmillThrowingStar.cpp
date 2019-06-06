#include "ItemWindmillThrowingStar.h"
#include "Player.h"

ItemWindmillThrowingStar::ItemWindmillThrowingStar()
{
}


ItemWindmillThrowingStar::~ItemWindmillThrowingStar()
{
}

void ItemWindmillThrowingStar::setType(ITEM_WINDMILL_THROWING_STAR_TYPE type)
{
	this->type = type;
}

ITEM_WINDMILL_THROWING_STAR_TYPE ItemWindmillThrowingStar::getType()
{
	return type;
}

void ItemWindmillThrowingStar::Init(float x, float y)
{
	isExist = true;
	setX(x);
	setY(y);
	setSize(16, 16);
	setCollisionType(COLLISION_TYPE_ITEM);

	startAnimation = false;
	sprite->SetAnimation(getWidth(), getHeight(), 1, 1, 0, 0);
	switch (getItemType())
	{
	case ITEM_WINDMILL_THROWING_STAR:
		type = WINDMILL_THROWING_STAR;
		sprite->LoadTexture("images/items/item_windmill_throwing_star.png", D3DCOLOR_XRGB(255, 255, 255));
		break;
	}
}

void ItemWindmillThrowingStar::CheckCollisionStatus(GameObject * player)
{
	if (this->isExist)
	{
		if (collisionTime > 1.0f && !player->isAttacking)
		{
			if (type == WINDMILL_THROWING_STAR)
			{
				Player::getInstance()->setItem(this);
				isExist = false;
			}
			
		}
		
	}
}

void ItemWindmillThrowingStar::UseItem()
{
	if (Player::getInstance()->getSpiritualStrength() >= SPIRITUAL_STRENGTH_REQUIRE)
	{
		Player::getInstance()->decrease_spiritualStrength(SPIRITUAL_STRENGTH_REQUIRE);


	}
}
