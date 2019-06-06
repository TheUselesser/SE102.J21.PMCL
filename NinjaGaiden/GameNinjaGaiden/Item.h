#pragma once
#include "GameObject.h"
class Item :
	public GameObject
{
	int itemTypeID;
	ITEM_TYPE itemType;
public:
	Item();
	virtual ~Item();

	int	getItemTypeID();		void setItemTypeID(int id);
	ITEM_TYPE getItemType();	void setItemType(ITEM_TYPE itemType);

	virtual void Init(float x, float y) {}
	virtual void Init(float x, float y, int itemTypeID) {}
	virtual void Init(float x, float y, ITEM_TYPE itemType) {}

	void Update(DWORD dt, GameObject &player);
	virtual void CheckCollisionStatus(GameObject * player) {}

	// for weapons only
	virtual void UseItem() {}
};

