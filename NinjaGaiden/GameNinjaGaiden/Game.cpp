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

	groundLine = 40;	// ^_^
	float nonsense = 0;
	Camera::getInstance()->worldToView(0, groundLine, nonsense, groundLine);

	directionX = 1;

	// Game Time
	start = GetTickCount();
	countPerFrame = 1000 / FPS;
	maxHeightReached = false;

	// Load stage
	switch (stageIndex)
	{
	case 0:	// stage 3-1
		Stage.Release();
		Stage.LoadTilemap("images/Stage31/tilesheet.png", "images/Stage31/matrix.dat");
		Stage.setMapStart(0);
		Stage.setPlayerStart(8);
		//Stage.setPlayerEnd(Stage.getMapEnd());
		break;
	case 1:	// stage 3-2
		Stage.Release();
		Stage.LoadTilemap("images/Stage32/tilesheet.png", "images/Stage32/matrix.dat");
		Stage.setMapEnd(Stage.getMapEnd() - 512);
		Stage.setPlayerStart(0);
		//Stage.setPlayerEnd(224);
		break;
	case 2:	// stage 3-3
		Stage.Release();
		Stage.LoadTilemap("images/Stage33/tilesheet.png", "images/Stage33/matrix.dat");
		Stage.setMapStart(512);
		Stage.setPlayerStart(8);
		//Stage.setPlayerEnd(Stage.getMapEnd());
		break;
	}

	// Player
	_Ryu.LoadTexture("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255));
	_Ryu.SetAnimation(22, 32, 4, 4);

	_Ryu.setX(Stage.getPlayerStart());
	_Ryu.setY(groundLine - _Ryu.getHeight());
	_Ryu.setVelX(8);
	_Ryu.setVelY(16);

	_Ryu.setCurrentAnimation(0);
	_Ryu.setLastAnimation(3);
	_Ryu.isMoving = false;
	_Ryu.isJumping = false;
	_Ryu.currentHeight = 0;
	_Ryu.maxHeight = 56;	// tạm set cứng vầy để test jump

	// Phạm vi vẽ game
	placeOfTheCameraOnTheScreen.left = 0;
	placeOfTheCameraOnTheScreen.right = placeOfTheCameraOnTheScreen.left + cameraWidth;
	placeOfTheCameraOnTheScreen.top = 16;
	placeOfTheCameraOnTheScreen.bottom = placeOfTheCameraOnTheScreen.top + cameraHeight;
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

	// update mouse and keyboard
	//Poll_Mouse();
	Poll_Keyboard();

	// Bắt đầu vẽ 1 frame
	if (GetTickCount() - start >= countPerFrame)
	{
		start = GetTickCount();

		if (d3ddev->BeginScene())
		{
			d3ddev->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

			/*d3ddev->StretchRect(
				Stage[stageIndex].getBackground(),
				Camera::getInstance(),
				backbuffer,
				&placeOfTheCameraOnTheScreen,
				D3DTEXF_NONE);*/

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

			// Xử lý phím
			KeysControl();

			// Vẽ tilemap
			Stage.Draw(Camera::getInstance());

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

	// check vị trí camera
	if (Key_Down(DIK_C))
	{
		std::string message = std::to_string(Camera::getInstance()->getX());
		MessageBox(0, message.c_str(), "camera X", 0);
	}
	// check vị trí Ryu
	if (Key_Down(DIK_V))
	{
		std::string message = std::to_string(_Ryu.getX());
		MessageBox(0, message.c_str(), "Ryu X", 0);
	}

	// Xử lý Di chuyển
	// Đi qua phải
	if (Key_Down(DIK_RIGHTARROW))
	{
		_Ryu.isMoving = true;
		directionX = 1;
		if (!_Ryu.isJumping)
			_Ryu.LoadTexture("images/Ryu_right.png", D3DCOLOR_XRGB(255, 0, 255));
		Camera::getInstance()->trackSprite(_Ryu);

		// Di chuyển nhân vật khi camera chạm biên
		if (cameraX <= Stage.getMapStart() && _Ryu.getX() < (cameraWidth - _Ryu.getWidth()) / 2)
		{
			_Ryu.moveRight();
			if (_Ryu.getX() >= (cameraWidth - _Ryu.getWidth()) / 2)
				_Ryu.setX((cameraWidth - _Ryu.getWidth()) / 2);
		}
		else if (cameraX >= Stage.getMapEnd() - cameraWidth)
		{
			_Ryu.moveRight();
		}
		// Di chuyển camera
		else
		{
			Camera::getInstance()->moveRight();
		}
	}
	// Đi qua trái
	else if (Key_Down(DIK_LEFTARROW))
	{
		_Ryu.isMoving = true;
		directionX = -1;
		if (!_Ryu.isJumping)
			_Ryu.LoadTexture("images/Ryu_left.png", D3DCOLOR_XRGB(255, 0, 255));
		Camera::getInstance()->trackSprite(_Ryu);

		// Di chuyển nhân vật khi camera chạm biên
		if (cameraX <= Stage.getMapStart())
		{
			_Ryu.moveLeft();
		}
		else if (cameraX >= Stage.getMapEnd() - cameraWidth && _Ryu.getX() > (cameraWidth - _Ryu.getWidth()) / 2)
		{
			_Ryu.moveLeft();
			if (_Ryu.getX() < (cameraWidth - _Ryu.getX()) / 2)
				_Ryu.setX((cameraWidth - _Ryu.getWidth()) / 2);
		}
		// Di chuyển camera
		else
		{
			Camera::getInstance()->moveLeft();
		}
	}
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

	// Xử lý giới hạn nhân vật trong camera
	if (_Ryu.getX() < 0)
	{
		_Ryu.setX(0);
	}

	// Đến cuối map -> chuyển stage
	if (cameraX >= Stage.getMapEnd() - cameraWidth && _Ryu.getX() >= cameraWidth - _Ryu.getWidth())
	{
		if (stageIndex < numberOfStages)	// Vượt qua tất cả stage
		{
			stageIndex++;
			if (!(stageIndex < numberOfStages))	 // ^_^
				stageIndex = 0;

			Camera::getInstance()->setX(Stage.getMapStart());
			InitGame();
		}
	}

	// Xử lý giới hạn camera trong map
	if (cameraX < Stage.getMapStart())
	{
		Camera::getInstance()->setX(Stage.getMapStart());
	}
	if (cameraX > Stage.getMapEnd() - cameraWidth)
	{
		Camera::getInstance()->setX(Stage.getMapEnd() - cameraWidth);
	}
}