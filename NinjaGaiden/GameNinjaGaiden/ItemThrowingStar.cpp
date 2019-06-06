#include "ItemThrowingStar.h"
#include "Player.h"

ItemThrowingStar::ItemThrowingStar()
{
}


ItemThrowingStar::~ItemThrowingStar()
{
}

void ItemThrowingStar::setType(ITEM_THROWING_STAR_TYPE type)
{
	this->type = type;
}

ITEM_THROWING_STAR_TYPE ItemThrowingStar::getType()
{
	return type;
}

void ItemThrowingStar::Init(float x, float y)
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
	case ITEM_THROWING_STAR:
		type = THROWING_STAR;
		sprite->LoadTexture("images/items/item_throwing_star.png", D3DCOLOR_XRGB(255, 255, 255));
		break;
	}
}

void ItemThrowingStar::CheckCollisionStatus(GameObject * player)
{
	if (this->isExist)
	{
		if (collisionTime < 1.0f && !player->isAttacking)
		{
			if (type == THROWING_STAR)
			{
				Player::getInstance()->setItem(this);
				isExist = false;
			}
		}
	}
}

void ItemThrowingStar::UseItem()
{
	if (Player::getInstance()->getSpiritualStrength() >= SPIRITUAL_STRENGTH_REQUIRE)
	{
		Player::getInstance()->decrease_spiritualStrength(SPIRITUAL_STRENGTH_REQUIRE);


	}
}
