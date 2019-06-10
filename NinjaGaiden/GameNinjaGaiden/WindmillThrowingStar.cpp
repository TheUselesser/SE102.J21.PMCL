#include "WindmillThrowingStar.h"
#include "Player.h"


WindmillThrowingStar::WindmillThrowingStar()
{
	setSize(DEFAULT_WINDMILL_THROWING_STAR_WIDTH, DEFAULT_WINDMILL_THROWING_STAR_HEIGHT);
	isExist = false;
	setType(UIT_WINDMILL_THROWING_STAR);

	startAnimation = true;
	sprite->SetAnimation(getWidth(), getHeight(), 2, 2, 0, 1);
	sprite->LoadTexture("images/items/windmill_throwing_star.png", D3DCOLOR_XRGB(0, 128, 128));
}


WindmillThrowingStar::~WindmillThrowingStar()
{
}

void WindmillThrowingStar::Init(float x, float y)
{
	setX(x);
	setY(y);
	isExist = true;
	isUsed = false;
	startExist = GetTickCount();
	directionY = 1;
	//setObjectType(ENEMY_BOSS_3_BULLET);
	//setCollisionType(COLLISION_TYPE_ENEMY);
	setVelX(DEFAULT_WINDMILL_THROWING_STAR_VELOCITY * directionX);

}

void WindmillThrowingStar::Update()
{
	timer.tickPerAnim = 100;
	autoMove();
	Draw();
}

void WindmillThrowingStar::autoMove()	[incompleted]
{
	Player * player = Player::getInstance();
	minY = player->getMidY();
	maxY = minY + 96;

	if (getMidX() >= spawnX - range && getMidX() <= spawnX + range)
	{
		if (!movingY)
			selfMovingX();
		else
		{
			if (getMidY() >= minY && getMidY() <= maxY)
				selfMovingY();
			else
			{
				directionY *= -1;
				movingY = false;

				if (getMidY() < minY)
					setY(minY + getHeight() / 2);
				else
					setY(maxY + getHeight() / 2);


			}
		}
	}
	else
	{
		directionX *= -1;
		movingY = true;

		if (getMidX() < spawnX - range)
			setX(spawnX - range - getWidth() / 2);
		else
			setX(spawnX + range - getWidth() / 2);

		spawnX = player->getMidX();
	}

	if (GetTickCount() - startExist >= EXIST_TIME)
	{
		isExist = false;
	}
}
