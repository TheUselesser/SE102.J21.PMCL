#include "Camera.h"
#include <d3dx9.h>


Camera * Camera::instance = NULL;
Camera * Camera::getInstance()
{
	if (instance == NULL)
	{
		instance = new Camera();
	}
	return instance;
}

Camera::Camera()
{
	// default setting ? ? ? ?
	posX = 0; posY = 192;	// điểm góc trên bên trái
	width = 256; height = 176;
}


Camera::~Camera()
{
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

void Camera::worldToView(float xW, float yW, float & xV, float & yV)
{
	// lập matrix translate
	D3DXMATRIX matrix;

	D3DXMatrixIdentity(&matrix);
	matrix._22 = -1;
	matrix._41 = -getX();		// x0
	matrix._42 = getY();		// y0

	//	1	0	0	0
	//	0	-1	0	0
	//	0	0	1	0
	//	-x0	y0	0	1	

	// transform nó
	D3DXVECTOR4 matrixP(xW, yW, 1, 1);
	D3DXVec4Transform(&matrixP, &matrixP, &matrix);

	xV = matrixP.x;
	yV = matrixP.y;
}

