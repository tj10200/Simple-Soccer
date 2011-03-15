#include <windows.h>


class WinClass
{
	private:
		//initialize window settings
		WNDCLASSEX	wc;
	public:
		WinClass	(string		ClassName,
					 WNDPROC	WinProc,
					 HINSTANCE	hInst) 
		{
			wc.cbSize			= sizeof(WNDCLASSEX); 
			wc.style			= CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc		= WinProc;
			wc.cbClsExtra		= 0;
			wc.cbWndExtra		= 0;
			wc.hInstance		= hInst;
			wc.hIcon			= NULL;
			wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
			wc.lpszMenuName		= NULL;
			wc.lpszClassName	= ClassName.c_str();
			wc.hIconSm			= NULL;
			RegisterClassEx(&wc);
		}
};