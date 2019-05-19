#include "Game.h"
#include <time.h>
#include "DXInput.h"

#include <string>

#define BACKBUFFER_WIDTH 256
#define BACKBUFFER_HEIGHT 192
#define FPS 60

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

LPD3DXSPRITE Game::getSpriteHandler()
{
	return this->spriteHandler;
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
	D3DXCreateSprite(d3ddev, &spriteHandler);
}

void Game::InitGame()
{
	srand(time(NULL));

	// Game Timer
	timer.startTime = GetTickCount();
	timer.tickPerFrame = 1000 / FPS;

	// Load stage
	switch (stageIndex)
	{
	// Stage 3-1
	case 0:
		stage = new Stage();
		stage->LoadTilemap("images/Stage31/3_1_tilesheet.png", "images/Stage31/3_1_matrix.txt");
		stage->setPlayerStart(8);
		
		groundLine = 40;	// ^_^
		// add enemy
		//stage->LoadObjects("images/Stage31/object_spawn.txt");
		stage->InitGrid("images/Stage31/grid_info.txt", "images/Stage31/cells_info.txt");
		// the first enemy <3
		/*lovelyEnemy = SwordMan(200, groundLine);
		lovelyEnemy.setVelX(-2);
		lovelyEnemy.isExist = true;
		*/
		break;
	// Stage 3-2
	case 1:
		stage = new Stage();
		stage->LoadTilemap("images/Stage32/3_2_tilesheet.png", "images/Stage32/3_2_matrix.txt");
		groundLine = 40;	// ^_^
		stage->setPlayerStart(0);
		break;
	// Stage 3-3
	case 2:
		stage = new Stage();
		stage->LoadTilemap("images/Stage33/3_3_tilesheet.png", "images/Stage33/3_3_matrix.txt");
		groundLine = 28;	// ^_^
		stage->setMapStart(512);
		stage->setPlayerStart(512);
		break;
	}

	// Player Ryu
	_Ryu.InitPlayer(stage->getPlayerStart(), groundLine);
}

