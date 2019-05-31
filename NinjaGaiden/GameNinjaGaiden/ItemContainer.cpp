#include "ItemContainer.h"
#include <string>

ItemContainer::ItemContainer()
{
}

ItemContainer::ItemContainer(float x, float y)
{
	setSize(DEFAULT_ITEM_CONTAINER_WIDTH, DEFAULT_ITEM_CONTAINER_HEIGHT);
	setX(x);
	setY(y + getHeight());
	isExist = false;
}


ItemContainer::~ItemContainer()
{
}

void ItemContainer::Init(GameObject * player)
{
	isExist = true;
	startAnimation = true;

	sprite->SetAnimation(getWidth(), getHeight(), 2, 2, 0, 1);

	int type = 4;
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

void ItemContainer::Update(DWORD dt, GameObject & player)
{
	timer.tickPerAnim = dt;
	Draw();
}
