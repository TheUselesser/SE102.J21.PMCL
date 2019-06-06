#include "Game.h"
#include <time.h>
#include "DXInput.h"
#include "Scorebar.h"

#include <string>

#define BACKBUFFER_WIDTH 256
#define BACKBUFFER_HEIGHT 216
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
		//MessageBox(hWnd, "Error initializing the keyboard", "Error", MB_OK);
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
		// Map
		Stage::getInstance()->LoadTilemap("images/Stage31/3_1_tilesheet.png", "images/Stage31/3_1_matrix.txt");
		Stage::getInstance()->LoadGroundBlocks("images/Stage31/ground_blocks.txt");
		Stage::getInstance()->setPlayerStart(8);
		groundLine = 38;
		// Enemy
		Stage::getInstance()->InitGrid("images/Stage31/grid_info.txt", "images/Stage31/cells_info.txt");
		break;
	// Stage 3-2
	case 1:
		// Map
		Stage::getInstance()->LoadTilemap("images/Stage32/3_2_tilesheet.png", "images/Stage32/3_2_matrix.txt");
		Stage::getInstance()->LoadGroundBlocks("images/Stage32/ground_blocks.txt");
		Stage::getInstance()->setPlayerStart(0);
		groundLine = 38;
		// Enemy
		Stage::getInstance()->InitGrid("images/Stage32/grid_info.txt", "images/Stage32/cells_info.txt");
		break;
	// Stage 3-3
	case 2:
		// Map
		Stage::getInstance()->LoadTilemap("images/Stage33/3_3_tilesheet.png", "images/Stage33/3_3_matrix.txt");
		Stage::getInstance()->LoadGroundBlocks("images/Stage33/ground_blocks.txt");
		Stage::getInstance()->setMapStart(512);
		Stage::getInstance()->setPlayerStart(528);
		groundLine = 28;
		// Enemy
		Stage::getInstance()->InitGrid("images/Stage33/grid_info.txt", "images/Stage33/cells_info.txt");
		break;
	}

	// Player Ryu
	Player::getInstance()->InitPlayer(Stage::getInstance()->getPlayerStart(), groundLine);
	// Camera
	Camera::getInstance()->setX(0);

	// Scorebar
	Scorebar::getInstance()->Init();
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
			update();

			// Kết thúc 1 frame
			d3ddev->EndScene();
		}

		// Flip backbuffer lên frontbuffer
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
}

void Game::update()
{
	// Vẽ Scorebar
	Scorebar::getInstance()->Update();

	// Vẽ tilemap
	Stage::getInstance()->Draw(Camera::getInstance());

	// Update enemy trong stage
	Stage::getInstance()->Update(300, Player::getInstance());

	// Vẽ Ryu
	Player::getInstance()->Update(60);
}

void Game::end()
{
	if (backbuffer != NULL) backbuffer->Release();
	//if (spriteHandler != NULL) spriteHandler->Release();
	if (d3ddev != NULL) d3ddev->Release();
	if (d3d != NULL) d3d->Release();
}



int count = 0;

