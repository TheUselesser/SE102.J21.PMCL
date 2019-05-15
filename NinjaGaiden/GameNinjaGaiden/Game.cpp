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

	// Game Time
	start = GetTickCount();
	countPerFrame = 900 / FPS;
	maxHeightReached = false;

	// Load stage
	switch (stageIndex)
	{
	case 0:	// stage 3-1
		stage = new Stage();
		stage->LoadTilemap("images/Stage31/3_1_tilesheet.png", "images/Stage31/3_1_matrix.txt");
		stage->setPlayerStart(8);
		//stage->setPlayerEnd(stage->getMapEnd());
		break;
	case 1:	// stage 3-2
		stage = new Stage();
		stage->LoadTilemap("images/Stage32/3_2_tilesheet.png", "images/Stage32/3_2_matrix.txt");
		//stage->setMapEnd(stage->getMapEnd());
		stage->setPlayerStart(0);
		//stage->setPlayerEnd(224);
		//Camera::getInstance()->setY(Camera::getInstance()->getY() - 16);
		break;
	case 2:	// stage 3-3
		stage = new Stage();
		stage->LoadTilemap("images/Stage33/3_3_tilesheet.png", "images/Stage33/3_3_matrix.txt");
		stage->setMapStart(512);
		stage->setPlayerStart(0);
		//stage->setPlayerEnd(stage->getMapEnd());
		break;
	}

	groundLine = 40;	// ^_^

	// Player Ryu
	_Ryu.LoadTexture("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255));
	_Ryu.SetAnimation(22, 32, 4, 4);

	_Ryu.setX(stage->getPlayerStart());
	_Ryu.setY(groundLine + _Ryu.getHeight());
	_Ryu.setVelX(4);
	_Ryu.setVelY(8);

	_Ryu.setCurrentAnimation(0);
	_Ryu.setLastAnimation(3);
	_Ryu.isMoving = false;
	_Ryu.isJumping = false;
	_Ryu.currentHeight = 0;
	_Ryu.maxHeight = 48;	// tạm set cứng vầy để test jump

	// clone Ryu
	cloneRyu.LoadTexture("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255));
	cloneRyu.SetAnimation(22, 32, 1, 1);
	cloneRyu.setX(200);
	cloneRyu.setY(_Ryu.getY());
}

void Game::run()
{
	// Kiểm tra device
	if (d3ddev == NULL)
		return;

	// Lay thong so camera
	cameraX = Camera::getInstance()->getX();
	cameraY = Camera::getInstance()->getY();
	cameraWidth = Camera::getInstance()->getWidth();
	cameraHeight = Camera::getInstance()->getHeight();

	// update keyboard
	Poll_Keyboard();

	// Bắt đầu vẽ 1 frame
	if (GetTickCount() - start >= countPerFrame)
	{
		start = GetTickCount();

		if (d3ddev->BeginScene())
		{
			// Nếu không vẽ gì thì màn hình sẽ đen thui
			d3ddev->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

			// Xử lý phím
			KeysControl();

			// Update các thông số game;
			update();

			// Vẽ tilemap
			stage->Draw(Camera::getInstance());

			// clone Ryu
			cloneRyu.Draw();

			Collision::CollisionHandle(_Ryu, cloneRyu);

			// Vẽ Ryu
			_Ryu.Draw();

			// Kết thúc 1 frame
			d3ddev->EndScene();
		}

		// Flip backbuffer lên frontbuffer
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
}

void Game::update()
{
	// Xử lý nhảy cho Ryu
	if (_Ryu.isJumping)
	{
		_Ryu.isMoving = true;

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
			if (!maxHeightReached)
			{
				maxHeightReached = true;
				TickAtMaxHeight = GetTickCount();

				_Ryu.directionY = 0;
			}
			if (GetTickCount() - TickAtMaxHeight >= 69)	// giữ 0.069s
			{
				_Ryu.directionY = -1;
				maxHeightReached = false;
			}
		}

		// chạm đất thì mọi chuyện trở lại như cũ
		if (_Ryu.currentHeight <= 0)
		{
			_Ryu.currentHeight = 0;
			_Ryu.directionY = -1;
			_Ryu.isJumping = false;
			_Ryu.isMoving = false;
			_Ryu.SetAnimation(22, 32, 4, 4);
			// Đổi lại animation đứng
			if (_Ryu.directionX == 1)
			{
				_Ryu.LoadTexture("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255));
			}
			if (_Ryu.directionX == -1)
			{
				_Ryu.LoadTexture("images/Ryu_left.png", D3DCOLOR_XRGB(255, 0, 255));
			}
		}
	}
	else
	{
		if (!_Ryu.isOnCollisionY && _Ryu.getBottom() > groundLine)
		{
			_Ryu.directionY = -1;
			if (_Ryu.getVelY() > 0) _Ryu.setVelY(_Ryu.getVelY() * _Ryu.directionY);
			_Ryu.moveY(_Ryu.getVelY());
		}
	}
}

void Game::end()
{
	if (backbuffer != NULL) backbuffer->Release();

	if (d3ddev != NULL) d3ddev->Release();
	if (d3d != NULL) d3d->Release();
}

void Game::KeysControl()
{
// ***** Will be deleted ********************************

	// Giữ Shift để tăng tốc
	if (Key_Down(DIK_LSHIFT) || Key_Down(DIK_RSHIFT))
	{
		_Ryu.setVelX(40 * _Ryu.directionX);
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
	// check vị trí Ryu (camera)
	if (Key_Down(DIK_V))
	{
		std::string message = std::to_string(_Ryu.getX());
		MessageBox(0, message.c_str(), "Ryu X in view", 0);
	}
	// check vị trí Ryu (world)
	if (Key_Down(DIK_B))
	{
		std::string message = std::to_string(_Ryu.getX() + Camera::getInstance()->getX());
		MessageBox(0, message.c_str(), "Ryu X in world", 0);
	}
	if (Key_Down(DIK_N))
	{
		std::string message = std::to_string(Collision::CollisionTime);
		MessageBox(0, message.c_str(), "collision time", 0);
		message = std::to_string(Collision::EntryTime);
		MessageBox(0, message.c_str(), "entry time", 0);
		message = std::to_string(Collision::ExitTime);
		MessageBox(0, message.c_str(), "exit time", 0);
		message = std::to_string(_Ryu.getVelY());
		MessageBox(0, message.c_str(), "Ryu vy", 0);
		message = std::to_string(_Ryu.getBottom());
		MessageBox(0, message.c_str(), "Ryu bottom", 0);
		message = std::to_string(cloneRyu.getTop());
		MessageBox(0, message.c_str(), "cloneRyu top", 0);
	}

// ******************************************************

	// Xử lý Di chuyển
	// Đi qua phải
	if (Key_Down(DIK_RIGHTARROW))
	{
		_Ryu.isMoving = true;
		_Ryu.directionX = 1;
		if (_Ryu.getVelX() < 0) _Ryu.setVelX(_Ryu.directionX * _Ryu.getVelX());
		if (!_Ryu.isJumping)
			_Ryu.LoadTexture("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255));

		if (!_Ryu.isOnCollisionX)	// <--- đây
		{
			// Di chuyển nhân vật khi camera chạm biên
			if (cameraX <= stage->getMapStart() && _Ryu.getX() < (cameraWidth - _Ryu.getWidth()) / 2)
			{
				_Ryu.moveRight();
				if (_Ryu.getX() >= (cameraWidth - _Ryu.getWidth()) / 2)
					_Ryu.setX((cameraWidth - _Ryu.getWidth()) / 2);
			}
			else if (cameraX >= stage->getMapEnd() - cameraWidth)
			{
				_Ryu.moveRight();
			}
			// Di chuyển camera
			else
			{
				_Ryu.moveRight();
				Camera::getInstance()->moveX(_Ryu.getVelX());
			}
		}
	}
	// Đi qua trái
	else if (Key_Down(DIK_LEFTARROW))
	{
		_Ryu.isMoving = true;
		_Ryu.directionX = -1;
		if (_Ryu.getVelX() > 0) _Ryu.setVelX(_Ryu.directionX * _Ryu.getVelX());
		if (!_Ryu.isJumping)
			_Ryu.LoadTexture("images/Ryu_left.png", D3DCOLOR_XRGB(255, 0, 255));

		if (!_Ryu.isOnCollisionX)	// <--- đây
		{
			// Di chuyển nhân vật khi camera chạm biên
			if (cameraX <= stage->getMapStart())
			{
				_Ryu.moveLeft();
			}
			else if (cameraX >= stage->getMapEnd() - cameraWidth &&
				_Ryu.getX() > (cameraWidth - _Ryu.getWidth()) / 2)
			{
				_Ryu.moveLeft();
				if (_Ryu.getX() < (cameraWidth - _Ryu.getX()) / 2)
					_Ryu.setX((cameraWidth - _Ryu.getWidth()) / 2);
			}
			// Di chuyển camera
			else
			{
				_Ryu.moveLeft();
				Camera::getInstance()->moveX(_Ryu.getVelX());
			}
		}
	}
	// Không di chuyển
	else
	{
		if (!_Ryu.isJumping)
			_Ryu.isMoving = false;
		Camera::getInstance()->untrackSprite(_Ryu);
	}

	// Xử lý nhảy
	if (Key_Down(DIK_SPACE))
	{
		if (!_Ryu.isJumping)
		{
			_Ryu.directionY = 1;
			_Ryu.isJumping = true;
			_Ryu.isMoving = true;

			// Đổi animation nhảy
			if (_Ryu.directionX == 1)
			{
				//_Ryu.Release();
				_Ryu.LoadTexture("images/Ryu_jump_right.png", D3DCOLOR_XRGB(255, 0, 255));
			}
			if (_Ryu.directionX == -1)
			{
				//_Ryu.Release();
				_Ryu.LoadTexture("images/Ryu_jump_left.png", D3DCOLOR_XRGB(255, 0, 255));
			}
			_Ryu.SetAnimation(24, 24, 4, 2);
		}
	}

// quy định giới hạn nhân vật trong camera
	if (_Ryu.getX() < 0)
	{
		_Ryu.setX(0);
	}
	// Đến cuối map -> chuyển stage
	if (cameraX >= stage->getMapEnd() - cameraWidth && _Ryu.getX() >= cameraWidth - _Ryu.getWidth())
	{
		if (stageIndex < NUMBER_OF_STAGES)	// Vượt qua tất cả stage
		{
			stageIndex++;
			if (!(stageIndex < NUMBER_OF_STAGES))	 // ^_^
				stageIndex = 0;

			InitGame();
			Camera::getInstance()->setX(stage->getMapStart());
		}
	}

// quy định giới hạn camera trong map
	if (cameraX < stage->getMapStart())
	{
		Camera::getInstance()->setX(stage->getMapStart());
	}
	if (cameraX > stage->getMapEnd() - cameraWidth)
	{
		Camera::getInstance()->setX(stage->getMapEnd() - cameraWidth);
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
}