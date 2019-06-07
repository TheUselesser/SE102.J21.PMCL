#pragma once
#include "GameTexture.h"
#include "Player.h"
#include "Game.h"
#include <vector>

class Scorebar
{
	int playerHP;
	int bossHP;
	int playChance = 2;
	int health;
	int bossHealth;
	int maxHealth;
	int heartCount;
	int score;
	int time;
	int act;

	static Scorebar * instance;

	ID3DXFont *FONT;
	RECT *Rect;
	std::string m;

	std::vector<Sprite*> *playerHPList;
	std::vector<Sprite*> *bossHPList;
	std::vector<Sprite*> *emptyHPList;

	Sprite* black;
	Sprite* sprite;

	GameTexture *gameTexture;

	Scorebar();
	~Scorebar();
public:
	bool isInit = true;

	static Scorebar * getInstance();

	

	bool Init();
	void Draw(int x, int y);
	void Update();
};