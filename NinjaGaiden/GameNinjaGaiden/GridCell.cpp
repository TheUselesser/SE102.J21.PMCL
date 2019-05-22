#include "GridCell.h"
#include "Banshee.h"
#include "BirdBrown.h"
#include "MachineGunGuy.h"
#include "SwordMan.h"
#include "Cat.h"
#include "Cannoneer.h"
#include "Runner.h"
#include "ItemContainer.h"

#include<string>

GridCell::GridCell()
{
	columnIndex = rowIndex = 0;
	size = DEFAULT_CELL_SIZE;
	isEmpty = true;
}

GridCell::~GridCell()
{
}

int GridCell::getColumnIndex()
{
	return columnIndex;
}

void GridCell::setColumnIndex(int x)
{
	this->columnIndex = x;
}

int GridCell::getRowIndex()
{
	return rowIndex;
}

void GridCell::setRowIndex(int y)
{
	this->rowIndex = y;
}

int GridCell::getSize()
{
	return size;
}

void GridCell::setSize(int size)
{
	this->size = size;
}

int GridCell::getListSize()
{
	return listSize;
}

void GridCell::setListSize(int n)
{
	this->listSize = n;
}

void GridCell::addObject(int objTypeID, float x, float y)
{
	// Theo lý thuyết thì không create object, chỉ add số liệu từ file để đến lúc thì mới create
	//		nhưng thôi cứ create rồi add nguyên con vào list luôn

	GameObject * object = NULL;

	switch (objTypeID)
	{
	case 1:
		break;
	case 2:
		object = new Banshee(x, y);
		object->setObjectType(ENEMY_BANSHEE);
		object->Init();
		break;
	case 3:
		// Bat (brown)
		break;
	case 4:
		break;
	case 5:
		object = new BirdBrown(x, y);
		object->setObjectType(ENEMY_BIRD_BROWN);
		object->Init();
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		object = new MachineGunGuy(x, y);
		object->setObjectType(ENEMY_MACHINE_GUN_GUY);
		object->Init();
		break;
	case 11:
		break;
	case 12:
		object = new SwordMan(x, y);
		object->setObjectType(ENEMY_SWORD_MAN);
		object->Init();
		break;
	case 13:
		object = new Cat(x, y);
		object->setObjectType(ENEMY_CAT);
		object->Init();
		break;
	case 14:
		object = new Cannoneer(x, y);
		object->setObjectType(ENEMY_CANNONEER);
		object->Init();
		break;
	case 15:
		MessageBox(0, "", "15", 0);
		object = new Runner(x, y);
		object->setObjectType(ENEMY_RUNNER);
		object->Init();
		break;
	case 24:
		object = new ItemContainer(x, y);
		object->setObjectType(ITEM_ITEM_CONTAINER_4);
		object->Init();
		break;
	default:
		break;
	}

	objectList.push_back(object);
}

std::vector<GameObject*> GridCell::getObjectList()
{
	return objectList;
}

void GridCell::setInUsed(bool isUsing)
{
	if (isUsing)
	{
		for (int i = 0; i < listSize; i++)
		{
			objectList[i]->setSpawned(true);
		}
	}
	else
	{
		for (int i = 0; i < listSize; i++)
		{
			objectList[i]->setSpawned(false);
		}
	}
}


// Có lẽ không bao giờ sử dụng
Rect GridCell::getBound()
{
	Rect bound;
	bound.setX(columnIndex * size);
	bound.setY(rowIndex * size + size);
	bound.setSize(size, size);
	return bound;
}

bool GridCell::isIntersect(Rect rect)
{
	// Kiểm tra xem cell có giao với cái rect không
	Rect bound = getBound();
	return Collision::AABBCheck(bound, rect);
}
