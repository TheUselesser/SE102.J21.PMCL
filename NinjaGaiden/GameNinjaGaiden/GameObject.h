#pragma once
#include "MovableRect.h"
#include "Sprite.h"

typedef struct SpriteTimer
{
	DWORD startTime = 0;
	DWORD tickPerAnim = 0;

	bool isAtTime()
	{
		if (GetTickCount() - startTime >= tickPerAnim)
		{
			startTime = GetTickCount();
			return true;
		}
		return false;
	}
} ObjTimer;

// loại đối tượng (bao gồm enemy và item)
enum GAME_OBJECT_TYPE
{
	ENEMY_ARMOR_SWORD_MAN = 1,
	ENEMY_BANSHEE = 2,				//
	ENEMY_BAT_BROWN = 3,			//
	ENEMY_BAT_MAN = 4,
	ENEMY_BIRD_BROWN = 5,			//
	ENEMY_BIRD_GREY = 6,
	ENEMY_BOXER = 7,
	ENEMY_DOG = 8,
	ENEMY_KNIFE_THROWER = 9,
	ENEMY_MACHINE_GUN_GUY = 10,		//
	ENEMY_GUNNER = 11,
	ENEMY_SWORD_MAN = 12,			//
	ENEMY_CAT = 13,					//
	ENEMY_CANNONEER = 14,
	ENEMY_RUNNER = 15,
	ITEM_ITEM_CONTAINER_1 = 21,
	ITEM_ITEM_CONTAINER_2 = 22,
	ITEM_ITEM_CONTAINER_3 = 23,
	ITEM_ITEM_CONTAINER_4 = 24
};

class GameObject :
	public MovableRect
{
protected:
	Sprite * sprite;
	ObjTimer timer;

	GAME_OBJECT_TYPE objType;

	void Draw();
	void CreateObject(const char * imagePath, D3DCOLOR transColor, float width, float height);

public:
	bool startAnimation;
	int directionX, directionY;
	bool isOnCollisionX, isOnCollisionY;
	bool isExist;

	GameObject();
	~GameObject();

	void setObjectType(GAME_OBJECT_TYPE objType);
	GAME_OBJECT_TYPE getObjectType();

	virtual void Init();
	virtual void setSpawned(bool isSpawned) {}	// dành cho enemy	
	virtual void SetStatus();
	virtual void Update(DWORD dt, GameObject &player);
};

