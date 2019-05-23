#pragma once
#include "GroundBlock.h"
#include "GameObject.h"

class GroundBlocks
{
	GroundBlock * *groundBlocks;
	int numberOfBlocks;
public:
	GroundBlocks(const char * filePath);
	~GroundBlocks();

	int getNumberOfBlocks();
	GroundBlocks getGroundBlock(int index);
	void Update(DWORD dt, GameObject &player);
};

