#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "MovableRect.h"
#include "GameTexture.h"


typedef struct {
	float X, Y;
} myPoint;

class Sprite
{
	myPoint * animation;
	bool startAnimation;
	int currentAnimation, firstAnimation, lastAnimation;

	GameTexture spriteTexture;

public:
	Sprite();
	~Sprite();

	int getCurrentAnimation();	void setCurrentAnimation(int currentAnimation);
	int getFirstAnimation();	void setFirstAnimation(int firstAnimation);
	int getLastAnimation();		void setLastAnimation(int lastAnimation);

	void LoadTexture(const char * imagePath, D3DCOLOR transColor);
	void SetAnimation(float animationWidth, float animationHeight, int animationCount, int animationsPerRow, int firstAnimation, int lastAnimation);

	void Draw(float x, float y, Rect * rect);
	void Release();
};

