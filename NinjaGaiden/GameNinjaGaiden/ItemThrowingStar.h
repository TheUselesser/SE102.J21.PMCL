#include "Item.h"

#define SPIRITUAL_STRENGTH_REQUIRE 3

enum ITEM_THROWING_STAR_TYPE
{
	THROWING_STAR,
};

class ItemThrowingStar :
	public Item
{
	ITEM_THROWING_STAR_TYPE type;
public:
	ItemThrowingStar();
	~ItemThrowingStar();

	void setType(ITEM_THROWING_STAR_TYPE type);
	ITEM_THROWING_STAR_TYPE getType();

	void Init(float x, float y);
	void CheckCollisionStatus(GameObject * player);

	void UseItem();
};



