#include "ItemContainer.h"
#include "SpiritualStrength.h"

#include <string>

ItemContainer::ItemContainer()
{
}

ItemContainer::ItemContainer(float x, float y)
{
	setSize(DEFAULT_ITEM_CONTAINER_WIDTH, DEFAULT_ITEM_CONTAINER_HEIGHT);
	setX(x);
	setY(y + getHeight());
	isExist = true;
	setCollisionType(COLLISION_TYPE_NOTHING);
	isBroken = false;
}


ItemContainer::~ItemContainer()
{
}

int ItemContainer::getItemTypeID()
{
	return itemTypeID;
}

void ItemContainer::setItemTypeID(int id)
{
	itemTypeID = id;
}

void ItemContainer::Init(GameObject * player)
{
	startAnimation = true;

	item = new Item();
	sprite->SetAnimation(getWidth(), getHeight(), 2, 2, 0, 1);

	//sprite->Release();
	switch (objType)
	{
	case ITEM_ITEM_CONTAINER_1:
		sprite->LoadTexture("images/items/ItemContainer4.png", D3DCOLOR_XRGB(255, 255, 255));
		break;
	case ITEM_ITEM_CONTAINER_2:
		sprite->LoadTexture("images/items/ItemContainer4.png", D3DCOLOR_XRGB(255, 255, 255));
		break;
	case ITEM_ITEM_CONTAINER_3:
		sprite->LoadTexture("images/items/ItemContainer4.png", D3DCOLOR_XRGB(255, 255, 255));
		break;
	case ITEM_ITEM_CONTAINER_4:
		sprite->LoadTexture("images/items/ItemContainer4.png", D3DCOLOR_XRGB(255, 255, 255));
		break;
	}
}

void ItemContainer::CheckCollisionStatus(GameObject * player)
{
	if (!isBroken)
	if (player->isAttacking)
	{
		if (collisionTime < 1.0f)
		{
			// Player phá hộp
			// player tấn công khi đang đứng
			if (!player->isJumping)
			{
				if (player->directionX * (this->getLeft() >= player->getLeft() ? 1 : -1) > 0)
				{
					//this->isExist = false;
					this->isBroken = true;
				}
			}
			// player tấn công khi đang nhảy (xoay vòng vòng nên khỏi xét hướng)
			else
			{
				//this->isExist = false;
				this->isBroken = true;
			}

			// Rớt item
			DropItem();
		}
	}
}

void ItemContainer::DropItem()
{
	std::string msg = std::to_string(itemTypeID);
	MessageBox(0, msg.c_str(), "id", 0);

	// Khởi tạo item
	switch (itemTypeID)
	{
	case 1:
		// Spiritual Strength 5 points
		MessageBox(0, "", "", 0);
		item = new SpiritualStrength();
		item->setItemTypeID(itemTypeID);
		break;
	case 2:
		// Spiritual Strength 10 points
		item = new SpiritualStrength();
		item->setItemTypeID(itemTypeID);
		break;
	case 3:
		//ITEM_TIME_FREEZE
		break;
	case 4:
		//ITEM_THROWING_STAR
		break;
	case 5:
		//ITEM_BONUS_500_PTS
		break;
	case 6:
		//ITEM_BONUS_1000_PTS
		break;
	case 7:
		//ITEM_FIRE_WHEEL_ART
		break;
	case 8:
		//ITEM_HP_RECOVER
		break;
	case 9:
		//ITEM_WINDMILL_THROWING_STAR
		break;
	default:
		break;
	}

	item->Init(getX(), getY());
}

void ItemContainer::Update(DWORD dt, GameObject & player)
{
	timer.tickPerAnim = dt;

	if (!isBroken)
	{
		Draw();
	}
	else
	{
		item->Update(dt, player);
	}
}
