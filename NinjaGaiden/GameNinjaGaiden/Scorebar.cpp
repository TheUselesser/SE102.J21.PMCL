#include "Scorebar.h"
#include "Game.h"

#include <string>
#include <sstream>



int FONT_SIZE = 8;
std::string Int2String(int x)
{
	std::string out_string;
	std::stringstream ss;
	ss << x;
	out_string = ss.str();

	return out_string;
}

Scorebar::Scorebar()
{
	//playerHP = 10;//cần 1 hàm lấy máu của ryu 
	score = 000000;
	time = 1000;
	playChance = 2;
	//1 vài biến khác nữa
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
	gameTexture = new GameTexture();
	//gameTexture->LoadTexture("images/Scorebar/newUI.png", D3DCOLOR_XRGB(255, 255, 255));
	////load font
	//FONT = NULL;
	//HRESULT hr = D3DXCreateFont(
	//	Game::getInstance()->get3DDevice(),
	//	FONT_SIZE,
	//	0,
	//	FW_NORMAL,
	//	1,
	//	false,
	//	DEFAULT_CHARSET,
	//	OUT_DEFAULT_PRECIS,
	//	ANTIALIASED_QUALITY,
	//	FF_DONTCARE,
	//	"Ninja Gaiden (NES)",
	//	&FONT);
	//if (!SUCCEEDED(hr))
	//{
	//	isInit = false;
	//}

	this->playerHP = 10;//có hàm get HP ở ryu 
	this->bossHP = 10;// có hàm get HP ở boss

	black = new Sprite();
	black->LoadTexture("images/GameUI/black.png", D3DCOLOR_XRGB(255, 255, 255));

	
	playerHPList = new std::vector<Sprite*>();
	for (int i = 0; i < 16; i++)
	{
		Sprite* sprite = new Sprite(120 + 12 * (i + 1), 67);
		sprite->LoadTexture("images/GameUI/hp.png", D3DCOLOR_XRGB(255, 255, 255));
		playerHPList->push_back(sprite);
	}

	bossHPList = new std::vector<Sprite*>();
	for (int i = 0; i < 16; i++)
	{
		Sprite* sprite = new Sprite(120 + 12 * (i + 1), 67);
		sprite->LoadTexture("images/GameUI/enemy_hp.png", D3DCOLOR_XRGB(255, 255, 255));
		bossHPList->push_back(sprite);
	}

	emptyHPList = new std::vector<Sprite*>();
	for (int i = 0; i < 16; i++)
	{
		Sprite* sprite = new Sprite(120 + 12 * (i + 1), 43);
		sprite->LoadTexture("images/GameUI/empty_hp.png", D3DCOLOR_XRGB(255, 255, 255));
		emptyHPList->push_back(sprite);
	}

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
	SetRect(Rect, 0, 20, 256, 40);
	m = "SCORE_000000 TIME 0000 STAGE 00\n";
	m += "PLAYER                =62\n";
	m += "ENEMY                P=3\n";


	return true;
}

void Scorebar::Draw(int x, int y)
{
	gameTexture = new GameTexture();
	SetRect(Rect, x, y, 256, 116);

	if (FONT)
		FONT->DrawTextA(NULL, m.c_str(), -1, Rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	black->Draw(black->getPosition(), Rect);



	for (std::vector<Sprite*>::iterator i = emptyHPList->begin(); i != emptyHPList->end(); i++)
	{
		SetRect(Rect, 0, 0, 8, 15);
		(*i)->Draw((*i)->getPosition(), Rect);
		D3DXVECTOR3 newPosition = (*i)->getPosition();
		newPosition.y = 67;
		(*i)->Draw(newPosition, Rect);
	}


	int count = 0;
	for (std::vector<Sprite*>::iterator i = playerHPList->begin(); i != playerHPList->end(); i++)
	{
		if (count < playerHP)
		{
			SetRect(Rect, 0, 0, 8, 15);
			(*i)->Draw((*i)->getPosition(), Rect);
		}
		count++;
	}


	count = 0;
	for (std::vector<Sprite*>::iterator i = bossHPList->begin(); i != bossHPList->end(); i++)
	{
		if (count < bossHP)
		{
			SetRect(Rect, 0, 0, 8, 15);
			(*i)->Draw((*i)->getPosition(), Rect);
		}
		count++;
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


	// Vẽ lên (0, 0) trên backbuffer
	Draw(0, 0);
}


