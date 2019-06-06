﻿#include "Stage.h"
#include "Game.h"
#include <fstream>
#include <string>
#include "DXInput.h"


Stage * Stage::instance = NULL;
Stage * Stage::getInstance()
{
	if (instance == NULL)
	{
		instance = new Stage();
	}
	return instance;
}

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
	grid->InitGrid(Camera::getInstance());

	if (!grid->isEmpty)
	{
		objectList = grid->GetObjectList(Camera::getInstance());
		prevFirstCellPosition = grid->GetFirstCellPosition();
		prevLastCellPosition = grid->GetLastCellPosition();
	}
}

std::vector<GameObject*> Stage::GetObjectList()
{
	return grid->GetObjectList(Camera::getInstance());
}

std::vector<GameObject*> Stage::GetAllObjects()
{
	return grid->GetObjectList();
}

void Stage::Update(DWORD dt, Player * player)
{
	// Update grid
	if (!grid->isEmpty)
	{
		grid->UpdateCellsSet(Camera::getInstance());
		D3DXVECTOR2 firstCellPosition = grid->GetFirstCellPosition();
		D3DXVECTOR2	lastCellPosition = grid->GetLastCellPosition();

		// Khi camera đi vào cell mới của grid thì load object của cell mới zô objectList 
		if (firstCellPosition.x != prevFirstCellPosition.x || firstCellPosition.y != prevFirstCellPosition.y ||
			lastCellPosition.x != prevLastCellPosition.x || lastCellPosition.y != prevLastCellPosition.y)
		{
			grid->UpdateObjectList(Camera::getInstance());

			// camera dịch qua phải => xóa object của cell bên trái
			if (firstCellPosition.x > prevFirstCellPosition.x || lastCellPosition.x > prevLastCellPosition.x)
			{
				std::string msg = std::to_string(firstCellPosition.x);

				grid->IgnoreLeft(Camera::getInstance());
				if (lastCellPosition.x > prevLastCellPosition.x)
				{
					grid->AddRight(Camera::getInstance(), Player::getInstance());
				}
			}
			// camera dịch qua trái => xóa object của cell bên phải
			else
			{
				grid->IgnoreRight(Camera::getInstance());
				if (firstCellPosition.x < prevFirstCellPosition.x)
					grid->AddLeft(Camera::getInstance(), Player::getInstance());
			}

			objectList = grid->GetObjectList(Camera::getInstance());
			
			prevFirstCellPosition = firstCellPosition;
			prevLastCellPosition = lastCellPosition;
		}

		// lần lượt update các object
		for (int i = 0; i < objectList.size(); i++)
		{
			if (objectList[i]->isExist)
			{
				Collision::CollisionHandle(*player, *objectList[i]);
				for (int j = 0; j < groundBlocks->getNumberOfBlocks(); j++)
				{
					objectList[i]->MindTheGroundBlocks(groundBlocks->getGroundBlock(j));
				}
				objectList[i]->Update(dt, *player);
			}
		}
	}

	// Update ground
	groundBlocks->Update(dt, *player);
}


// MAP

void Stage::LoadTilemap(const char * imagePath, const char * matrixPath)
{
	tilemap = new Tilemap();
	tilemap->LoadTilemap(imagePath, matrixPath);

	mapStart = 0;
	mapEnd = tilemap->mapWidth;
}

void Stage::LoadGroundBlocks(const char * filePath)
{
	groundBlocks = new GroundBlocks(filePath);
}

void Stage::Draw(Camera * camera)
{
	tilemap->Draw(camera);
}

void Stage::Release()
{
	//...
}
