#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "GameWindow.h"
#include "Sprite.h"
#include "Player.h"
#include "Camera.h"
#include "Stage.h"

class Game
{
	static Game * instance;

	HWND hWnd;

	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 d3ddev = NULL;
	D3DPRESENT_PARAMETERS d3dpp;
	LPDIRECT3DSURFACE9 backbuffer = NULL;

	// Camera
	Camera camera;
	RECT placeOfTheCameraOnTheScreen;

	// Stage
	Stage Stage[3];	// tạm set mảng 3
	int stageIndex = 0;

	// 
	Player _Ryu;

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

	void init();
	void run();
	void end();



	void KeysControl();
};

