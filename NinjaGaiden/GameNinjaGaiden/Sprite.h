#pragma once
#include <d3d9.h>
#include <d3dx9.h>

typedef struct {
	float X, Y;
} myPoint;

class Sprite
{
	myPoint * animation;
	float posX, posY;
	float width, height;
	float velX, velY;
	int currentAnimation, lastAnimation;

	LPDIRECT3DDEVICE9 d3ddev;
	LPDIRECT3DTEXTURE9 spriteTexture;
	LPD3DXSPRITE spriteHandler;

public:
	float currentHeight;
	float maxHeight;
	bool isMoving;
	bool isJumping;

	Sprite();
	~Sprite();

	float getX();		void setX(float X);
	float getY();		void setY(float Y);
	float getWidth();	void setWidth(float width);
	float getHeight();	void setHeight(float height);
	float getVelX();	void setVelX(float velX);
	float getVelY();	void setVelY(float velY);
	int getCurrentAnimation();	void setCurrentAnimation(int currentAnimation);
	int getLastAnimation();		void setLastAnimation(int lastAnimation);

	void moveLeft(); void moveRight(); void moveUp(); void moveDown();

	void LoadTexture(const char * imagePath, D3DCOLOR transcolor);
	void SetAnimation(float spriteWidth, float spriteHeight, int animationCount, int animationsPerRow);

	void Draw();

	void Release();

	// . . .
	void Scale(float scale);
	void Flip();
};

