#include "Sprite.h"
#include "Camera.h"


Sprite::Sprite()
{
}


Sprite::~Sprite()
{
}

int Sprite::getCurrentAnimation()
{
	return currentAnimation;
}

void Sprite::setCurrentAnimation(int currentAnimation)
{
	this->currentAnimation = currentAnimation;
}

int Sprite::getFirstAnimation()
{
	return firstAnimation;
}

void Sprite::setFirstAnimation(int firstAnimation)
{
	this->firstAnimation = firstAnimation;
}

int Sprite::getLastAnimation()
{
	return lastAnimation;
}

void Sprite::setLastAnimation(int lastAnimation)
{
	this->lastAnimation = lastAnimation;
}

void Sprite::LoadTexture(const char * imagePath, D3DCOLOR transColor)
{
	spriteTexture.LoadTexture(imagePath, transColor);
}

void Sprite::SetAnimation(float animationWidth, float animationHeight, int animationCount, int animationsPerRow, int firstAnimation, int lastAnimation)
{
	if (animationCount > 0)
	{
		//
		this->firstAnimation = firstAnimation;
		this->currentAnimation = firstAnimation;
		this->lastAnimation = lastAnimation;

		// Lưu tọa độ các animation trong sprite vào mảng animation;
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
				animation[i].Y = animation[i - 1].Y + animationHeight;
				j++;
			}

			// Tăng tọa độ X sang animation kế tiếp
			animation[i + 1].X = animation[i].X + animationWidth;
			animation[i + 1].Y = animation[i].Y;
			i++;
		}
	}
}

void Sprite::Draw(float x, float y, Rect * rect)
{
	RECT frame;
	frame.left = animation[currentAnimation].X;
	frame.top = animation[currentAnimation].Y;
	frame.right = frame.left + rect->getWidth();
	frame.bottom = frame.top + rect->getHeight();

	float xV, yV;
	Camera::getInstance()->worldToView(x, y, xV, yV);
	spriteTexture.Draw(xV, yV, &frame);
}

void Sprite::Release()
{
	spriteTexture.Release();
}