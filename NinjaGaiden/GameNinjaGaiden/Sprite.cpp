#include "Sprite.h"
#include "Game.h"



Sprite::Sprite()
{
}


Sprite::~Sprite()
{
}

float Sprite::getVelX()
{
	return velX;
}

void Sprite::setVelX(float velX)
{
	this->velX = velX;
}

float Sprite::getVelY()
{
	return velY;
}

void Sprite::setVelY(float velY)
{
	this->velY = velY;
}

int Sprite::getCurrentAnimation()
{
	return currentAnimation;
}

void Sprite::setCurrentAnimation(int currentAnimation)
{
	this->currentAnimation = currentAnimation;
}

int Sprite::getLastAnimation()
{
	return lastAnimation;
}

void Sprite::setLastAnimation(int lastAnimation)
{
	this->lastAnimation = lastAnimation;
}

void Sprite::moveLeft()
{
	moveX(-velX);
}

void Sprite::moveRight()
{
	moveX(velX);
}

void Sprite::moveUp()
{
}

void Sprite::moveDown()
{
}

void Sprite::LoadTexture(const char * imagePath, D3DCOLOR transColor)
{
	spriteTexture.LoadTexture(imagePath, false, transColor);
}

void Sprite::SetAnimation(float spriteWidth, float spriteHeight, int animationCount, int animationsPerRow)
{
	if (animationCount > 0)
	{
		this->width = spriteWidth;
		this->height = spriteHeight;
		this->currentAnimation = 0;
		this->lastAnimation = animationCount - 1;

		animation = new myPoint[animationCount];
		animation[0].X = 0;
		animation[0].Y = 0;
		int i = 0;
		int j = 1;
		while (i < lastAnimation)
		{
			// Nếu đạt số lượng animation tối đa của dòng thì chuyển X về 0 và tăng Y
			if (i == animationsPerRow * j)
			{
				animation[i].X = 0;
				animation[i].Y = animation[i - 1].Y + spriteHeight;
				j++;
			}

			// Tăng tọa độ X sang animation kế tiếp
			animation[i + 1].X = animation[i].X + spriteWidth;
			animation[i + 1].Y = animation[i].Y;
			i++;
		}
	}
}

void Sprite::Draw()
{
	RECT rect;
	rect.left = animation[currentAnimation].X;
	rect.top = animation[currentAnimation].Y;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	if (isMoving)
	{
		if (isJumping)
			if (currentAnimation == lastAnimation)
				currentAnimation = 0;
			else
				currentAnimation++;
		else
		{
			if (currentAnimation == lastAnimation)
				currentAnimation = 0;
			currentAnimation++;
		}
	}
	else
	{
		currentAnimation = 0;
	}

	spriteTexture.Draw(posX, posY, &rect);
}