void Game::run()
{
	// Kiểm tra device
	if (d3ddev == NULL)
		return;

	// update keyboard
	Poll_Keyboard();

	// Bắt đầu vẽ 1 frame
	if (timer.isAtTime())
	{
		if (d3ddev->BeginScene())
		{
			// Nếu không vẽ gì thì màn hình sẽ đen thui
			d3ddev->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

			// Xử lý phím
			KeysControl();

			// Update các thông số game;
			update(1000);	// Thời gian chưa quan trọng

			// Vẽ tilemap
			stage->Draw(Camera::getInstance());

			// The First Enemy
			/*if (lovelyEnemy.isExist)
			{
				lovelyEnemy.Update(500, _Ryu);
			}*/
			// Update enemy trong stage
			stage->Update(500, _Ryu);

			// Vẽ Ryu
			_Ryu.Update(30);

			// Kết thúc 1 frame
			d3ddev->EndScene();
		}

		// Flip backbuffer lên frontbuffer
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
}

void Game::update(DWORD dt)
{
	// Xử lý nhảy cho Ryu
	if (_Ryu.isJumping)
	{
		// Tăng giảm độ cao
		_Ryu.setVelY(8 * _Ryu.directionY);		// velY cứng ngắc (temp)
		_Ryu.currentHeight += _Ryu.getVelY();

		if (_Ryu.isOnCollisionY)
		{
			_Ryu.currentHeight = 0;
			// phải tính cho việc tăng height lên lại vì height lúc nhảy bé hơn height lúc đứng 8px (noob)
			_Ryu.setY(_Ryu.getY() + 8);
		}
		else
		{
			_Ryu.setY(_Ryu.getY() + _Ryu.getVelY());
		}

		// Chạm đất
		if (_Ryu.getBottom() <= groundLine)
			_Ryu.setY(groundLine + _Ryu.getHeight());

		// Nhảy lên điểm cao nhất thì rụng xuống
		if (_Ryu.currentHeight >= _Ryu.maxHeight)
		{
			// Ghi nhận thời điểm lên đỉnh và giữ Ryu ở trên đỉnh 1 xíu
			if (!_Ryu.maxHeightReached)
			{
				_Ryu.maxHeightReached = true;
				TickAtMaxHeight = GetTickCount();

				_Ryu.directionY = 0;
			}
			if (GetTickCount() - TickAtMaxHeight >= 69)	// giữ 0.069s
			{
				_Ryu.directionY = -1;
				_Ryu.maxHeightReached = false;
			}
		}

		// chạm đất thì mọi chuyện trở lại như cũ
		if (_Ryu.currentHeight <= 0)
		{
			_Ryu.currentHeight = 0;
			_Ryu.directionY = -1;
			_Ryu.isMoving = false;
			_Ryu.isJumping = false;
			if (_Ryu.getBottom() <= groundLine) _Ryu.isFalling = false;

			_Ryu.SetStatus(PLAYER_END_JUMPING);
			_Ryu.SetStatus(PLAYER_STANDING, _Ryu.directionX);
		}
	}
	else
	{
		if (!_Ryu.isOnCollisionY && _Ryu.getBottom() > groundLine)
		{
			_Ryu.directionY = -1;
			if (_Ryu.getVelY() > 0) _Ryu.setVelY(-_Ryu.directionY);
			_Ryu.selfMovingY();
		}
	}
}

void Game::end()
{
	if (backbuffer != NULL) backbuffer->Release();
	//if (spriteHandler != NULL) spriteHandler->Release();
	if (d3ddev != NULL) d3ddev->Release();
	if (d3d != NULL) d3d->Release();
}

void Game::KeysControl()
{
	// ***** Will be deleted ********************************

		// Giữ Shift để tăng tốc
	if (Key_Down(DIK_LSHIFT) || Key_Down(DIK_RSHIFT))
	{
		_Ryu.setVelX(20 * _Ryu.directionX);
	}
	else
	{
		_Ryu.setVelX(4 * _Ryu.directionX);
	}
	// check vị trí camera
	if (Key_Down(DIK_C))
	{
		std::string message = std::to_string(Camera::getInstance()->getX());
		MessageBox(0, message.c_str(), "camera X", 0);
	}
	// check vị trí Ryu
	if (Key_Down(DIK_V))
	{
		std::string message = std::to_string(_Ryu.getX() + Camera::getInstance()->getX());
		MessageBox(0, message.c_str(), "Ryu X in world", 0);
	}

	// ******************************************************

	// Xử lý Di chuyển
	if (Key_Down(DIK_RIGHTARROW) || Key_Down(DIK_LEFTARROW))
	{
		_Ryu.directionX = Key_Down(DIK_RIGHTARROW) ? 1 : -1;

		if (_Ryu.getVelX() * _Ryu.directionX < 0)
		{
			_Ryu.setVelX(-_Ryu.getVelX());
			_Ryu.directionChanged = true;
		}

		_Ryu.isMoving = true;
		if (!_Ryu.isJumping)
		{
			_Ryu.SetStatus(PLAYER_MOVING, _Ryu.directionX);
		}
		Camera::getInstance()->trackPlayer(_Ryu);
	}
	// Không di chuyển
	else
	{
		_Ryu.isMoving = false;
		if (!_Ryu.isJumping)
		{
			_Ryu.SetStatus(PLAYER_STANDING, _Ryu.directionX);
		}
		Camera::getInstance()->freeze();
	}

	// Xử lý nhảy
	if (Key_Down(DIK_SPACE))
	{
		if (!_Ryu.isJumping && !_Ryu.isFalling)
		{
			_Ryu.isJumping = true;
			_Ryu.SetStatus(PLAYER_JUMPING, _Ryu.directionX);
			_Ryu.directionY = 1;
		}
	}

	// Nhấn ESC để thoát game
	if (Key_Down(DIK_ESCAPE))
	{
		// giải phóng bàn phím
		Kill_Keyboard();
		if (dinput != NULL) dinput->Release();

		end();
		PostMessage(hWnd, WM_DESTROY, 0, 0);
	}

	// Đến cuối map -> chuyển stage
	if (_Ryu.getRight() >= stage->getMapEnd())
	{
		if (stageIndex < NUMBER_OF_STAGES)	// Vượt qua tất cả stage
		{
			stageIndex++;
			if (!(stageIndex < NUMBER_OF_STAGES))	 // ^_^
				stageIndex = 0;

			lovelyEnemy.isExist = false;
			InitGame();
			Camera::getInstance()->setX(stage->getMapStart());
		}
	}
}

Stage * Game::getStage()
{
	return stage;
}