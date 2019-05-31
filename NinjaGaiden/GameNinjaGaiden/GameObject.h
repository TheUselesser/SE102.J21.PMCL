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
	ENEMY_CANNONEER = 14,			//
	ENEMY_RUNNER = 15,				//
	ITEM_ITEM_CONTAINER_1 = 21,
	ITEM_ITEM_CONTAINER_2 = 22,
	ITEM_ITEM_CONTAINER_3 = 23,
	ITEM_ITEM_CONTAINER_4 = 24,		//
	OBJECT_GROUND,
};
// loại va chạm
enum COLLISION_TYPE
{
	COLLISION_TYPE_GROUND,
	COLLISION_TYPE_ENEMY,
	COLLISION_TYPE_ITEM
};

// trạng thái của player
enum PLAYER_STATUS
{
	PLAYER_NULL,
	PLAYER_STANDING,
	PLAYER_MOVING,
	PLAYER_JUMPING,
	PLAYER_END_JUMPING,
	PLAYER_KNOCKBACK,
	PLAYER_INVINCIBLE,
	PLAYER_ATTACK,
	PLAYER_JUMP_ATTACK,
	PLAYER_DIE,
};

class GameObject :
	public MovableRect
{
protected:
	Sprite * sprite;
	ObjTimer timer;

	GAME_OBJECT_TYPE objType;
	COLLISION_TYPE clsType;

	void Draw();
	void CreateObject(const char * imagePath, D3DCOLOR transColor, float width, float height);

	float realWidth;
public:
	float nx = 0.0f, ny = 0.0f;
	float collisionTime = 1.0f;

	bool startAnimation;
	int directionX, directionY;
	bool isExist;
	bool isInCellsSet;

	// player only
	bool isJumping;
	bool isInvincible;
	bool isKnockback;
	bool isOnGround;
	bool isAttacking;
	bool isDead;

	bool collideGroundX;

	GameObject();
	~GameObject();

	// Player only
	virtual void setMinJumpHeight(float) {}
	virtual float getMinJumpHeight() { return 0.0f; }
	virtual void resetMaxJumpHeight() {}
	virtual float getMaxJumpHeight() { return 0.0f; }
	virtual float getDefaultPlayerWidth() { return 0.0f; }
	virtual float getDefaultPlayerHeight() { return 0.0f; }
	virtual void SetStatus(PLAYER_STATUS, int = 1) {}
	virtual float getRealWidth() { return 0.0f; }

	// Dùng trong Collision.cpp để lấy thông tin va chạm (cụ thể là với nhân vật)
	void UpdateCollisionStatus(int nx, int ny, float collisionTime);
	virtual void CheckCollisionStatus(GameObject * player);

	// Enemies, items, ground
	void setObjectType(GAME_OBJECT_TYPE objectType);
	GAME_OBJECT_TYPE getObjectType();
	void setCollisionType(COLLISION_TYPE collisionType);
	COLLISION_TYPE getCollisionType();

	// General
	virtual void Init();
	virtual void Init(GameObject * player) {}
	virtual void setSpawned(bool isSpawned) {}	// dành cho enemy	
	virtual void Update(DWORD dt, GameObject &player);
};

