#include "Sprite.h"
#include "Game.h"



Sprite::Sprite()
{
}


Sprite::~Sprite()
{
}

float Sprite::getX()
{
	return posX;
}

void Sprite::setX(float X)
{
	this->posX = X;
}

float Sprite::getY()
{
	return posY;
}

void Sprite::setY(float Y)
{
	this->posY = Y;
}

float Sprite::getWidth()
{
	return width;
}

void Sprite::setWidth(float width)
{
	this->width = width;
}

float Sprite::getHeight()
{
	return height;
}

void Sprite::setHeight(float height)
{
	this->height = height;
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
	posX -= velX;
}

void Sprite::moveRight()
{
	posX += velX;
}

void Sprite::moveUp()
{
}

void Sprite::moveDown()
{
}

void Sprite::LoadTexture(const char * imagePath, D3DCOLOR transcolor)
{
	this->Release();
	this->d3ddev = Game::getInstance()->get3DDevice();

	D3DXIMAGE_INFO info;
	HRESULT result;

	result = D3DXGetImageInfoFromFile(imagePath, &info);
	if (result != D3D_OK)
		return;

	result = D3DXCreateTextureFromFileEx(
		d3ddev,
		imagePath,
		info.Width,
		info.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,		      // bộ lọc ảnh
		D3DX_DEFAULT,		      // bộ lọc mip
		transcolor,				// chỉ ra màu trong suốt
		&info,			          // thông tin của sprite
		NULL,			          // đổ màu
		&spriteTexture			      // texture sẽ chứa sprite
	);

	if (result != D3D_OK)
	{
		MessageBox(NULL, "Texture Error!", "Loi kia", MB_OK);
		return;
	}
	D3DXCreateSprite(d3ddev, &spriteHandler);
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

void Sprite::Scale(float scale)
{
	/*
	*	Đang lỗi
	*/

	//width *= scale;
	//height *= scale;

	D3DXMATRIX matScale;
	//D3DXMatrixScaling(&matScale, 1, scale, 1);
	//D3DXMatrixTranslation(&matScale, scale, scale, 0);
	D3DXMatrixRotationX(&matScale, D3DXToRadian(180));

	spriteHandler->SetTransform(&matScale);
}

void Sprite::Flip()
{
	/*
	*	Đang lỗi
	*/

	
}

void Sprite::Draw()
{
	HRESULT result;
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

	D3DXVECTOR3 pos(posX, posY, 0);

	// Bắt đầu vẽ sprite
	spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

	result = spriteHandler->Draw(
		spriteTexture,
		&rect,							// hiển thị phần này 
		NULL,				            // đặt tâm ở đây, NULL là góc trên bên trái
		&pos,							// hiện thị sprite lên tọa độ này trên backbuffer
		D3DCOLOR_XRGB(255, 255, 255)	// màu thay thế
	);
	if (result != D3D_OK)
	{
		MessageBox(NULL, "Texture Error!", "Loi kia", MB_OK);
		spriteHandler->End();
		return;
	}

	// Kết thúc vẽ sprite
	spriteHandler->End();
}

void Sprite::Release()
{
	if (spriteTexture != NULL)	spriteTexture->Release();
	if (spriteHandler != NULL)	spriteHandler->Release();
}


