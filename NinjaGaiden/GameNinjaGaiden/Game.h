#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "GameWindow.h"
#include "Sprite.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "SwordMan.h"
#include "Stage.h"

#include "Collision.h"

typedef struct GameTimer
{
	DWORD startTime = 0;
	DWORD tickPerFrame = 0;

	bool isAtTime()
	{
		if (GetTickCount() - startTime >= tickPerFrame)
		{
			startTime = GetTickCount();
			return true;
		}
		return false;
	}
} Timer;

class Game
{
	static Game * instance;

	HWND hWnd;

	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 d3ddev = NULL;
	D3DPRESENT_PARAMETERS d3dpp;
	LPDIRECT3DSURFACE9 backbuffer = NULL;

	LPD3DXSPRITE spriteHandler;

	// Stage
	Stage * stage;
	int stageIndex = 0;
	const int NUMBER_OF_STAGES = 3; // làm có 3 stage thôi

	// 
	Player _Ryu;
	SwordMan lovelyEnemy;
	SwordMan secondEnemy;

	float groundLine;
	DWORD TickAtMaxHeight;

	Timer timer;

	void InitDirectX();
	void InitGame();

public:
	static Game * getInstance();

	Game();
	~Game();

	Stage * getStage();
	LPDIRECT3DDEVICE9 get3DDevice();
	LPD3DXSPRITE getSpriteHandler();

	void init();
	void run();
	void update(DWORD dt);
	void end();

	void KeysControl();
};

