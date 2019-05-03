#include "Tilemap.h"
#include <fstream>

// Hiển thị integer lên MessageBox:
//	#include <string>
//		std::string message = std::to_string(int);
//		MessageBox(0, message.c_str(), "Show int", 0);
// Để test thôi ý mà

Tilemap::Tilemap()
{
}


Tilemap::~Tilemap()
{
}

void Tilemap::LoadTilemap(const char * imagePath, const char * matrixPath)
{
	//tilesheet->LoadTexture(imagePath);
	if (tilesheet == NULL) tilesheet = new GameTexture(imagePath);

	std::ifstream fs(matrixPath);

	if (!fs.is_open()) {
		MessageBox(0, "Loi mo file", "Loi kia", 0);
		return;
	}

	fs >> mapRows >> mapCols >> tileWidth >> tileHeight >> tilesheetCols;

	matrix = new int*[mapRows];
	for (int row = 0; row < mapRows; row++)
	{
		matrix[row] = new int[mapCols];
		for (int col = 0; col < mapCols; col++)
		{
			fs >> matrix[row][col];
		}
	}

	mapWidth = mapCols * tileWidth;

	fs.close();
}

void Tilemap::Draw(Camera * camera)
{
	// Tile 4 rìa camera
	int left, right, top, bottom;
	int mapHeight = mapRows * tileHeight;

	/*left = camera->left / tileWidth;
	right = camera->right / tileWidth;
	top = (mapHeight - camera->top) / tileHeight;
	bottom = (mapHeight - camera->bottom) / tileHeight;*/
	left = camera->getLeft() / tileWidth;
	right = camera->getRight() / tileWidth;
	top = (mapHeight - camera->getTop()) / tileHeight;
	bottom = (mapHeight - camera->getBottom()) / tileHeight;

	if (left < 0) {
		left = 0;
	}
	if (top < 0) {
		top = 0;
	}
	if (right >= mapCols) {
		right = mapCols - 1;
	}
	if (bottom >= mapRows) {
		bottom = mapRows - 1;
	}

	/* duyệt các tile cắt camera và vẽ lên màn hình */
	for (int row = top; row <= bottom; row++)
	{
		for (int col = left; col <= right; col++)
		{
			// Vị trí trong world space
			int xW = col * tileWidth;
			int yW = mapHeight - row * tileHeight;

			// Chuyển thành vị trí trên camera
			float xV, yV;
			camera->worldToView(xW, yW, xV, yV);

			// Lấy tile từ tilesheet vẽ lên màn hình
			int chosenTile = matrix[row][col];

			int chosenTileRow = chosenTile / tilesheetCols;
			int chosenTileCol = chosenTile % tilesheetCols;

			int xTileInTilesheet = chosenTileCol * tileWidth;
			int yTileInTilesheet = chosenTileRow * tileHeight;
			RECT rect;
			SetRect(&rect, xTileInTilesheet, yTileInTilesheet, xTileInTilesheet + tileWidth, yTileInTilesheet + tileHeight);
			tilesheet->Draw(xV, yV, &rect);
		}
	}
}
