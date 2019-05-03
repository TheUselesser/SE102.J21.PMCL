#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "GameWindow.h"
#include "Sprite.h"
#include "Player.h"
#include "Camera.h"
#include "Stage.h"
#include "Tilemap.h"

class Game
{
	static Game * instance;

	HWND hWnd;

	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 d3ddev = NULL;
	D3DPRESENT_PARAMETERS d3dpp;
	LPDIRECT3DSURFACE9 backbuffer = NULL;

	LPD3DXSPRITE spriteHandler;

	// Camera
	float cameraX, cameraY, cameraWidth, cameraHeight;
	RECT placeOfTheCameraOnTheScreen;

	// Stage
	Stage Stage;
	int stageIndex = 0;
	int numberOfStages = 3; // set cứng bằng 3 luôn

	// 
	Sprite _Ryu;

	float groundLine;
	int directionX, directionY;

	bool maxHeightReached;
	DWORD TickAtMaxHeight;

	DWORD start, countPerFrame;

	void InitGame();
	void InitDirectX();

public:
	static Game * getInstance();
	Game();
	~Game();
	LPDIRECT3DDEVICE9 get3DDevice();
	LPD3DXSPRITE getSpriteHandler();

	void init();
	void run();
	void end();



	void KeysControl();
};

