#include "Game.h"
#include <time.h>
#include <math.h>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define BACKBUFFER_WIDTH 1920
#define BACKBUFFER_HEIGHT 1080
#define FPS 60

#define PI 3.14159265

#define WINDOW_CLASS "GameWindow"
#define WINDOW_TITLE "Game"

bool Game::chayhinhsin = false;
bool Game::restart = true;
void Game::init(HINSTANCE hInstance)
{
	// Game init
	InitGame();

	// Window init
	instance = hInstance;
	InitWindow();
}

void Game::InitGame() 
{
	restart = false;

	srand(time(NULL));

	boxWidth = boxHeight = 200;
	x = y = 0;
	middleY = BACKBUFFER_HEIGHT / 2 - boxHeight / 2;
	vx = vy = 10;

	hinhsinX = 0;

	start = GetTickCount();
	countPerFrame = 1000 / FPS;
}

void Game::InitWindow()
{
	HRESULT result;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	hWnd = CreateWindow(
		WINDOW_CLASS,
		WINDOW_TITLE,
		WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		0,	// default: CW_USEDEFAULT	
		0,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		instance,
		NULL
	);

	// d3d present parameters
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = BACKBUFFER_WIDTH;
	d3dpp.BackBufferHeight = BACKBUFFER_HEIGHT;
	d3dpp.hDeviceWindow = hWnd;

	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		MessageBox(hWnd, "Error initializing Direct3D", "Error",
			MB_OK);
		return;
	}
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		NULL,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev
	);

	// backbuffer
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	// surface
	result = d3ddev->CreateOffscreenPlainSurface(
		500, // width
		100, // height
		D3DFMT_X8R8G8B8, // surface format
		D3DPOOL_DEFAULT, // memory pool to use
		&surface, // pointer to the surface
		NULL);

	if (FAILED(result))
	{
		MessageBox(NULL, "Loi kia", "Offscreen Error!", MB_OK);
		return;
	}
}

void Game::run(HINSTANCE hInstance)
{
	if (restart) {
		InitGame();
	}

	int r, g, b;

	DWORD now = GetTickCount();

	// Kiểm tra con trỏ
	if (d3ddev == NULL)
	{
		return;
	}

	// Bắt đầu vẽ 1 frame
	if (now - start >= countPerFrame)
	{
		start = GetTickCount();

		if (d3ddev->BeginScene())
		{
			d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

			/*r = rand() % 255;
			g = rand() % 255;
			b = rand() % 255;*/
			r = 255;
			g = 222;
			b = 232;

			/*RECT surfaceTopLeft, surfaceTopRight, surfaceBottomLeft, surfaceBottomRight;

			surfaceTopLeft.left = 0;
			surfaceTopLeft.right = 250;
			surfaceTopLeft.top = 0;
			surfaceTopLeft.bottom = 50;

			surfaceTopRight.left = 250;
			surfaceTopRight.right = 500;
			surfaceTopRight.top = 0;
			surfaceTopRight.bottom = 50;

			surfaceBottomLeft.left = 0;
			surfaceBottomLeft.right = 250;
			surfaceBottomLeft.top = 50;
			surfaceBottomLeft.bottom = 100;

			surfaceBottomRight.left = 250;
			surfaceBottomRight.right = 500;
			surfaceBottomRight.top = 50;
			surfaceBottomRight.bottom = 100;

			d3ddev->ColorFill(surface, &surfaceTopLeft, D3DCOLOR_XRGB(r, g, b));
			d3ddev->ColorFill(surface, &surfaceTopRight, D3DCOLOR_XRGB(r + 55, g, b));
			d3ddev->ColorFill(surface, &surfaceBottomLeft, D3DCOLOR_XRGB(r, g + 55, b));
			d3ddev->ColorFill(surface, &surfaceBottomRight, D3DCOLOR_XRGB(r, g, b + 55));
			*/
			d3ddev->ColorFill(surface, NULL, D3DCOLOR_XRGB(r, g, b));

			rect.left = x;
			rect.right = x + boxWidth;
			rect.top = y;
			rect.bottom = y + boxHeight;
			d3ddev->StretchRect(surface, NULL, backbuffer, &rect, D3DTEXF_NONE);

			if (chayhinhsin)
			{
				DiChuyenHinhSin();
			}
			else
			{
				DiChuyenSatCanhManHinh();
			}

			// Kết thúc 1 frame
			d3ddev->EndScene();
		}

		// Flip backbuffer lên frontbuffer
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
}

void Game::end()
{
	if (d3ddev != NULL) d3ddev->Release();
	if (d3d != NULL) d3d->Release();
}

Game::Game()
{
}


Game::~Game()
{
}

LRESULT CALLBACK Game::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			Game::chayhinhsin = !Game::chayhinhsin;
			Game::restart = true;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


void Game::DiChuyenSatCanhManHinh()
{
	if (x <= 0 && y <= 0)
	{
		directionX = 1;
		directionY = 0;
	}
	if (x >= BACKBUFFER_WIDTH - boxWidth && y <= 0)
	{
		directionX = 0;
		directionY = 1;
	}
	if (x >= BACKBUFFER_WIDTH - boxWidth && y >= BACKBUFFER_HEIGHT - boxHeight)
	{
		directionX = -1;
		directionY = 0;
	}
	if (x <= 0 && y >= BACKBUFFER_HEIGHT - boxHeight)
	{
		directionX = 0;
		directionY = -1;
	}

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		vx = vy = 100;
	}
	else
	{
		vx = vy = 10;
	}

	x += vx * directionX;
	y += vy * directionY;

	if (x <= 0) x = 0;
	if (y <= 0) y = 0;
	if (x >= BACKBUFFER_WIDTH - boxWidth) x = BACKBUFFER_WIDTH - boxWidth;
	if (y >= BACKBUFFER_HEIGHT - boxHeight) y = BACKBUFFER_HEIGHT - boxHeight;
}

void Game::DiChuyenHinhSin()
{
	if (x <= 0) directionX = 1;
	if (x >= BACKBUFFER_WIDTH - boxWidth) directionX = -1;
	hinhsinX += PI/30 * directionX;

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		countPerFrame = (1000 / FPS) /100;
	}
	else
	{
		countPerFrame = 1000 / FPS;
	}

	x = hinhsinX * 192;
	y = middleY + 324 * sin(hinhsinX);


	if (x <= 0) x = 0;
	if (y <= 0) y = 0;
	if (x >= BACKBUFFER_WIDTH - boxWidth) x = BACKBUFFER_WIDTH - boxWidth;
	if (y >= BACKBUFFER_HEIGHT - boxHeight) y = BACKBUFFER_HEIGHT - boxHeight;
}