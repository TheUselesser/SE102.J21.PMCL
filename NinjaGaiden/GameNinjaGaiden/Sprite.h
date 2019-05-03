#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "Rect.h"
#include "GameTexture.h"

typedef struct {
	float X, Y;
} myPoint;

class Sprite :
	public Rect
{
	myPoint * animation;
	float posX, posY;
	float width, height;
	float velX, velY;
	int currentAnimation, lastAnimation;

	GameTexture spriteTexture;

public:
	float currentHeight;
	float maxHeight;
	bool isMoving;
	bool isJumping;

	Sprite();
	~Sprite();

	float getX();		void setX(float x);
	float getY();		void setY(float y);
	float getHeight();	void setHeight(float height);
	float getWidth();	void setWidth(float width);
	float getVelX();	void setVelX(float velX);
	float getVelY();	void setVelY(float velY);
	int getCurrentAnimation();	void setCurrentAnimation(int currentAnimation);
	int getLastAnimation();		void setLastAnimation(int lastAnimation);

	void moveLeft(); void moveRight(); void moveUp(); void moveDown();

	void LoadTexture(const char * imagePath, D3DCOLOR transColor);
	void SetAnimation(float spriteWidth, float spriteHeight, int animationCount, int animationsPerRow);

	void Draw();
};

