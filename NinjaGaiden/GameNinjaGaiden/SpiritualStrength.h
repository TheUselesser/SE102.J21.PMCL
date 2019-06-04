#pragma once
#include "Item.h"

enum SPIRITUAL_STRENGTH_TYPE
{
	SPIRITUAL_STRENGTH_5,
	SPIRITUAL_STRENGTH_10,
};

class SpiritualStrength :
	public Item
{
	SPIRITUAL_STRENGTH_TYPE type;
public:
	SpiritualStrength();
	~SpiritualStrength(); 
	
	void setType(SPIRITUAL_STRENGTH_TYPE type);
	SPIRITUAL_STRENGTH_TYPE getType();
	
	void Init(float x, float y);
	void Update(DWORD dt, GameObject &player);
	void CheckCollisionStatus(GameObject * player);
};

