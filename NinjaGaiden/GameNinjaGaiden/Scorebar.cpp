#include "Scorebar.h"
#include "Game.h"
#include "Player.h"

#include <string>



int FONT_SIZE = 8;

Scorebar::Scorebar()
{
	//playerHP = 10;//cần 1 hàm lấy máu của ryu 
	score = 0000000;
	Time = 1000;
	playChance = 2;
	//1 vài biến khác nữa
}

Scorebar::~Scorebar()
{
	score = 0;
	Time = 0;
	m = "";
}

Scorebar * Scorebar::instance = NULL;
Scorebar * Scorebar::getInstance()
{
	if (instance == NULL)
	{
		instance = new Scorebar();
	}
	return instance;
}

void Scorebar::Init()
{
	gameTexture = new GameTexture();
	gameTexture->LoadTexture("images/Scorebar/tinybar.png", D3DCOLOR_XRGB(255, 255, 255));
	//load font
	FONT = NULL;
	HRESULT hr = D3DXCreateFont(
		Game::getInstance()->get3DDevice(),
		FONT_SIZE,
		0,
		FW_NORMAL,
		1,
		false,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE,
		"Ninja Gaiden (NES)",
		&FONT);
	if (!SUCCEEDED(hr))
	{
		isInit = false;
	}
}

void Scorebar::Draw(int x, int y)
{
	SetRect(Rect, 0, 0, 80, 256);
	m = score + "\n";
	m += Time + "\n";
	m += playChance + "     00";

	if (isInit == true)
		if (FONT)
			FONT->DrawTextA(NULL, m.c_str(), -1, Rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	gameTexture->Draw(x, y, Rect);
}

void Scorebar::Update()
{
	// Dùng hẳn cái switch vì thực tế có nhiều stage hơn
	switch (Game::getInstance()->getStageIndex())
	{
		// stage 3-1
	case 0:
		stage = 1;
		break;
		// stage 3-2
	case 1:
		stage = 2;
		break;
		// stage 3-3
	case 2:
		stage = 3;
		break;
	}

	// Update player stats
	score = Player::getInstance()->getScore();
	playerHP = Player::getInstance()->getHP();
	playerSpiritualStr = Player::getInstance()->getSpiritualStrength();
	playChance = Player::getInstance()->getLife();


	// Vẽ lên (0, 0) trên backbuffer
	Draw(0, 0);
}