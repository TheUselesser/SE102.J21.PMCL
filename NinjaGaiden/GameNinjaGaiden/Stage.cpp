#include "Stage.h"
#include "Game.h"
#include <fstream>
#include <string>

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

// OBJECT

void Stage::InitGrid(const char * gridInfoPath, const char * cellsInfoPath)
{
	grid = new Grid();
	grid->readGridInfo(gridInfoPath, cellsInfoPath);

	objectList = grid->GetObjectList(Camera::getInstance());
}

std::vector<GameObject*> Stage::GetObjectList()
{
	return grid->GetObjectList(Camera::getInstance());
}

void Stage::Update(DWORD dt, Player &player)
{
	grid->UpdateFirstCellPosition(Camera::getInstance());
	D3DXVECTOR2 firstCellPosition = grid->GetFirstCellPosition();
	if (firstCellPosition.x != pevFirstCellPosition.x || firstCellPosition.y != pevFirstCellPosition.y)
	{
		objectList = grid->GetObjectList(Camera::getInstance());
	}

	for (int i = 0; i < objectList.size(); i++)
	{
		objectList[i]->Update(dt, player);
	}
}


// MAP

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
