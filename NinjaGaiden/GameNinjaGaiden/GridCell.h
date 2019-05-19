#pragma once
#include "Rect.h"
#include "Collision.h"
#include "GameObject.h"
#include <vector>

#define DEFAULT_CELL_SIZE	120

class GridCell
{
	int columnIndex, rowIndex;
	int size;
	int listSize;
	std::vector<GameObject*> objectList;

public:
	bool isEmpty;

	GridCell();
	~GridCell();

	int getColumnIndex();	void setColumnIndex(int x);
	int getRowIndex();		void setRowIndex(int y);
	int getSize();		void setSize(int size);
	int getListSize();	void setListSize(int n);

	void addObject(int objTypeID, float x, float y);
	std::vector<GameObject*> getObjectList();

	void setInUsed(bool isUsing);

	// có lẽ chưa cần sử dụng
	Rect getBound();
	bool isIntersect(Rect rect);
};

