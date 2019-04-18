#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "GameWindow.h"
#include "Sprite.h"


class Game
{
	static Game * instance;

	//HINSTANCE hInstance;
	HWND hWnd;

	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 d3ddev = NULL;
	D3DPRESENT_PARAMETERS d3dpp;
	LPDIRECT3DSURFACE9 background = NULL;
	LPDIRECT3DSURFACE9 backbuffer = NULL;

	RECT camera;
	float cameraX, cameraY, cameraWidth, cameraHeight;
	RECT bbbbb;
	float mapWidth;
	Sprite sprite_Ryu;

	DWORD start, countPerFrame;

	float groundLine;
	int directionX, directionY;
	bool directionChanged;

	bool maxHeightReached;
	DWORD TickAtMaxHeight;

	void InitGame();
	void InitDirectX();

public:
	static Game * getInstance();
	Game();
	~Game();
	LPDIRECT3DDEVICE9 get3DDevice();

	void init();
	void run();
	void end();

	LPDIRECT3DSURFACE9 LoadSurface(const char * imagePath, D3DCOLOR transcolor);
	LPDIRECT3DTEXTURE9 LoadTexture(const char * imagePath, D3DCOLOR transcolor);


	void DiChuyenSatCanhManHinh();
	void XuLyDiChuyen();
};

