﻿#include "Grid.h"
#include <fstream>
#include <string>
#include "DXInput.h"
Grid::Grid()
{
	numberOfColumns = numberOfRows = 0;
	cellSize = DEFAULT_CELL_SIZE;
	numberOfObjects = 0;
	isEmpty = true;
}


Grid::~Grid()
{
}


void Grid::InitGrid(Camera * camera)
{
	if (!this->isEmpty)
		objects.clear();

	UpdateCellsSet(camera);

	for (int row = firstRow; row <= lastRow; row++)
	{
		for (int col = firstColumn; col <= lastColumn; col++)
		{
			// Thao tác với cell[y][x]
			if (!cell[row][col].isEmpty)
			{
				for (int i = 0; i < cell[row][col].getListSize(); i++)
				{
					cell[row][col].getObjectList()[i]->isExist = true;
					cell[row][col].getObjectList()[i]->isInCellsSet = true;
					objects.push_back(cell[row][col].getObjectList()[i]);
				}
			}
		}
	}
}


void Grid::UpdateCellsSet(Camera * camera)
{
	firstColumn = (int)camera->getLeft() / cellSize;
	firstRow = (int)camera->getBottom() / cellSize;
	lastColumn = (int)camera->getRight() / cellSize;
	lastRow = (int)camera->getTop() / cellSize;
}

void Grid::UpdateObjectList(Camera * camera)
{
	std::vector<GameObject*> newObjectList;

	// cập nhật bộ cells đang hoạt động
	UpdateCellsSet(camera);

	for (int i = 0; i < objects.size(); i++)
	{
		// Nếu object là enemy, không thuộc cellsSet và nằm ngoài camera thì bỏ luôn
		if (objects[i]->getCollisionType() == COLLISION_TYPE_ENEMY &&
			!objects[i]->isInCellsSet &&
			!(objects[i]->getRight() > camera->getLeft() && objects[i]->getLeft() < camera->getRight()))
		{
			objects[i]->isExist = false;
		}

		if (objects[i]->isExist)
		{	
			newObjectList.push_back(objects[i]);
		}
		else
		{
			//delete objects[i];
			//objects[i]->isExist = false;
		}
	}

	objects.clear();
	objects = newObjectList;
}

std::vector<GameObject*> Grid::GetObjectList(Camera * camera)
{
	return objects;
}

void Grid::readCellsInfo(const char * cellsInfoPath)
{
	// index để check có đúng cell không, tránh đọc file bị thừa quá nhiều
	int rowIndex, columnIndex;
	int objTypeID;
	std::string nonsense = "";	// ^_^
	float objX, objY;

	std::ifstream fs(cellsInfoPath);
	if (!fs.is_open()) {
		MessageBox(0, "Lỗi mở file grid info", "Lỗi kìa", 0);
		return;
	}

	int row, col;
	for (col = 0; col < numberOfColumns; col++)
	{
		for (row = 0; row < numberOfRows; row++)
		{
			if (!cell[row][col].isEmpty)
			{
				if (this->isEmpty)	this->isEmpty = false;

				for (int i = 0; i < cell[row][col].getListSize();)
				{
					fs >> columnIndex >> rowIndex >> objTypeID >> nonsense >> objX >> objY;

					// check xem có đúng cell không
					if (rowIndex == cell[row][col].getRowIndex() && columnIndex == cell[row][col].getColumnIndex())
					{
						// thêm objectInfo vào objectInfoList của cell
						cell[row][col].addObjectInfo(objX, objY, objTypeID);
						i++;	// tự tin không sợ vòng lặp vô hạn
					}
				}

				// tạo hết object luôn cho máu
				cell[row][col].InitAllObjects();
			}
		}
	}

	fs.close();
}

