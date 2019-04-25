#include "Camera.h"


Camera::Camera()
{
	// default setting ? ? ? ?
	posX = 0; posY = 32;
	width = 256; height = 176;
}


Camera::~Camera()
{
}

float Camera::getX()
{
	return posX;
}

void Camera::setX(float x)
{
	this->posX = x;
}

float Camera::getY()
{
	return posY;
}

void Camera::setY(float y)
{
	this->posY = y;
}

float Camera::getWidth()
{
	return width;
}

void Camera::setWidth(float width)
{
	this->width = width;
}

float Camera::getHeight()
{
	return height;
}

void Camera::setHeight(float height)
{
	this->height = height;
}

float Camera::getVelX()
{
	return this->velX;
}

float Camera::getVelY()
{
	return this->velY;
}

void Camera::setVelX(float velX)
{
	this->velX = velX;
}

void Camera::setVelY(float velY)
{
	this->velY = velY;
}

void Camera::moveLeft()
{
	this->posX -= this->velX;
}

void Camera::moveRight()
{
	this->posX += this->velX;
}

void Camera::trackSprite(Sprite sprite)
{
	this->velX = sprite.getVelX();
}

void Camera::untrackSprite(Sprite sprite)
{
	this->velX = 0;
}
