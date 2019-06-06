#pragma once
#include "GameTexture.h"
#include "Player.h"
#include "Game.h"

class Scorebar
{
	static Scorebar * instance;

	ID3DXFont *FONT;
	RECT *Rect;
	std::string m;

	GameTexture *gameTexture;

	Scorebar();
	~Scorebar();
public:
	bool isInit = true;

	static Scorebar * getInstance();

	int playerHP;
	int playerSpiritualStr; // kiểu như mana
	//int bossHP;
	int score;
	int Time;
	int playChance = 2;
	int act = 3, stage = 1;

	void Init();
	void Draw(int x, int y);
	void Update();

};