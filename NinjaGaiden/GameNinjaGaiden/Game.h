#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "GameWindow.h"
#include "Sprite.h"
#include "Player.h"
#include "Camera.h"
#include "Stage.h"

#include "Collision.h"

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
	float cameraX, cameraY;
	float cameraWidth, cameraHeight;

	// Stage
	Stage * stage;
	int stageIndex = 0;
	const int NUMBER_OF_STAGES = 3; // làm có 3 stage thôi

	// 
	Sprite _Ryu, cloneRyu;

	float groundLine;
	bool maxHeightReached;
	DWORD TickAtMaxHeight;

	DWORD start, countPerFrame;

	void InitDirectX();
	void InitGame();

public:
	static Game * getInstance();

	Game();
	~Game();

	LPDIRECT3DDEVICE9 get3DDevice();
	LPD3DXSPRITE getSpriteHandler();

	void init();
	void run();
	void update();
	void end();

	void KeysControl();
};

