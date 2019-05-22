#pragma once
#include "Player.h"
#include "MovableRect.h"

#define	DEFAULT_CAMERA_WIDTH	256
#define	DEFAULT_CAMERA_HEIGHT	192

class Camera :
	public MovableRect
{
	static Camera * instance;

public:
	static Camera * getInstance();

	Camera();
	~Camera();

	void trackPlayer(Player player);
	void freeze();

	void worldToView(float xW, float yW, float &xV, float &yV);
};

