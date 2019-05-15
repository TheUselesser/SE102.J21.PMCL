#include "Stage.h"
#include "Game.h"


Stage::Stage()
{
	mapStart = mapEnd = playerStart = playerEnd = 0;
}


Stage::~Stage()
{
}

int Stage::getMapStart()
{
	return mapStart;
}

void Stage::setMapStart(int mapStart)
{
	this->mapStart = mapStart;
}

int Stage::getMapEnd()
{
	return mapEnd;
}

void Stage::setMapEnd(int mapEnd)
{
	this->mapEnd = mapEnd;
}

int Stage::getPlayerStart()
{
	return playerStart;
}

void Stage::setPlayerStart(int playerStart)
{
	this->playerStart = playerStart;
}

int Stage::getPlayerEnd()
{
	return playerEnd;
}

void Stage::setPlayerEnd(int playerEnd)
{
	this->playerEnd = playerEnd;
}

void Stage::LoadTilemap(const char * imagePath, const char * matrixPath)
{
	tilemap = new Tilemap();
	tilemap->LoadTilemap(imagePath, matrixPath);

	mapStart = 0;
	mapEnd = tilemap->mapWidth;
}

void Stage::Draw(Camera * camera)
{
	tilemap->Draw(camera);
}

void Stage::Release()
{
	//...
}
