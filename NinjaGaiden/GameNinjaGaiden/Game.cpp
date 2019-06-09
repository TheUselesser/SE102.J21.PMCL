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

	// Scorebar init
	Scorebar::getInstance()->Init();

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
	//srand(time(NULL));

	// Game Timer
	timer.startTime = GetTickCount();
	timer.tickPerFrame = 1000 / FPS;

	// Load stage
	switch (stageIndex)
	{
	case 0:
		#pragma region Stage 3-1
		// Map
		Stage::getInstance()->LoadTilemap("images/Stage31/3_1_tilesheet.png", "images/Stage31/3_1_matrix.txt");
		Stage::getInstance()->LoadGroundBlocks("images/Stage31/ground_blocks.txt");
		Stage::getInstance()->setPlayerStart(8);
		groundLine = 38;
		// Camera
		Camera::getInstance()->setX(Stage::getInstance()->getMapStart());
		// Enemy
		Stage::getInstance()->InitGrid("images/Stage31/grid_info.txt", "images/Stage31/cells_info.txt");
		break;
#pragma endregion
	case 1:
		#pragma region Stage 3-2
		// Map
		Stage::getInstance()->LoadTilemap("images/Stage32/3_2_tilesheet.png", "images/Stage32/3_2_matrix.txt");
		Stage::getInstance()->LoadGroundBlocks("images/Stage32/ground_blocks.txt");
		Stage::getInstance()->setPlayerEnd(Stage::getInstance()->getMapEnd() - 16);
		groundLine = 38;
		// Camera
		Camera::getInstance()->setX(Stage::getInstance()->getMapStart());
		// Enemy
		Stage::getInstance()->InitGrid("images/Stage32/grid_info.txt", "images/Stage32/cells_info.txt");
		break;
#pragma endregion
	case 2:
		#pragma region Stage 3-3
		// Map
		Stage::getInstance()->LoadTilemap("images/Stage33/3_3_tilesheet.png", "images/Stage33/3_3_matrix.txt");
		Stage::getInstance()->LoadGroundBlocks("images/Stage33/ground_blocks.txt");
		Stage::getInstance()->setMapStart(512);
		Stage::getInstance()->setPlayerStart(544);
		groundLine = 22;
		// Camera
		Camera::getInstance()->setX(Stage::getInstance()->getMapStart());
		// Enemy
		Stage::getInstance()->InitGrid("images/Stage33/grid_info.txt", "images/Stage33/cells_info.txt");
		break;
#pragma endregion
	}

	// Player Ryu
	Player::getInstance()->InitPlayer(Stage::getInstance()->getPlayerStart(), groundLine);
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
	// Vẽ tilemap
	Stage::getInstance()->Draw(Camera::getInstance());

	// Update enemy trong stage
	Stage::getInstance()->Update(300, Player::getInstance());

	// Vẽ Ryu
	Player::getInstance()->Update(60);

	// Vẽ Scorebar
	Scorebar::getInstance()->Update();

	// Đến cuối map -> chuyển stage
	if (Player::getInstance()->getRight() >= Stage::getInstance()->getPlayerEnd()
		&& !Player::getInstance()->isJumping)
	{
		if (stageIndex < NUMBER_OF_STAGES)	// Vượt qua tất cả stage
		{
			stageIndex++;
			if (!(stageIndex < NUMBER_OF_STAGES))	 // ^_^
				stageIndex = 0;

			Stage::getInstance()->Release();
			InitGame();
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

	// [R] restart stage
	if (Key_Down(DIK_R))
	{
		if (GetTickCount() - startCooldown_R > 500)
		{
			startCooldown_R = GetTickCount();
			Stage::getInstance()->Release();
			InitGame();
		}
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
		if (stageIndex != 0)
		{
			stageIndex = 0;
			Stage::getInstance()->Release();
			InitGame();
		}
	}
	if (Key_Down(DIK_NUMPAD2))
	{
		if (stageIndex != 1)
		{
			stageIndex = 1;
			Stage::getInstance()->Release();
			InitGame();
		}
	}
	if (Key_Down(DIK_NUMPAD3))
	{
		if (stageIndex != 2)
		{
			stageIndex = 2;
			Stage::getInstance()->Release();
			InitGame();
		}
	}
	if (Key_Down(DIK_NUMPAD4))
	{
		if (stageIndex == 1)
		{
			Player::getInstance()->setX(Stage::getInstance()->getPlayerEnd() - 320);
			Player::getInstance()->setY(134);
			Camera::getInstance()->trackPlayer(Player::getInstance());
		}
	}

	// ******************************************************

	#pragma region [LEFT] [RIGHT]
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
#pragma endregion di chuyển trái phải

	#pragma region [UP] [DOWN]
	if (Key_Down(DIK_UPARROW) || Key_Down(DIK_DOWNARROW))
	{
		if (Player::getInstance()->isClimbing)
		{
			Player::getInstance()->isMoving = true;
			Player::getInstance()->directionY = Key_Down(DIK_UPARROW) ? 1 : -1;

			if (Player::getInstance()->getVelY() * Player::getInstance()->directionY < 0)
			{
				Player::getInstance()->setVelY(-Player::getInstance()->getVelY());
				Player::getInstance()->directionChanged = true;
			}

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
#pragma endregion lên xuống khi leo trèo

	#pragma region [Z]
	if (Key_Down(DIK_Z))
	{
		// Không được tấn công lúc đang leo trèo
		if (!Player::getInstance()->isClimbing)
		if (!Player::getInstance()->isAttacking)
		{
			if (Player::getInstance()->isOnGround && !Player::getInstance()->isJumping)
			{
				Player::getInstance()->SetStatus(PLAYER_ATTACK, Player::getInstance()->directionX);
			}
			else
			{
				Player::getInstance()->SetStatus(PLAYER_JUMP_ATTACK, Player::getInstance()->directionX);
			}
		}
	}
#pragma endregion tấn công

	#pragma region [Space] [X]
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
#pragma endregion nhảy

	#pragma region [C]
	if (Key_Down(DIK_C))
	{
		if (!Player::getInstance()->isThrowing)
		//if (Player::getInstance()->hasItem)
		{
			Player::getInstance()->getItem()->UseItem();
			
			Player::getInstance()->SetStatus(PLAYER_ITEM_USE, Player::getInstance()->directionX);
		}
	}
#pragma endregion dùng item

	#pragma region [ESC]
	if (Key_Down(DIK_ESCAPE))
	{
		// giải phóng bàn phím
		Kill_Keyboard();
		if (dinput != NULL) dinput->Release();

		end();
		PostMessage(hWnd, WM_DESTROY, 0, 0);
	}
#pragma endregion thoát game
}