// Beginning Game Programming, Third Edition
// MyWindows.cpp

#include "GameEngine.h"
#include "WinClass.h"
#include "WinMake.h"
using namespace std;


//************************************************************
//	Function:	Winproc
//	Vars:	hWnd		-	The window handle
//			msg		-	The window message 
//			wParam	-	The upper parameter of the message
//			lParam	-	Lower parameter of the message
//	Process:	This is the windows procedure. It is built to only handle
//			the quit message identified at game over
//	Returns:	None
//************************************************************
LRESULT WINAPI WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
	}
    return DefWindowProc( hWnd, msg, wParam, lParam );
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Game		game;											//Game object
	WinClass	wc (												//Window Class
				game.getAppTitle(),								//	Class Name
				WinProc,										//	Procedure Name
				hInstance);									//	Handle Instance
	WinMaker	win;												//Actual Window
	MSG		msg;												//Used in message loop
	

	if (!win.NewWindow(											//Create Window
					game.getAppTitle(),							//	Title Bar
					game.getScreenW(),							//	Width in pixels
					game.getScreenH(),							//	Height in Pixels
					hInstance,								//	Instance
					nCmdShow									//	Display options
					))
		return 0;

     if (!game.Game_Init(win.getHandle()))							//Initialize the Game object
		return 0;												// exit if failed

	while (!game.isGameOver())									//Main Message loop
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))				//Dispatch msg if exists
		{
			if (msg.message == WM_QUIT)							//Set game over flag on quit
				game.setGameOver(true);

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
										
		game.Game_Run(win.getHandle());							//process game loop 
	}

	return msg.wParam;											//Exit with final message parameter
}

