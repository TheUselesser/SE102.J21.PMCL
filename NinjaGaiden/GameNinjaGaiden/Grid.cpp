#include "Grid.h"
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


void Grid::InitGrid()
{
	cell = new GridCell*[numberOfRows];
	for (int row = 0; row < numberOfRows; row++)
	{
		cell[row] = new GridCell[numberOfColumns];
		
		for (int col = 0; col < numberOfColumns; col++)
		{
			cell[row][col] = GridCell();	// 
		}
	}
}

std::vector<GameObject*> Grid::GetObjectList()
{
	int row, col;
	std::vector<GameObject*> objectList;

	for (row = 0; row < numberOfRows; row++)
	{
		for (col = 0; col < numberOfColumns; col++)
		{
			// Thao tác với cell[y][x]
			if (!cell[row][col].isEmpty)
				for (int i = 0; i < cell[row][col].getListSize(); i++)
					objectList.push_back(cell[row][col].getObjectList()[i]);
		}
	}

	return objectList;
}

std::vector<GameObject*> Grid::GetObjectList(Camera * camera)
{
	int row, col;
	std::vector<GameObject*> objectList;

	firstColumn = (int)camera->getLeft() / cellSize;
	firstRow = (int)camera->getBottom() / cellSize;
	lastColumn = (int)camera->getRight() / cellSize;
	lastRow = (int)camera->getTop() / cellSize;

	for (row = firstRow; row <= lastRow; row++)
	{
		for (col = firstColumn; col <= lastColumn; col++)
		{
			// Thao tác với cell[y][x]
			if (!cell[row][col].isEmpty)
			for (int i = 0 ; i < cell[row][col].getListSize(); i++)
				objectList.push_back(cell[row][col].getObjectList()[i]);
		}
	}

	return objectList;
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
				if (isEmpty)	isEmpty = false;

				for (int i = 0; i < cell[row][col].getListSize();)
				{
					fs >> columnIndex >> rowIndex >> objTypeID >> nonsense >> objX >> objY;

					if (rowIndex == cell[row][col].getRowIndex() && columnIndex == cell[row][col].getColumnIndex())
					{
						cell[row][col].addObject(objTypeID, objX, objY);
						i++;	// tự tin không sợ vòng lặp vô hạn
					}
				}
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

	InitGrid();

	// 
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


void Grid::UpdateCellsSet(Camera * camera)
{
	firstColumn = (int)camera->getLeft() / cellSize;
	firstRow = (int)camera->getBottom() / cellSize;
	lastColumn = (int)camera->getRight() / cellSize;
	lastRow = (int)camera->getTop() / cellSize;
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