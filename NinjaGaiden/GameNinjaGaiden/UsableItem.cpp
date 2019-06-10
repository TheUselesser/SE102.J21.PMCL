#include "UsableItem.h"
#include "Player.h"


UsableItem::UsableItem()
{
}


UsableItem::~UsableItem()
{
}

void UsableItem::UseItem()
{
	switch (type)
	{
	case UIT_THROWING_STAR:
		spiritualStrengthCost = 3;
		break;
	case UIT_WINDMILL_THROWING_STAR:
		spiritualStrengthCost = 5;
		break;
	}

	Player * player = Player::getInstance();
	if (player->getSpiritualStrength() >= spiritualStrengthCost)
	{
		player->decrease_spiritualStrength(spiritualStrengthCost);
		player->SetStatus(PLAYER_ITEM_USE, player->directionX);

		isExist = true;
		isUsed = true;
	}
}
