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
	std::vector<D3DXVECTOR4*> objectInfoList;

public:
	bool isEmpty;

	GridCell();
	~GridCell();

	int getColumnIndex();	void setColumnIndex(int x);
	int getRowIndex();		void setRowIndex(int y);
	int getSize();		void setSize(int size);
	int getListSize();	void setListSize(int n);

	void InitObject(float x, float y, int objTypeID, int itemTypeID = -1);
	void InitObject(D3DXVECTOR4 * objectInfo);
	void InitAllObjects();

	void addObjectInfo(float x, float y, int objTypeID, int itemTypeID = -1);
	std::vector<GameObject*> getObjectList();
	std::vector<D3DXVECTOR4*> getObjectInfoList();

	void enableUpdate(GameObject * player);
	void disableUpdate();

	// có lẽ chưa cần sử dụng
	Rect getBound();
	bool isIntersect(Rect rect);
};

