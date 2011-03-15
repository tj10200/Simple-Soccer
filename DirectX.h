#ifndef DIRECTX_H
#define DIRECTX_H

#include "GameHeaders.h"

//macro to detect key presses
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

class DirectX
{
	private:
		//Direct3D objects
		LPDIRECT3D9				d3d;								//Direct 3d Base object
		LPDIRECT3DDEVICE9		d3ddev;								//D3d Device
		LPDIRECT3DSURFACE9		backbuffer;							//Pointer to Backbuffer
		LPD3DXSPRITE			sprite;								//Used for sprites
		LPD3DXSPRITE			fontSprite;							//Used for Debug font
	
		
		//DirectInput objects, devices, and states
		LPDIRECTINPUT8			dinput;								//All Input object base
		LPDIRECTINPUTDEVICE8	dimouse;								//Mouse object pointer
		LPDIRECTINPUTDEVICE8	dikeyboard;							//Keyboard pointer
		DIMOUSESTATE			mouse_state;							//Current mouse state
		char					keys[256];							//Store Ascii data used in 
																// registering keystrokes
		HRESULT				result;								//Used to measure if proces worked



	public:
		DirectX							();								//Default contructor
		DirectX							(								//Constructor
											HWND					hWnd,	// Window handle
											int					width,	// Screen Width
											int					height,	// Screen Height
											bool					fullScreen // is full screen set
										);
		~DirectX							();								//Deconstructor
		LPD3DXSPRITE			getSprite		()	{return				sprite;}	//Get Sprite Pointer
		LPDIRECT3DDEVICE9		getDevice		()	{return				d3ddev;}	//Get Device Pointer
		LPDIRECT3DSURFACE9		getBackBuffer	()	{return				backbuffer;}//Get Back Buffer pointer
		
		//Direct3D functions
		bool					beginDev		()	{return				(d3ddev->BeginScene());} //Start scene rendering
		void					endDev		()	{							//End Scene Rendering
											 d3ddev->EndScene(); 
											 d3ddev->Present(NULL,NULL,NULL,NULL);
										}
		bool					Direct3D_Init	(								//Initialize d3d objects
											HWND					hwnd,	//	Window handle
											int					width,	//	screen width
											int					height,	//	screen height
											bool					fullscreen //	is full screen set
										);
		void				Direct3D_Shutdown	();								//Destroys pointers
		LPDIRECT3DSURFACE9	LoadSurface		(	string				filename);//Loads a surface into the D3D object

		void				DrawSurface		(								//Draws a surface into the buffer
											LPDIRECT3DSURFACE9		dest, 
											float				x, 
											float				y, 
											LPDIRECT3DSURFACE9		source);
		LPDIRECT3DTEXTURE9	LoadTexture		(	string				filename, //Loads and returns a texture
											D3DCOLOR				transcolor = D3DCOLOR_XRGB(255,0,255));
		void				Sprite_Draw_Frame	(								//Draws a sprite as it currently is
											LPDIRECT3DTEXTURE9		texture, 
											int					destx,	
											int					desty, 
											int					framenum, 
											int					framew, 
											int					frameh, 
											int					columns);
		void				Sprite_Transform_Draw(								//Transforms a sprite and draws it		
											LPDIRECT3DTEXTURE9	img,			// sprite texture
											float				scaling,	// scales sprite by some value
											int					xVal,	// paints to coordinates
											int					yVal, 
											float				rotation,	// rotates image before drawing
											int					width,	// indicates the sprite width 
											int					height,	//  and height
											int					curFrame, // current frame to draw
											int					columns,	// columns in sprite sheet
											D3DCOLOR			transColor);	// transparant color in sprite sheet
		void				MatrixLookAt2D		(	D3DXVECTOR2		src,			//Forces the camera to look as a point in 2d space
											D3DXVECTOR2		dest);

		//DirectInput functions
		bool				DirectInput_Init	(	HWND					hWnd);	//Initializes all pointers
		void				DirectInput_Update	();								//Detects input and updates pointers
		void				DirectInput_Shutdown();								//Destroys pointers
		bool				Key_Down			(	int					keyPressed);//tests if a key was pressed
		int				Mouse_Button		(	int					btnPressed);//Tests if a mouse button was pressed
		int				Mouse_X			();								//Returns mouse coordinates
		int				Mouse_Y			();	
		void				BeginSpriteDraw	()	{sprite->Begin(D3DXSPRITE_ALPHABLEND);}//Locks the buffer in order to draw the sprite object
		void				EndSpriteDraw		()	{sprite->End();}				//Releases a previously set up lock
		void				ClearScene		()	{d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,100), 1.0f, 0);}
		LPD3DXFONT		MakeFont			(	string			name,		//Creates a font object
											int				size);
		void				FontPrint			(	LPD3DXFONT		font,		//Prints the font to the screen
											int				x, 
											int				y, 
											string			text, 
											D3DCOLOR color);
		
};	

#endif