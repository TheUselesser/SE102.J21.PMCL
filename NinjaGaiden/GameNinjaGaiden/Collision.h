#pragma once
#include "Rect.h"
#include "Sprite.h"

class Collision
{
public:
	Collision();
	~Collision();

	static float CollisionTime, EntryTime, ExitTime;

	static Sprite GetSweptBroadphaseBox(Sprite box);	// tạo cái hộp to đùng bao bọc không gian chưa vị trí trước và sau di chuyển của box
	static float SweptAABB(Sprite box1, Sprite box2, float &normalX, float &normalY);
	static bool AABBCheck(Rect box1, Rect box2);
	static void CollisionHandle(Sprite &box1, Sprite box2);
};