void Game::KeysControl()
{
	// ***** Will be deleted ********************************

	// [R] restart stage
	if (Key_Down(DIK_R))
	{
		init();
	}
	if (Key_Down(DIK_B))
	{
		std::string msg = std::to_string(count);
		MessageBox(0, msg.c_str(), "checking", 0);
	}
	// [Q] to turn on invincibility
	if (Key_Down(DIK_Q))
	{
		allowHurtingPlayer = false;
	}
	// [W] to turn off invincibility
	if (Key_Down(DIK_W))
	{
		allowHurtingPlayer = true;
	}
	// [1] [2] [3] to switch between stages
	if (Key_Down(DIK_NUMPAD1))
	{
		stageIndex = 0;
		init();
	}
	if (Key_Down(DIK_NUMPAD2))
	{
		stageIndex = 1;
		init();
	}
	/*if (Key_Down(DIK_NUMPAD3))
	{
		stageIndex = 2;
		init();
	}*/

	// ******************************************************

	// [LEFT ARROW] [RIGHT ARROW] di chuyển trái phải
	if (Key_Down(DIK_RIGHTARROW) || Key_Down(DIK_LEFTARROW))
	{
		if (!Player::getInstance()->isKnockback && !Player::getInstance()->isClimbing)
		{
			Player::getInstance()->directionX = Key_Down(DIK_RIGHTARROW) ? 1 : -1;

			if (Player::getInstance()->getVelX() * Player::getInstance()->directionX < 0)
			{
				Player::getInstance()->setVelX(-Player::getInstance()->getVelX());
				Player::getInstance()->directionChanged = true;
			}

			Player::getInstance()->isMoving = true;
			if (!Player::getInstance()->isJumping && !Player::getInstance()->isAttacking)
			{
				Player::getInstance()->SetStatus(PLAYER_MOVING, Player::getInstance()->directionX);
			}
		}
	}
	// Không di chuyển
	else
	{
		if (!Player::getInstance()->isClimbing)
		{
			Player::getInstance()->isMoving = false;
			if (!Player::getInstance()->isJumping && !Player::getInstance()->isKnockback)
			{
				Player::getInstance()->SetStatus(PLAYER_STANDING, Player::getInstance()->directionX);
			}
		}
	}
	// [UP ARROW] [DOWN ARROW] trèo lên xuống
	if (Key_Down(DIK_UPARROW) || Key_Down(DIK_DOWNARROW))
	{
		if (Player::getInstance()->isClimbing)
		{
			Player::getInstance()->isMoving = true;
			Player::getInstance()->directionY = Key_Down(DIK_UPARROW) ? 1 : -1;
			Player::getInstance()->SetStatus(PLAYER_CLIMBING, Player::getInstance()->directionX);
		}
	}
	else
	{
		if (Player::getInstance()->isClimbing)
		{
			Player::getInstance()->isMoving = false;
			Player::getInstance()->SetStatus(PLAYER_CLINGING, Player::getInstance()->directionX);
		}
	}

	// [Z] tấn công
	if (Key_Down(DIK_Z))
	{
		// Không được tấn công lúc đang leo trèo
		if (!Player::getInstance()->isClimbing)
		if (!Player::getInstance()->isAttacking)
		{
			if (Player::getInstance()->isOnGround && !Player::getInstance()->isJumping)
			{
				count++;
				Player::getInstance()->SetStatus(PLAYER_ATTACK, Player::getInstance()->directionX);
			}
			else
			{
				Player::getInstance()->SetStatus(PLAYER_JUMP_ATTACK, Player::getInstance()->directionX);
			}
		}
	}

	// [Space] [X] nhảy
	if (Key_Down(DIK_SPACE) || Key_Down(DIK_X))
	{
		if (Player::getInstance()->isClimbing)
		{
			// thiết lập sau khi thoát climbing
			Player::getInstance()->isClimbing = false;
			Player::getInstance()->setMinJumpHeight(Player::getInstance()->getBottom());
			Player::getInstance()->setMaxJumpHeight(Player::getInstance()->getMinJumpHeight() + 48);
		}

		if (Player::getInstance()->isJumpable)
		{
			if (!Player::getInstance()->isAttacking && !Player::getInstance()->isKnockback)
			{
				if (!Player::getInstance()->isJumping && Player::getInstance()->isOnGround)
				{
					Player::getInstance()->directionY = 1;
					Player::getInstance()->SetStatus(PLAYER_JUMPING, Player::getInstance()->directionX);
				}
			}
		}
	}

	// [C] use item
	if (Key_Down(DIK_C))
	{
		if (Player::getInstance()->hasItem)
		{
			Player::getInstance()->getItem()->UseItem();
			
			// set animation dùng item
		}
	}


	// [ESC] close game
	if (Key_Down(DIK_ESCAPE))
	{
		// giải phóng bàn phím
		Kill_Keyboard();
		if (dinput != NULL) dinput->Release();

		end();
		PostMessage(hWnd, WM_DESTROY, 0, 0);
	}

	// Đến cuối map -> chuyển stage
	if (Player::getInstance()->getRight() >= Stage::getInstance()->getMapEnd())
	{
		if (stageIndex < NUMBER_OF_STAGES)	// Vượt qua tất cả stage
		{
			stageIndex++;
			if (!(stageIndex < NUMBER_OF_STAGES))	 // ^_^
				stageIndex = 0;

			InitGame();
		}
	}
}