#pragma once
#include "UsableItem.h"

#define DEFAULT_WINDMILL_THROWING_STAR_WIDTH	16
#define DEFAULT_WINDMILL_THROWING_STAR_HEIGHT	16
#define DEFAULT_WINDMILL_THROWING_STAR_VELOCITY	8

// thời gian phi tiêu windmill tồn tại
#define EXIST_TIME 5000

class WindmillThrowingStar :
	public UsableItem
{
	float range = 72;
	float spawnX, spawnY;
	float minY, maxY;
	float currentY, nextY;
	bool movingY = false;

	DWORD startExist;
public:
	WindmillThrowingStar();
	~WindmillThrowingStar();

	void Init(float x, float y);

	void Update();

	void autoMove();
};

