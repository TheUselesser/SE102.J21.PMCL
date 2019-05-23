﻿#pragma once
#include "GameObject.h"

class Collision
{
public:
	Collision();
	~Collision();

	// đặc cách
	static void GroundCollisionCheck(GameObject &box, GameObject &groundBlock);

	static MovableRect GetSweptBroadphaseBox(MovableRect box);	// tạo cái hộp to đùng bao bọc không gian chưa vị trí trước và sau di chuyển của box
	static float SweptAABB(MovableRect box1, MovableRect box2, float &normalX, float &normalY);
	static bool AABBCheck(Rect box1, Rect box2);
	static void CollisionHandle(GameObject &box1, GameObject &box2);
};

