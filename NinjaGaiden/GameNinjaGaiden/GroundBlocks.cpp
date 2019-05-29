#include "GroundBlocks.h"
#include "Collision.h"
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
	if (numberOfBlocks == 0)
	{
		fs.close();
		return;
	}

	groundBlocks = new GroundBlock*[numberOfBlocks];

	for (int i = 0; i < numberOfBlocks; i++)
	{
		fs >> id >> x >> y >> width >> height >> blockType;
		groundBlocks[i] = new GroundBlock(x, y, width, height, blockType);
	}

	fs.close();
	currentBlock = groundBlocks[0];
	currentHeight = 0;
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
	int i;
	GroundBlock * block;
	// count để kiểm tra xem có cái gì để giữ nhân vật đừng rơi xuống hay không (nếu = 0 thì là không có gì cả)
	int count = 0;

	if (player.getMinJumpHeight() > 0 || !player.isOnGround)
	{
		for (i = 0; i < numberOfBlocks; i++)
		{
			if (player.isJumping)
				count++;

			// player ở trong phạm vi X của ground block
			if (player.getLeft() > groundBlocks[i]->getLeft() - player.getWidth() &&
				player.getLeft() < groundBlocks[i]->getRight())
			{
				if (player.getBottom() >= groundBlocks[i]->getTop())
					count++;

				// xác định block đang đứng
				if (player.getBottom() >= groundBlocks[i]->getTop() && player.isOnGround && !player.isJumping)
				{
					currentBlock = groundBlocks[i];
					player.setMinJumpHeight(currentBlock->getTop());
					player.resetMaxJumpHeight();
				}

				//if (groundBlocks[i] != currentBlock)
				{
					if (player.getBottom() >= groundBlocks[i]->getTop() &&
						((player.getBottom() < currentBlock->getTop() && !player.isOnGround) ||
						(groundBlocks[i]->getTop() > currentBlock->getTop() && player.isJumping)))
					{
						player.setMinJumpHeight(groundBlocks[i]->getTop());
					}
				}

				groundBlocks[i]->Update(dt, player);
			}
		}
	}

	// không đứng trên mảnh đất nào;
	if (count == 0)
	{
		player.setMinJumpHeight(-player.getHeight()*5);
		player.resetMaxJumpHeight();
		// player.SetStatus(PLAYER_DIE);
	}
}
