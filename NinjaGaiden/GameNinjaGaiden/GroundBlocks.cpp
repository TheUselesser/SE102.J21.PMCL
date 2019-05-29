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
	currentHeight = new float[numberOfBlocks];

	for (int i = 0; i < numberOfBlocks; i++)
	{
		fs >> id >> x >> y >> width >> height >> blockType;
		groundBlocks[i] = new GroundBlock(x, y, width, height, blockType);
		currentHeight[i] = 0;
	}

	fs.close();
	currentBlock = groundBlocks[0];
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
	minHeight = -1;

	for (i = 0; i < numberOfBlocks; i++)
	{
		if (player.isJumping)
		{
			count++;
		}

		// player ở trong phạm vi X của ground block
		if (player.getLeft() > groundBlocks[i]->getLeft() - player.getWidth() &&
			player.getLeft() < groundBlocks[i]->getRight())
		{

			if (player.getBottom() >= groundBlocks[i]->getTop())
			{
				count++;
				currentHeight[i] = player.getBottom() - groundBlocks[i]->getTop();
				if (minHeight == -1) minHeight = currentHeight[i];
			}

			// xác định block đang đứng
			if (player.getBottom() >= groundBlocks[i]->getTop() && player.isOnGround && !player.isJumping)
			{
				if (currentHeight[i] <= minHeight)
				{
					minHeight = currentHeight[i];
					currentBlock = groundBlocks[i];
					player.setMinJumpHeight(currentBlock->getTop());
					player.resetMaxJumpHeight();
				}
			}

			if (!player.isOnGround || player.isJumping)
			{
				if (player.getBottom() >= groundBlocks[i]->getTop() &&
					(player.getBottom() < currentBlock->getTop() ||
					(groundBlocks[i]->getTop() > currentBlock->getTop())))
				{
					player.setMinJumpHeight(groundBlocks[i]->getTop());
				}
			}

			groundBlocks[i]->Update(dt, player);
		}
	}

	// không đứng trên mảnh đất nào;
	if (count == 0)
	{
		player.setMinJumpHeight(-player.getHeight() * 5);
		player.resetMaxJumpHeight();
		if (player.getY() < 0) player.SetStatus(PLAYER_DIE);
	}
}
