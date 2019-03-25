#pragma once
#include <windows.h>
#include <d3d9.h>

class Game
{
	static bool chayhinhsin;
	static bool restart;

	HINSTANCE instance;
	HWND hWnd;

	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 d3ddev = NULL;
	D3DPRESENT_PARAMETERS d3dpp;
	LPDIRECT3DSURFACE9 surface = NULL;
	LPDIRECT3DSURFACE9 backbuffer = NULL;

	static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	DWORD start, countPerFrame;
	RECT rect;
	int boxWidth, boxHeight;
	float x, y;
	float vx, vy;
	int directionX, directionY;
	float middleY;
	float hinhsinX;

	void InitGame();
	void InitWindow();
public:
	void init(HINSTANCE hInstance);
	void run(HINSTANCE hInstance);
	void end();

	Game();
	~Game();

	void DiChuyenSatCanhManHinh();
	void DiChuyenHinhSin();
};

