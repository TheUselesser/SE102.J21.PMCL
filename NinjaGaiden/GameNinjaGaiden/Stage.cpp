#include "Stage.h"
#include "Game.h"


Stage::Stage()
{
}


Stage::~Stage()
{
}

float Stage::getMapWidth()
{
	return mapWidth;
}

LPDIRECT3DSURFACE9 Stage::getBackground()
{
	return background;
}

void Stage::loadBackground(const char * imagePath, D3DCOLOR transcolor)
{
	D3DXIMAGE_INFO info;
	HRESULT result;

	// get width and height from bitmap file
	result = D3DXGetImageInfoFromFile(imagePath, &info);
	if (result != D3D_OK)
	{
		MessageBox(NULL, "ImageInfo Error!", "Loi kia", MB_OK);
		return;
	}

	mapWidth = info.Width;

	// create surface
	result = Game::getInstance()->get3DDevice()->CreateOffscreenPlainSurface(
		info.Width,
		info.Height,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&background,
		NULL);
	if (result != D3D_OK)
	{
		MessageBox(NULL, "Image Error!", "Loi kia", MB_OK);
		return;
	}

	result = D3DXLoadSurfaceFromFile(
		background,
		NULL,
		NULL,
		imagePath,
		NULL,
		D3DX_DEFAULT,
		transcolor,
		&info);
	if (result != D3D_OK)
	{
		MessageBox(NULL, "LoadSurface Error!", "Loi kia", MB_OK);
		return;
	}
}

void Stage::Release()
{
	if (background != NULL) background->Release();
}
