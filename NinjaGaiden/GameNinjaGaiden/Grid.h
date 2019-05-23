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
	bool isEmpty;

	Grid();
	~Grid();

	void readCellsInfo(const char * cellsInfoPath);
	void readGridInfo(const char * gridInfoPath, const char * cellsInfoPath);
	
	// liên tục cập nhật bộ cells tùy theo vị trí của camera
	void UpdateCellsSet(Camera * camera);
	// để check xem bộ cells có thay đổi hay không
	D3DXVECTOR2 GetFirstCellPosition();
	D3DXVECTOR2 GetLastCellPosition();

	// Lấy toàn bộ object trong grid (để test, không dùng)
	std::vector<GameObject*> GetObjectList();
	// Lấy object từ những cell giao với camera
	std::vector<GameObject*> GetObjectList(Camera * camera);
};

