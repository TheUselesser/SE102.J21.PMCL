#include <windows.h>
#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) 
{
	MSG msg;
	bool done = false;

	Game myGame;
	myGame.init(hInstance);

	DWORD start_time = GetTickCount();

	// vòng lặp chính
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = true;
				myGame.end();
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Game loop
		myGame.run(hInstance);
	}

	return msg.wParam;
}