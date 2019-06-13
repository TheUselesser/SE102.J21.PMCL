#include "Scorebar.h"
#include <string>



int FONT_SIZE = 12;

Scorebar::Scorebar()
{
	player->setHP(1);
	playChance = player->getLife();
	playerHealth = player->getHP();

	//1 vài biến khác nữa
	time = 300;
	gameTexture = new GameTexture();
}

Scorebar::~Scorebar()
{
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

bool Scorebar::Init()
{
	this->Hpbar = 5;

	FONT = NULL;
	AddFontResourceEx("", FR_PRIVATE, NULL);
	HRESULT result = D3DXCreateFont(
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
		"Init Text",
		&FONT);

	if (!SUCCEEDED(result))
		return false;


	m = "SCORE \n";
	m += "TiME                =62\n";
	m += "LIFE                P=3\n";

	return true;
}

void Scorebar::Draw(int x, int y)
{
	RECT newRect;
	SetRect(&newRect, x, y, 256, 100);
	//vẽ text
	if (FONT)
		FONT->DrawTextA(NULL, m.c_str(), -1, &newRect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	// 2 thanh empty hp cho boss + ryu
	RECT box;
	SetRect(&box, 0, 0, 4, 8);
	gameTexture->LoadTexture("images/GameUI/empty_hp.png");
	for (int i = 0; i < Hpbar * 20; i += 5)
		gameTexture->Draw(135 + i, 14, &box);
	gameTexture->LoadTexture("images/GameUI/empty_hp.png");
	for (int i = 0; i < Hpbar * 20; i += 5)
		gameTexture->Draw(135 + i, 26, &box);
	// Thanh hp cho ryu
	gameTexture->LoadTexture("images/GameUI/hp.png");
	for (int i = 0; i < Hpbar * playerHealth; i += 5)
		gameTexture->Draw(135 + i, 14, &box);
	// Thanh hp cho boss
	gameTexture->LoadTexture("images/GameUI/enemy_hp.png");
	for (int i = 0; i < Hpbar * 20; i += 5)
		gameTexture->Draw(135 + i, 26, &box);

	// Khung item
	SetRect(&box, 0, 0, 34, 21);
	gameTexture->LoadTexture("images/GameUI/item_box_resize1.png");
	gameTexture->Draw(65 , 14, &box);

	// Icon item
	if (player->hasItem)
	{
		SetRect(&box, 0, 0, 16, 16);

		switch (player->getItem()->getType())
		{
		case UIT_THROWING_STAR:
			gameTexture->LoadTexture("images/items/item_throwing_star.png");
			break;
		case UIT_WINDMILL_THROWING_STAR:
			gameTexture->LoadTexture("images/items/item_windmill_throwing_star.png");
			break;
		}

		gameTexture->Draw(74, 16, &box);
	}
}

void Scorebar::Update()
{
	// Dùng hẳn cái switch vì thực tế có nhiều stage hơn
	switch (Game::getInstance()->getStageIndex())
	{
		// stage 3-1
	case 0:
		act = 1;
		break;
		// stage 3-2
	case 1:
		act = 2;
		break;
		// stage 3-3
	case 2:
		act = 3;
		break;
	}

	// Làm cái gì khác nữa...
	//update time
	//time = gì đó

	score = player->getScore();
	std::string scoreString = std::to_string(score);
	while (scoreString.length() < 8)
		scoreString = "0" + scoreString;


	std::string timeString = std::to_string(this->time);
	while (timeString.length() < 4)
		timeString = "0" + timeString;


	std::string stageString = std::to_string(this->act);
	stageString = "3 - " + stageString;

	playChance = player->getLife();
	std::string lifeString = std::to_string(this->playChance);
	while (lifeString.length() < 2)
		lifeString = "0" + lifeString;


	m = "SCORE  " + scoreString + "       STAGE " + stageString + "\n";
	m += "TIME      " + timeString + "               NINJA \n";
	m += "P         " + lifeString + "                    BOSS \n";

	playerHealth = player->getHP();
	if (player->getHP() <= 0)
	{
		player->setHP(player->getMaxHP());
		
		if (playChance <= 0) {
			player->setLife(3);
		}
		else
		{
			player->decrease_life();
		}
	}
	else
	{
		playerHealth = player->getHP();
	}

	// Vẽ lên (0, 0) trên backbuffer
	Draw(0, 0);
}


