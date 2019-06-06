#include "Item.h"

#define SPIRITUAL_STRENGTH_REQUIRE 5

enum ITEM_WINDMILL_THROWING_STAR_TYPE
{
	WINDMILL_THROWING_STAR,
};

class ItemWindmillThrowingStar :
	public Item
{
	ITEM_WINDMILL_THROWING_STAR_TYPE type;
public:
	ItemWindmillThrowingStar();
	~ItemWindmillThrowingStar();

	void setType(ITEM_WINDMILL_THROWING_STAR_TYPE type);
	ITEM_WINDMILL_THROWING_STAR_TYPE getType();

	void Init(float x, float y);
	void CheckCollisionStatus(GameObject * player);

	void UseItem();
};



