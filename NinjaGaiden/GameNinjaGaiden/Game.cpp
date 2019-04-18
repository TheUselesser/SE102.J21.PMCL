#include "Game.h"
#include <time.h>
#include <math.h>
#include "DXInput.h"

#define BACKBUFFER_WIDTH 256
#define BACKBUFFER_HEIGHT 192
#define FPS 60

#define PI 3.14159265

#define WINDOW_CLASS "GameWindow"
#define WINDOW_TITLE "Game"

Game * Game::instance = 0;
Game * Game::getInstance()
{
	if (instance == 0)
	{
		instance = new Game();
	}
	return instance;
}

Game::Game()
{
	this->hWnd = GameWindow::getInstance()->getHandleWindow();
}


Game::~Game()
{
}

LPDIRECT3DDEVICE9 Game::get3DDevice()
{
	return this->d3ddev;
}

void Game::init()
{
	// DirectX init
	InitDirectX();

	// Game init
	InitGame();

	// DirectInput
	//initialize mouse
	/*if (!Init_Mouse(hWnd))
	{
		MessageBox(hWnd, "Error initializing the mouse", "Error", MB_OK);
			return;
	}*/

	//initialize keyboard
	if (!Init_Keyboard(hWnd))
	{
		MessageBox(hWnd, "Error initializing the keyboard", "Error", MB_OK);
			return;
	}
}

