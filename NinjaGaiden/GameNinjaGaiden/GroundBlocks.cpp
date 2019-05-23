#include "GroundBlocks.h"
#include <fstream>

#include"DXInput.h"
#include <string>

GroundBlocks::GroundBlocks(const char * filePath)
{
	int id;
	float x, y, width, height;
	int blockType;

	std::ifstream fs(filePath);
	if (!fs.is_open()) {
		MessageBox(0, "Lỗi mở file ground block", "Lỗi kìa", 0);
		return;
	}

	fs >> numberOfBlocks;
	groundBlocks = new GroundBlock*[numberOfBlocks];

	for (int i = 0; i < numberOfBlocks; i++)
	{
		fs >> id >> x >> y >> width >> height >> blockType;
		groundBlocks[i] = new GroundBlock(x, y, width, height, blockType);
	}

	fs.close();
}


GroundBlocks::~GroundBlocks()
{
}

int GroundBlocks::getNumberOfBlocks()
{
	return numberOfBlocks;
}

GroundBlocks GroundBlocks::getGroundBlock(int index)
{
	return NULL;
}

void GroundBlocks::Update(DWORD dt, GameObject &player)
{
	for (int i = 0; i < numberOfBlocks; i++)
	{
		GroundBlock block = *groundBlocks[i];
		if (player.getLeft() > block.getLeft() - player.getWidth() && player.getLeft() < block.getRight())
		{
			if (Key_Down(DIK_Z))
			{
				std::string msg = std::to_string(groundBlocks[i]->getLeft()) + " " + std::to_string(groundBlocks[i]->getRight());
				MessageBox(0, msg.c_str(), "X", 0);
			}

			// chủ yếu là xử lý va chạm
			groundBlocks[i]->Update(dt, player);
		}
	}
}
