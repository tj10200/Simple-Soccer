#include <windows.h>

class WinMaker
{
	private:
		HWND hWnd;
		void Show		(int		nCmdShow);
	public:
		WinMaker (){}
		bool NewWindow	(string		WinName, 
						 int		Width, 
						 int		Height, 
						 HINSTANCE	hInst,
						 int		nCmdShow);
		HWND getHandle  ()						{return hWnd;}

	
};

bool WinMaker::NewWindow (string WinName, int Width, int Height, HINSTANCE hInst, int nCmdShow)
{
	hWnd = CreateWindow (WinName.c_str(), 
						 WinName.c_str(),
						 WS_OVERLAPPEDWINDOW,
						 CW_USEDEFAULT,
						 CW_USEDEFAULT,
						 Width,
						 Height,
						 NULL,
						 NULL,
						 hInst,
						 NULL);
	if (hWnd == 0) 
		return false;
	else
		Show(nCmdShow);
	
	return true;
}

void WinMaker::Show		(int		nCmdShow)
{
	//display the window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
}