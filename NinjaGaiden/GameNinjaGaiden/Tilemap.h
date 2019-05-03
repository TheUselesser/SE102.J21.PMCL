#pragma once
#include "GameTexture.h"
#include "Camera.h"

class Tilemap
{
	int tileWidth, tileHeight;
	int mapRows, mapCols;	// số hàng và cột của map	(tính theo tile)
	int tilesheetCols;		// số cột của tilesheet
	
	int **matrix;	// ma trận các tile index hiển thị cho map;
	GameTexture * tilesheet = NULL;

public:
	int mapWidth;

	Tilemap();
	~Tilemap();

	void LoadTilemap(const char * imagePath, const char * matrixPath);	// đường dẫn tilesheet và matrix
	void Draw(Camera * camera);
};