void Grid::readGridInfo(const char * gridInfoPath, const char * cellsInfoPath)
{
	std::ifstream fs(gridInfoPath);
	if (!fs.is_open()) {
		MessageBox(0, "Lỗi mở file grid info", "Lỗi kìa", 0);
		return;
	}

	fs >> numberOfColumns >> numberOfRows;

	if (numberOfColumns == 0 || numberOfRows == 0)
	{
		isEmpty = true;
		return;
	}
	else
	{
		isEmpty = false;
	}

	// tạo mảng cell
	cell = new GridCell*[numberOfRows];
	for (int row = 0; row < numberOfRows; row++)
	{
		cell[row] = new GridCell[numberOfColumns];

		for (int col = 0; col < numberOfColumns; col++)
		{
			cell[row][col] = GridCell();	// 
		}
	}

	// Khởi tạo cell
	for (int i = 0; i < numberOfRows * numberOfColumns; i++)
	{
		int col, row, listSize;
		fs >> col >> row >> listSize;
		numberOfObjects += listSize;

		cell[row][col].isEmpty = listSize > 0 ? false : true;
		cell[row][col].setColumnIndex(col);
		cell[row][col].setRowIndex(row);
		cell[row][col].setListSize(listSize);
	}

	fs.close();

	readCellsInfo(cellsInfoPath);
}


D3DXVECTOR2 Grid::GetFirstCellPosition()
{
	D3DXVECTOR2 position;
	position.x = firstColumn;
	position.y = firstRow;
	return position;
}
D3DXVECTOR2 Grid::GetLastCellPosition()
{
	D3DXVECTOR2 position;
	position.x = lastColumn;
	position.y = lastRow;
	return position;
}

// Chỉ đúng với setting game hiện tại hoặc tương tự
void Grid::IgnoreLeft(Camera * camera)
{
	cell[lastRow][lastColumn - 3].disableUpdate();
	cell[lastRow - 1][lastColumn - 3].disableUpdate();
}

void Grid::IgnoreRight(Camera * camera)
{
	cell[firstRow][firstColumn + 3].disableUpdate();
	cell[firstRow + 1][firstColumn + 3].disableUpdate();
}

void Grid::AddLeft(Camera * camera, GameObject * player)
{
	GridCell *cell_1, *cell_2;
	cell_1 = &cell[firstRow][firstColumn];
	cell_2 = &cell[firstRow + 1][firstColumn];


	for (int i = 0; i < cell_1->getObjectList().size(); i++)
	{
		if (!cell_1->getObjectList()[i]->isExist)
		{
			objects.push_back(cell_1->getObjectList()[i]);
			cell_1->enableUpdate(player);
		}
	}
	for (int i = 0; i < cell_2->getObjectList().size(); i++)
	{
		if (!cell_2->getObjectList()[i]->isExist)
		{
			objects.push_back(cell_2->getObjectList()[i]);
			cell_2->enableUpdate(player);
		}
	}

	/*objects.reserve(objects.size() + cell_1->getObjectList().size());
	std::copy(cell_1->getObjectList().begin(), cell_1->getObjectList().end(), objects.end());
	objects.reserve(objects.size() + cell_2->getObjectList().size());
	std::copy(cell_2->getObjectList().begin(), cell_2->getObjectList().end(), objects.end());*/
}

void Grid::AddRight(Camera * camera, GameObject * player)
{

	GridCell *cell_1, *cell_2;
	cell_1 = &cell[lastRow][lastColumn];
	cell_2 = &cell[lastRow - 1][lastColumn];

	for (int i = 0; i < cell_1->getObjectList().size(); i++)
	{
		if (!cell_1->getObjectList()[i]->isExist)
		{
			objects.push_back(cell_1->getObjectList()[i]);
			cell_1->enableUpdate(player);
		}
	}
	for (int i = 0; i < cell_2->getObjectList().size(); i++)
	{
		if (!cell_2->getObjectList()[i]->isExist)
		{
			objects.push_back(cell_2->getObjectList()[i]);
			cell_2->enableUpdate(player);
		}
	}

	/*objects.reserve(objects.size() + cell_1->getObjectList().size());
	std::copy(cell_1->getObjectList().begin(), cell_1->getObjectList().end(), objects.end());
	objects.reserve(objects.size() + cell_2->getObjectList().size());
	std::copy(cell_2->getObjectList().begin(), cell_2->getObjectList().end(), objects.end());*/
}