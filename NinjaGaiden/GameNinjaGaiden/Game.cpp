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

Game * Game::instance = NULL;
Game * Game::getInstance()
{
	if (instance == NULL)
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

	// Keyboard init
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
}

void Game::InitGame()
{
	srand(time(NULL));

	groundLine = 152;
	directionX = 1;

	// init Player
	_Ryu.LoadTexture("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255));
	_Ryu.SetAnimation(22, 32, 4, 4);

	_Ryu.setX(8);
	_Ryu.setY(groundLine - _Ryu.getHeight());
	_Ryu.setVelX(8);
	_Ryu.setVelY(16);

	_Ryu.setCurrentAnimation(0);
	_Ryu.setLastAnimation(3);
	_Ryu.isMoving = false;
	_Ryu.isJumping = false;
	_Ryu.currentHeight = 0;
	_Ryu.maxHeight = 56;	// tạm set cứng vầy để test jump

	// Game Time
	start = GetTickCount();
	countPerFrame = 1000 / FPS;
	maxHeightReached = false;

	// Load stage
	switch (stageIndex)
	{
	case 0:	// stage 3-1
		Stage->Release();
		Stage[stageIndex].loadBackground("images/NinjaGaidenMapStage3-1BG.png", D3DCOLOR_XRGB(255, 0, 255));
		break;
	case 1:	// stage 3-2
		Stage->Release();
		Stage[stageIndex].loadBackground("images/NinjaGaidenMapStage3-2BG.png", D3DCOLOR_XRGB(255, 0, 255));
		break;
	case 2:	// stage 3-3
		Stage->Release();
		Stage[stageIndex].loadBackground("images/NinjaGaidenMapStage3-3BG.png", D3DCOLOR_XRGB(255, 0, 255));
		break;
	//default: 
	//	// stage đầu
	//	Stage->Release();
	//	Stage[stageIndex].loadBackground("images/NinjaGaidenMapStage3-1BG.png", D3DCOLOR_XRGB(255, 0, 255)); 
	//	break;
	}

	// Phạm vi vẽ game
	placeOfTheCameraOnTheScreen.left = 0;
	placeOfTheCameraOnTheScreen.right = placeOfTheCameraOnTheScreen.left + camera.getWidth();
	placeOfTheCameraOnTheScreen.top = 16;
	placeOfTheCameraOnTheScreen.bottom = placeOfTheCameraOnTheScreen.top + camera.getHeight();
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
			camera.left = camera.getX();
			camera.right = camera.getX() + camera.getWidth();
			camera.top = camera.getY();
			camera.bottom = camera.getY() + camera.getHeight();

			d3ddev->StretchRect(Stage[stageIndex].getBackground(), &camera, backbuffer, &placeOfTheCameraOnTheScreen, D3DTEXF_NONE);

			// Xử lý nhảy cho Ryu
			if (_Ryu.isJumping)
			{	
				_Ryu.isMoving = true;

				// Tăng giảm độ cao
				_Ryu.currentHeight += _Ryu.getVelY() * directionY;
				_Ryu.setY(_Ryu.getY() - (_Ryu.getVelY() * directionY));

				// Chạm đất
				if (_Ryu.getY() >= groundLine - _Ryu.getHeight())
					_Ryu.setY(groundLine - _Ryu.getHeight());

				// Nhảy lên điểm cao nhất thì rụng xuống
				if (_Ryu.currentHeight >= _Ryu.maxHeight)
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
				if (_Ryu.currentHeight == 0)
				{
					_Ryu.isJumping = false;
					_Ryu.isMoving = false;
					_Ryu.SetAnimation(22, 32, 4, 4);
					// Đổi lại animation đứng
					if (directionX == 1)
					{
						_Ryu.LoadTexture("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255));
					}
					if (directionX == -1)
					{
						_Ryu.LoadTexture("images/Ryu_left.png", D3DCOLOR_XRGB(255, 0, 255));
					}
				}
			}

			// Xử lý di chuyển
			KeysControl();

			// Vẽ Ryu
			_Ryu.Draw();

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
	_Ryu.Release();
	
	if (backbuffer != NULL) backbuffer->Release();

	if (d3ddev != NULL) d3ddev->Release();
	if (d3d != NULL) d3d->Release();
}

void Game::KeysControl()
{
	// Nhấn Shift để tăng tốc (Just for fun)
	if (Key_Down(DIK_LSHIFT) || Key_Down(DIK_RSHIFT))
	{
		_Ryu.setVelX(40);
	}
	else
	{
		_Ryu.setVelX(8);
	}

	// Xử lý Di chuyển
	// Đi qua phải
	if (Key_Down(DIK_RIGHTARROW))
	{
		_Ryu.isMoving = true;
		directionX = 1;
		if (!_Ryu.isJumping)
			_Ryu.LoadTexture("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255));

		// Di chuyển nhân vật khi camera chạm biên
		if (camera.getX() <= 0 && _Ryu.getX() < (camera.getWidth() - _Ryu.getWidth()) / 2)
		{
			_Ryu.moveRight();
			if (_Ryu.getX() >= (camera.getWidth() - _Ryu.getWidth()) / 2)
				_Ryu.setX((camera.getWidth() - _Ryu.getWidth()) / 2);
		}
		else if (camera.getX() >= Stage[stageIndex].getMapWidth() - camera.getWidth())
		{
			_Ryu.moveRight();
		}
		// Di chuyển camera
		else
		{
			camera.trackSprite(_Ryu);
			camera.moveRight();
		}
	}
	// Đi qua trái
	else if (Key_Down(DIK_LEFTARROW))
	{
		_Ryu.isMoving = true;
		directionX = -1;
		if (!_Ryu.isJumping)
			_Ryu.LoadTexture("images/Ryu_left.png", D3DCOLOR_XRGB(255, 0, 255));
		
		// Di chuyển nhân vật khi camera chạm biên
		if (camera.getX() <= 0)
		{
			_Ryu.moveLeft();
		}
		else if (camera.getX() >= Stage[stageIndex].getMapWidth() - camera.getWidth() && _Ryu.getX() > (camera.getWidth() - _Ryu.getWidth()) / 2)
		{

			_Ryu.moveLeft();
			if (_Ryu.getX() <= (camera.getWidth() - _Ryu.getX()) / 2)
				_Ryu.setX((camera.getWidth() - _Ryu.getWidth()) / 2);
		}
		// Di chuyển camera
		else
		{
			camera.trackSprite(_Ryu);
			camera.moveLeft();
		}
	}
	else
	{
		if (!_Ryu.isJumping)
			_Ryu.isMoving = false;
		camera.untrackSprite(_Ryu);
	}

	// Xử lý nhảy
	if (Key_Down(DIK_SPACE))
	{
		if (!_Ryu.isJumping)
		{
			directionY = 1;
			_Ryu.isJumping = true;
			_Ryu.isMoving = true;

			// Đổi animation nhảy
			if (directionX == 1)
			{
				//_Ryu.Release();
				_Ryu.LoadTexture("images/Ryu_jump_right.png", D3DCOLOR_XRGB(255, 0, 255));
			}
			if (directionX == -1)
			{
				//_Ryu.Release();
				_Ryu.LoadTexture("images/Ryu_jump_left.png", D3DCOLOR_XRGB(255, 0, 255));
			}
			_Ryu.SetAnimation(24, 24, 4, 2);
		}
	}

	if (Key_Down(DIK_L))
	{
		_Ryu.Scale(-16);
	}
	if (Key_Down(DIK_K))
	{
		_Ryu.Scale(16);
	}

	// Xử lý giới hạn nhân vật trong camera
	if (_Ryu.getX() <= 0)
	{
		_Ryu.setX(0);
	}
	
	//if (_Ryu.getX() >= camera.getWidth() - _Ryu.getWidth())
	{
		//_Ryu.setX(camera.getWidth() - _Ryu.getWidth());
	}
	// Đến cuối map -> chuyển stage
	if (camera.getX() >= Stage[stageIndex].getMapWidth() - camera.getWidth() && _Ryu.getX() >= camera.getWidth() - _Ryu.getWidth())
	{
		if (stageIndex < 3)	// làm sơ sài vì có 3 stage thôi, lý thuyết là hết stage thì làm cái finish game
		{
			stageIndex++;
			if (!(stageIndex < 3))	 // ^_^
				stageIndex = 0;

			camera.setX(0);
			InitGame();
		}
	}

	// Xử lý giới hạn camera trong map
	if (camera.getX() <= 0)
	{
		camera.setX(0);
	}
	if (camera.getX() >= Stage[stageIndex].getMapWidth() - camera.getWidth())
	{
		camera.setX(Stage[stageIndex].getMapWidth() - camera.getWidth());
	}
}