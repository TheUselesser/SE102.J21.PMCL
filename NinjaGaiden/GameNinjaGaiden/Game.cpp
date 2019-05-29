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
		stage = new Stage();
		// Map
		stage->LoadTilemap("images/Stage31/3_1_tilesheet.png", "images/Stage31/3_1_matrix.txt");
		stage->LoadGroundBlocks("images/Stage31/ground_blocks.txt");
		stage->setPlayerStart(8);
		groundLine = 38;
		// Enemy
		stage->InitGrid("images/Stage31/grid_info.txt", "images/Stage31/cells_info.txt");
		break;
	// Stage 3-2
	case 1:
		stage = new Stage();
		// Map
		stage->LoadTilemap("images/Stage32/3_2_tilesheet.png", "images/Stage32/3_2_matrix.txt");
		stage->LoadGroundBlocks("images/Stage32/ground_blocks.txt");
		stage->setPlayerStart(0);
		groundLine = 38;
		// Enemy
		stage->InitGrid("images/Stage32/grid_info.txt", "images/Stage32/cells_info.txt");
		break;
	// Stage 3-3
	case 2:
		stage = new Stage();
		// Map
		stage->LoadTilemap("images/Stage33/3_3_tilesheet.png", "images/Stage33/3_3_matrix.txt");
		stage->LoadGroundBlocks("images/Stage33/ground_blocks.txt");
		stage->setMapStart(512);
		stage->setPlayerStart(528);
		groundLine = 28;
		// Enemy
		stage->InitGrid("images/Stage33/grid_info.txt", "images/Stage33/cells_info.txt");
		break;
	}

	// Player Ryu
	Player::getInstance()->InitPlayer(stage->getPlayerStart(), groundLine);
	// Camera
	Camera::getInstance()->setX(0);
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
	stage->Draw(Camera::getInstance());

	// Update enemy trong stage
	stage->Update(300, Player::getInstance());

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

void Game::KeysControl()
{
	// ***** Will be deleted ********************************

	// Hold [Shift] to speed up
	if (Key_Down(DIK_LSHIFT) || Key_Down(DIK_RSHIFT))
	{
		Player::getInstance()->setVelX(20 * Player::getInstance()->directionX);
	}
	else
	{
		Player::getInstance()->setVelX(4 * Player::getInstance()->directionX);
	}
	// [R] restart stage
	if (Key_Down(DIK_R))
	{
		init();
	}
	if (Key_Down(DIK_B))
	{
		std::string msg = std::to_string(Player::getInstance()->isJumping) + " " + std::to_string(Player::getInstance()->isOnGround) + " " + std::to_string(Player::getInstance()->getMinJumpHeight()) + " " + std::to_string(Player::getInstance()->getMaxJumpHeight());
		MessageBox(0, msg.c_str(), "checking", 0);
	}
	// Hold [Q] to be invincible
	if (Key_Down(DIK_Q))
	{
		Player::getInstance()->isInvincible = true;
	}

	// ******************************************************

	// [LEFT ARROW] [RIGHT ARROW] di chuyển trái phải
	if (Key_Down(DIK_RIGHTARROW) || Key_Down(DIK_LEFTARROW))
	{
		if (!Player::getInstance()->isKnockback)
		{
			Player::getInstance()->directionX = Key_Down(DIK_RIGHTARROW) ? 1 : -1;

			if (Player::getInstance()->getVelX() * Player::getInstance()->directionX < 0)
			{
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
		Player::getInstance()->isMoving = false;
		if (!Player::getInstance()->isJumping && !Player::getInstance()->isKnockback)
		{
			Player::getInstance()->SetStatus(PLAYER_STANDING, Player::getInstance()->directionX);
		}
	}

	// [Z] tấn công
	if (Key_Down(DIK_Z))
	{
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

	// [Space] [X] nhảy
	if (Key_Down(DIK_SPACE) || Key_Down(DIK_X))
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
	if (Player::getInstance()->getRight() >= stage->getMapEnd())
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

Stage * Game::getStage()
{
	return stage;
}