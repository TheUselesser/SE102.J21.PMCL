#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "Tilemap.h"
#include "Camera.h"

class Stage
{
	Tilemap * tilemap;

	int mapStart, mapEnd;
	int playerStart, playerEnd;

public:
	Stage();
	~Stage();

	int getMapStart();	void setMapStart(int mapStart);
	int getMapEnd();	void setMapEnd(int mapEnd);
	int getPlayerStart();	void setPlayerStart(int playerStart);
	int getPlayerEnd();	void setPlayerEnd(int playerEnd);


	void LoadTilemap(const char * imagePath, const char * matrixPath);
	void Draw(Camera * camera);	// vẽ lên camera

	void Release();
};

