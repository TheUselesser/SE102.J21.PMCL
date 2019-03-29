#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "GameWindow.h"

// Tạo sprite với vài thuộc tính cơ bản
typedef struct {
	float X, Y;
} myPoint;
typedef struct {
	myPoint position;
	myPoint animation[6];
	int width, height;
	int moveX, moveY;
	int currentFrame, lastFrame;
	int animsPerRow;

	bool isMoving;

	bool isJumping;
	float currentHeight;
	float maxHeight;
} SPRITE;

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

	LPDIRECT3DTEXTURE9 sprite_texture;
	SPRITE sprite;
	LPD3DXSPRITE sprite_handler;

	DWORD start, countPerFrame;
	RECT rect;

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

	void init();
	void run();
	void end();

	LPDIRECT3DSURFACE9 LoadSurface(const char * imagePath, D3DCOLOR transcolor);
	LPDIRECT3DTEXTURE9 LoadTexture(const char * imagePath, D3DCOLOR transcolor);

	void DiChuyenSatCanhManHinh();
	void XuLyDiChuyen();
};

