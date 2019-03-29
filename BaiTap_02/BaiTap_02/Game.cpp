#include "Game.h"
#include <time.h>
#include <math.h>
#include "DXInput.h"

#define BACKBUFFER_WIDTH 1024
#define BACKBUFFER_HEIGHT 892
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

void Game::init()
{
	// Game init
	InitGame();

	// DirectX init
	InitDirectX();

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

void Game::InitGame()
{
	srand(time(NULL));

	groundLine = 760;
	// Thiết lập thuộc tính cho sprite con mèo
	sprite.width = 92;
	sprite.height = 60;
	sprite.position.X = 0;
	sprite.position.Y = groundLine - sprite.height;
	sprite.moveX = 10;
	sprite.moveY = 25;

	sprite.currentFrame = 0;
	sprite.lastFrame = 5;
	sprite.animsPerRow = 3;
	sprite.isMoving = false;
	sprite.isJumping = false;
	sprite.currentHeight = 0;
	sprite.maxHeight = 150;

	sprite.animation[0].X = 0;
	sprite.animation[0].Y = 0;
	int i = 0;
	int j = 1;
	while (i < 5)
	{
		if (i == sprite.animsPerRow * j)
		{
			sprite.animation[i].X = 0;
			sprite.animation[i].Y = sprite.animation[i - 1].Y + sprite.height;
			j++;
		}

		sprite.animation[i + 1].X = sprite.animation[i].X + sprite.width;
		sprite.animation[i + 1].Y = sprite.animation[i].Y;
		i++;
	}

	start = GetTickCount();
	countPerFrame = 1000 / FPS;
	maxHeightReached = false;
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

	// create sprite handler
	result = D3DXCreateSprite(d3ddev, &sprite_handler);

	// backbuffer
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	
	// images
	background = LoadSurface("images/bg.bmp", D3DCOLOR_XRGB(255, 0, 255));
	sprite_texture = LoadTexture("images/kitty_right.bmp", D3DCOLOR_XRGB(0, 0, 0));
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
			//d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
			d3ddev->StretchRect(background, NULL, backbuffer, NULL, D3DTEXF_NONE);

			if (directionChanged)
			{
				if (directionX == 1)
				{
					sprite_texture = LoadTexture("images/kitty_right.bmp", D3DCOLOR_XRGB(0, 0, 0));
				}
				if (directionX == -1)
				{
					sprite_texture = LoadTexture("images/kitty_left.bmp", D3DCOLOR_XRGB(0, 0, 0));
				}
				directionChanged = false;
			}

			if (sprite.isJumping)
			{	
				sprite.currentHeight += sprite.moveY * directionY;
				sprite.position.Y -= sprite.moveY * directionY;

				// Giới hạn dưới
				if (sprite.position.Y >= groundLine - sprite.height)
					sprite.position.Y = groundLine - sprite.height;

				// Nhảy lên điểm cao nhất thì rụng xuống
				if (sprite.currentHeight >= sprite.maxHeight)
				{
					// Ghi nhận thời điểm lên đỉnh và giữ con mèo ở trên đỉnh 1 xíu
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
				if (sprite.currentHeight == 0)
				{
					sprite.isJumping = false;
				}
			}

			rect.left = sprite.animation[sprite.currentFrame].X;
			rect.top = sprite.animation[sprite.currentFrame].Y;
			rect.right = rect.left + sprite.width;
			rect.bottom = rect.top + sprite.height;

			if (sprite.isMoving)
			{
				sprite.currentFrame++;
				if (sprite.currentFrame == sprite.lastFrame)
					sprite.currentFrame = 0;
			}
			else 
			{
				sprite.currentFrame = 0;
			}

			D3DXVECTOR3 pos(sprite.position.X, sprite.position.Y, 0);

			// Bắt đầu vẽ sprite
			sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

			sprite_handler->Draw(
				sprite_texture,
				&rect,							// hiển thị phần này 
				NULL,				            // đặt tâm ở đây, NULL là góc trên bên trái
				&pos,							// hiện thị sprite lên tọa độ này trên backbuffer
				D3DCOLOR_XRGB(255, 255, 255)	// màu thay thế
			);

			// Kết thúc vẽ sprite
			sprite_handler->End();

			// tự động di chuyển
			XuLyDiChuyen();

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
	if (background != NULL) background->Release();
	if (sprite_texture != NULL) sprite_texture->Release();
	if (sprite_handler != NULL) sprite_handler->Release();
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

LPDIRECT3DTEXTURE9 Game::LoadTexture(const char * imagePath, D3DCOLOR transcolor)
{
	LPDIRECT3DTEXTURE9 texture;
	D3DXIMAGE_INFO info;
	HRESULT result;

	// get width and height from bitmap file
	result = D3DXGetImageInfoFromFile(imagePath, &info);
	if (result != D3D_OK)
		return NULL;

	result = D3DXCreateTextureFromFileEx(
		d3ddev,
		imagePath,
		info.Width,
		info.Height,
		1,
		D3DUSAGE_DYNAMIC,	      // kiểu surface
		D3DFMT_UNKNOWN,	          // định dạng surface
		D3DPOOL_DEFAULT,	      // lớp bộ nhớ cho texture
		D3DX_DEFAULT,		      // bộ lọc ảnh
		D3DX_DEFAULT,		      // bộ lọc mip
		transcolor,   // chỉ ra màu trong suốt
		&info,			          // thông tin của sprite
		NULL,			          // đổ màu
		&texture			      // texture sẽ chứa sprite
	);

	if (result != D3D_OK)
	{
		MessageBox(NULL, "Texture Error!", "Loi kia", MB_OK);
		return NULL;
	}

	return texture;
}

void Game::XuLyDiChuyen()
{
	// Nhấn Shift để tăng tốc
	if (Key_Down(DIK_LSHIFT) || Key_Down(DIK_RSHIFT))
	{
		sprite.moveX = 60;
	}
	else
	{
		sprite.moveX = 24;
	}

	// Xử lý Di chuyển
	if (Key_Down(DIK_RIGHTARROW))
	{
		sprite.position.X += sprite.moveX;
		sprite.isMoving = true;
		directionChanged = true;
		directionX = 1;
	}
	else if (Key_Down(DIK_LEFTARROW))
	{
		sprite.position.X -= sprite.moveX;
		sprite.isMoving = true;
		directionChanged = true;
		directionX = -1;
	}
	else
	{
		sprite.isMoving = false;
	}

	// Xử lý nhảy
	if (Key_Down(DIK_SPACE))
	{
		if (!sprite.isJumping)
		{
			directionY = 1;
			sprite.isJumping = true;
		}
	}


	// Xử lý giới hạn sân chơi
	if (sprite.position.X <= 0)
	{
		sprite.position.X = 0;
		//sprite.isMoving = false;
	}
	if (sprite.position.X >= BACKBUFFER_WIDTH - sprite.width)
	{
		sprite.position.X = BACKBUFFER_WIDTH - sprite.width;
		//sprite.isMoving = false;
	}
}