void Game::InitDirectX()
{
	HRESULT result;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

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

	// images
	background = LoadSurface("images/NinjaGaidenMapStage3-1BG.png", D3DCOLOR_XRGB(255, 0, 255));

	sprite_Ryu.LoadTexture("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255));
	sprite_Ryu.SetAnimation(22, 32, 4, 4);
}

void Game::InitGame()
{
	srand(time(NULL));

	groundLine = 152;
	directionX = 1;

	sprite_Ryu.setX(0);
	sprite_Ryu.setY(groundLine - sprite_Ryu.getHeight());
	sprite_Ryu.setVelX(10);
	sprite_Ryu.setVelY(25);

	sprite_Ryu.setCurrentAnimation(0);
	sprite_Ryu.setLastAnimation(3);
	sprite_Ryu.isMoving = false;
	sprite_Ryu.isJumping = false;
	sprite_Ryu.currentHeight = 0;
	sprite_Ryu.maxHeight = 85;

	start = GetTickCount();
	countPerFrame = 1000 / FPS;
	maxHeightReached = false;

	// camera tạm thời
	cameraX = 0;
	cameraY = 32;
	cameraWidth = 256;
	cameraHeight = 176;

	bbbbb.left = 0;
	bbbbb.right = bbbbb.left + cameraWidth;
	bbbbb.top = 16;
	bbbbb.bottom = bbbbb.top + cameraHeight;
}

void Game::run()
{
	// Kiểm tra device
	if (d3ddev == NULL)
		return;

	// update mouse and keyboard
	//Poll_Mouse();
	Poll_Keyboard();

	// Bắt đầu vẽ 1 frame
	if (GetTickCount() - start >= countPerFrame)
	{
		start = GetTickCount();

		if (d3ddev->BeginScene())
		{
			camera.left = cameraX;
			camera.right = cameraX + cameraWidth;
			camera.top = cameraY;
			camera.bottom = cameraY + cameraHeight;

			d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
			d3ddev->StretchRect(background, &camera, backbuffer, &bbbbb, D3DTEXF_NONE);

			// Xử lý nhảy cho Ryu
			if (sprite_Ryu.isJumping)
			{	
				sprite_Ryu.isMoving = true;
				// Đổi animation nhảy
				if (directionX == 1)
				{
					sprite_Ryu.LoadTexture("images/Ryu_jump_right.png", D3DCOLOR_XRGB(255, 0, 255));
				}
				if (directionX == -1)
				{
					sprite_Ryu.LoadTexture("images/Ryu_jump_left.png", D3DCOLOR_XRGB(255, 0, 255));
				}

				// Tăng giảm độ cao
				sprite_Ryu.currentHeight += sprite_Ryu.getVelY() * directionY;
				sprite_Ryu.setY(sprite_Ryu.getY() - (sprite_Ryu.getVelY() * directionY));

				// Giới hạn dưới
				if (sprite_Ryu.getY() >= groundLine - sprite_Ryu.getHeight())
					sprite_Ryu.setY(groundLine - sprite_Ryu.getHeight());

				// Nhảy lên điểm cao nhất thì rụng xuống
				if (sprite_Ryu.currentHeight >= sprite_Ryu.maxHeight)
				{
					// Ghi nhận thời điểm lên đỉnh và giữ Ryu ở trên đỉnh 1 xíu
					if (!maxHeightReached)
					{
						maxHeightReached = true;
						TickAtMaxHeight = GetTickCount();
						
						directionY = 0;
					}
					if (GetTickCount() - TickAtMaxHeight >= 69)	// giữ 0.069s
					{
						directionY = -1;
						maxHeightReached = false;
					}
				}

				// chạm đất thì mọi chuyện trở lại như cũ
				if (sprite_Ryu.currentHeight == 0)
				{
					sprite_Ryu.isJumping = false;
					sprite_Ryu.isMoving = false;
					sprite_Ryu.SetAnimation(22, 32, 4, 4);
					// Đổi lại animation đứng
					if (directionX == 1)
					{
						sprite_Ryu.LoadTexture("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255));
					}
					if (directionX == -1)
					{
						sprite_Ryu.LoadTexture("images/Ryu_left.png", D3DCOLOR_XRGB(255, 0, 255));
					}
				}
			}

			// Xử lý di chuyển
			XuLyDiChuyen();

			// Vẽ Ryu
			sprite_Ryu.Draw();

			// Kết thúc 1 frame
			d3ddev->EndScene();
		}

		// Flip backbuffer lên frontbuffer
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	// Nhấn ESC để thoát
	if (Key_Down(DIK_ESCAPE))
	{
		// giải phóng đối tượng input
		Kill_Keyboard();
		//Kill_Mouse();
		if (dinput != NULL) dinput->Release();
		
		end();
		PostMessage(hWnd, WM_DESTROY, 0, 0);
	}
}

void Game::end()
{
	sprite_Ryu.Release();
	if (background != NULL) background->Release();
	if (backbuffer != NULL) backbuffer->Release();

	if (d3ddev != NULL) d3ddev->Release();
	if (d3d != NULL) d3d->Release();
}

LPDIRECT3DSURFACE9 Game::LoadSurface(const char * imagePath, D3DCOLOR transcolor)
{
	LPDIRECT3DSURFACE9 image = NULL;
	D3DXIMAGE_INFO info;
	HRESULT result;

	// get width and height from bitmap file
	result = D3DXGetImageInfoFromFile(imagePath, &info);
	if (FAILED(result))
		return NULL;

	mapWidth = info.Width;

	// create surface
	result = d3ddev->CreateOffscreenPlainSurface(
		info.Width,
		info.Height,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&image,
		NULL);
	if (result != D3D_OK)
	{
		MessageBox(NULL, "Image Error!", "Loi kia", MB_OK);
		return NULL;
	}

	result = D3DXLoadSurfaceFromFile(
		image,
		NULL,
		NULL,
		imagePath,
		NULL,
		D3DX_DEFAULT,
		transcolor,
		&info);
	if (result != D3D_OK)
	{
		MessageBox(NULL, "Surface Error!", "Loi kia", MB_OK);
		return NULL;
	}

	return image;
}

void Game::XuLyDiChuyen()
{
	// Nhấn Shift để tăng tốc
	if (Key_Down(DIK_LSHIFT) || Key_Down(DIK_RSHIFT))
	{
		sprite_Ryu.setVelX(40);
	}
	else
	{
		sprite_Ryu.setVelX(8);
	}

	// Xử lý Di chuyển
	// Đi qua phải
	if (Key_Down(DIK_RIGHTARROW))
	{
		sprite_Ryu.isMoving = true;
		directionX = 1;
		if (!sprite_Ryu.isJumping)
			sprite_Ryu.LoadTexture("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255));

		// Di chuyển nhân vật khi camera chạm biên
		if (cameraX <= 0 && sprite_Ryu.getX() < (cameraWidth - sprite_Ryu.getWidth()) / 2)
		{
			sprite_Ryu.moveRight();
			if (sprite_Ryu.getX() >= (cameraWidth - sprite_Ryu.getWidth()) / 2)
				sprite_Ryu.setX((cameraWidth - sprite_Ryu.getWidth()) / 2);
		}
		else if (cameraX >= mapWidth - cameraWidth)
		{
			sprite_Ryu.moveRight();
		}
		// Di chuyển camera
		else
		{
			cameraX += sprite_Ryu.getVelX();
		}
	}
	// Đi qua trái
	else if (Key_Down(DIK_LEFTARROW))
	{
		sprite_Ryu.isMoving = true;
		directionX = -1;
		if (!sprite_Ryu.isJumping)
			sprite_Ryu.LoadTexture("images/Ryu_left.png", D3DCOLOR_XRGB(255, 0, 255));
		
		// Di chuyển nhân vật khi camera chạm biên
		if (cameraX <= 0)
		{
			sprite_Ryu.moveLeft();
		}
		else if (cameraX >= mapWidth - cameraWidth && sprite_Ryu.getX() > (cameraWidth - sprite_Ryu.getWidth()) / 2)
		{
			sprite_Ryu.moveLeft();
			if (sprite_Ryu.getX() <= (cameraWidth - sprite_Ryu.getX()) / 2)
				sprite_Ryu.setX((cameraWidth - sprite_Ryu.getWidth()) / 2);
		}
		// Di chuyển camera
		else
		{
			cameraX -= sprite_Ryu.getVelX();
		}
	}
	else
	{
		if (!sprite_Ryu.isJumping)
			sprite_Ryu.isMoving = false;
	}

	// Xử lý nhảy
	if (Key_Down(DIK_SPACE))
	{
		if (!sprite_Ryu.isJumping)
		{
			directionY = 1;
			sprite_Ryu.isJumping = true;
			sprite_Ryu.isMoving = true;
			sprite_Ryu.SetAnimation(24, 24, 4, 2);
		}
	}

	// Xử lý giới hạn nhân vật trong camera
	if (sprite_Ryu.getX() <= 0)
	{
		sprite_Ryu.setX(0);
	}
	if (sprite_Ryu.getX() >= cameraWidth - sprite_Ryu.getWidth())
	{
		sprite_Ryu.setX(cameraWidth - sprite_Ryu.getWidth());
	}
	// Xử lý giới hạn camera trong map
	if (cameraX <= 0)
	{
		cameraX = 0;
	}
	if (cameraX >= mapWidth - cameraWidth)
	{
		cameraX = mapWidth - cameraWidth;
	}
}