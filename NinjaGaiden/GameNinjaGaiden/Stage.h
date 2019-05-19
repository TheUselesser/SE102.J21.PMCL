#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "Tilemap.h"
#include "Camera.h"
#include "GameObject.h"
#include <vector>
#include "SwordMan.h"
#include "Player.h"
#include "Grid.h"

class Stage
{
	Tilemap * tilemap;

	int mapStart, mapEnd;
	int playerStart, playerEnd;

	std::vector<GameObject*> objectList;

	Grid * grid;
	D3DXVECTOR2 pevFirstCellPosition;

public:
	Stage();
	~Stage();

	int getMapStart();	void setMapStart(int mapStart);
	int getMapEnd();	void setMapEnd(int mapEnd);
	int getPlayerStart();	void setPlayerStart(int playerStart);
	int getPlayerEnd();	void setPlayerEnd(int playerEnd);

	// load trực tiếp tất cả enemy từ file
	/*
	void LoadObjects(const char * filePath);
	GameObject getObject(int index);
	*/
	void Update(DWORD dt, Player &player);

	// dùng grid
	void InitGrid(const char * gridInfoPath, const char * cellsInfoPath);
	std::vector<GameObject*> GetObjectList();

	// Tilemap
	void LoadTilemap(const char * imagePath, const char * matrixPath);
	void Draw(Camera * camera);	// vẽ lên camera

	void Release();
};

