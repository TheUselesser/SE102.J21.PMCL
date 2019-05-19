#pragma once
#include <vector>
#include "GridCell.h"
#include "Camera.h"
#include "GameObject.h"

class Grid
{
	GridCell **cell;
	int cellSize;
	int numberOfColumns, numberOfRows;
	int numberOfObjects;

	// Giới hạn nhóm các cell giao với camera
	int firstColumn, firstRow, lastColumn, lastRow;

	void InitGrid();

public:
	Grid();
	~Grid();

	void readCellsInfo(const char * cellsInfoPath);
	void readGridInfo(const char * gridInfoPath, const char * cellsInfoPath);
	

	void UpdateFirstCellPosition(Camera * camera);
	D3DXVECTOR2 GetFirstCellPosition();

	// Lấy toàn bộ object trong grid
	std::vector<GameObject*> GetObjectList();
	// Lấy object từ những cell giao với camera
	std::vector<GameObject*> GetObjectList(Camera * camera);
};

