#pragma once
#include "Item.h"

#define DEFAULT_ITEM_CONTAINER_WIDTH	15
#define DEFAULT_ITEM_CONTAINER_HEIGHT	14

class ItemContainer :
	public Item
{
public:
	ItemContainer();
	ItemContainer(float x, float y);
	~ItemContainer();

	void Init(GameObject * player);
	void Update(DWORD dt, GameObject &player);
};

