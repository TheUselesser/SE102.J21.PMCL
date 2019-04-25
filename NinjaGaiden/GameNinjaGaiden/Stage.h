#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class Stage
{
	LPDIRECT3DSURFACE9 background; // sẽ sửa thành tilemap

	float mapWidth;

public:
	Stage();
	~Stage();

	float getMapWidth();
	LPDIRECT3DSURFACE9 getBackground();

	void loadBackground(const char * imagePath, D3DCOLOR transcolor);

	void Release();
};

