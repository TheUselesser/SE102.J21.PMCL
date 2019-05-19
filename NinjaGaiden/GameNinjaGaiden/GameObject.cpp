#include "GameObject.h"



GameObject::GameObject()
{
	sprite = new Sprite();	// có lỗi gì thì nhớ quan tâm bạn này

	directionX = directionY = 1;
	isOnCollisionX = isOnCollisionY = false;


	timer.startTime = GetTickCount();
	timer.tickPerAnim = 0;
}


GameObject::~GameObject()
{
}

void GameObject::CreateObject(const char * imagePath, D3DCOLOR transColor, float width, float height)
{
	setSize(width, height);
	sprite->LoadTexture(imagePath, transColor);
}

void GameObject::setObjectType(GAME_OBJECT_TYPE objType)
{
	this->objType = objType;
}

GAME_OBJECT_TYPE GameObject::getObjectType()
{
	return objType;
}

void GameObject::Init()
{
}

void GameObject::SetStatus()
{
}

void GameObject::Update(DWORD dt, GameObject & player)
{
}

void GameObject::Draw()
{
	// vòng lặp animation
	if (timer.isAtTime())
	{
		if (startAnimation)
		{
			if (sprite->getCurrentAnimation() == sprite->getLastAnimation())
				sprite->setCurrentAnimation(sprite->getFirstAnimation());
			else
				sprite->setCurrentAnimation(sprite->getCurrentAnimation() + 1);
		}
		else
		{
			sprite->setCurrentAnimation(sprite->getFirstAnimation());
		}
	}

	// vẽ object
	sprite->Draw(getX(), getY(), this);
}
