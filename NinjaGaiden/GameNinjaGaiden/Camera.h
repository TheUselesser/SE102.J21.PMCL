#pragma once
#include "Sprite.h"
#include "Rect.h"

class Camera :
	public Rect
{
	static Camera * instance;

	float velX, velY;

public:
	static Camera * getInstance();

	Camera();
	~Camera();

	float getVelX();	void setVelX(float velX);
	float getVelY();	void setVelY(float velY);
	
	void moveLeft(); void moveRight();
	void updateRect();
	void trackSprite(Sprite sprite);
	void untrackSprite(Sprite sprite);

	void worldToView(float xW, float yW, float &xV, float &yV);
};

