#pragma once
#include <d3d9.h>
#include "Player.h"

class Camera :
	public RECT
{
	float posX, posY;
	float width, height;
	float velX, velY;

public:
	Camera();
	~Camera();

	float getX(); void setX(float x);
	float getY(); void setY(float y);
	float getWidth(); void setWidth(float width);
	float getHeight(); void setHeight(float height);
	float getVelX(); void setVelX(float velX);
	float getVelY(); void setVelY(float velY);
	
	void moveLeft(); void moveRight();
	void trackSprite(Sprite sprite);
	void untrackSprite(Sprite sprite);
};

