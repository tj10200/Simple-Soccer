#include "DirectX.h"
#include <iostream>
using namespace std;

//************************************************************
//	Function:	DirectX
//	Vars:	
//	Process:	This function clears all Direct x pointer objects
//			 but does not set up the d3d environment or device
//	Returns:	None
//************************************************************
DirectX::DirectX()
{
	//Direct3D variables
	LPDIRECT3D9			d3d			= NULL; 
	LPDIRECT3DDEVICE9		d3ddev		= NULL; 
	LPDIRECT3DSURFACE9		backbuffer	= NULL;
	LPD3DXSPRITE			spriteobj		= NULL;

	//DirectInput variables
	LPDIRECTINPUT8			dinput		= NULL;
	LPDIRECTINPUTDEVICE8	dimouse		= NULL;
	LPDIRECTINPUTDEVICE8	dikeyboard	= NULL;
}
//************************************************************
//	Function:	DirectX
//	Vars:	hWnd		-	The window handle being used
//			width	-	The width of the window being created
//			height	-	The Height ...
//			fullscreen -	Tells DX if the window will be displayed
//						 in window format or fullscreen
//	Process:	This function intializes all Direct x pointer object
//	Returns:	None
//************************************************************
DirectX::DirectX(HWND hWnd, int width, int height, bool fullScreen)
{
	//Direct3D variables
	LPDIRECT3D9			d3d			= NULL; 
	LPDIRECT3DDEVICE9		d3ddev		= NULL; 
	LPDIRECT3DSURFACE9		backbuffer	= NULL;
	LPD3DXSPRITE			spriteobj		= NULL;

	//DirectInput variables
	LPDIRECTINPUT8			dinput		= NULL;
	LPDIRECTINPUTDEVICE8	dimouse		= NULL;
	LPDIRECTINPUTDEVICE8	dikeyboard	= NULL;

	Direct3D_Init(hWnd, width, height, fullScreen);
}
//************************************************************
//	Function:	Direct3D_Init
//	Vars:	window	-	The window handle being used
//			width	-	The width of the window being created
//			height	-	The Height ...
//			fullscreen -	Tells DX if the window will be displayed
//						 in window format or fullscreen
//	Process:	This draws from a source surface area to a specific
//			 locatio in the destination
//	Returns:	None
//************************************************************
bool DirectX::Direct3D_Init(HWND window, int width, int height, bool fullscreen)
{
	D3DPRESENT_PARAMETERS d3dpp;											//Used for screen presentation 
    
	d3d = Direct3DCreate9(D3D_SDK_VERSION);									//initialize Direct3D
    
	if (!d3d) return false;												//Return if d3d creation failed

	ZeroMemory(&d3dpp, sizeof(d3dpp));										//set Direct3D presentation parameters
	d3dpp.hDeviceWindow					= window;							//Window Handle
	d3dpp.Windowed						= (!fullscreen);					//Advise if windowed/fullscreen
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;				//
	d3dpp.EnableAutoDepthStencil			= 1;
	d3dpp.AutoDepthStencilFormat			= D3DFMT_D24S8;
	d3dpp.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.BackBufferFormat				= D3DFMT_X8R8G8B8;					//Advise format for back buffer
	d3dpp.BackBufferCount				= 1;								//Initialize number of back buffers
	d3dpp.BackBufferWidth				= width;							//Width of buffer in pixels
	d3dpp.BackBufferHeight				= height;							//Height of buffer in pixels

	d3d->CreateDevice					(								//create Direct3D device
										D3DADAPTER_DEFAULT,				//Adapter 
										D3DDEVTYPE_HAL,				//Device Type
										window,						//Window Handle
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,//graphic processing option
										&d3dpp,						//Presentation parameters
										&d3ddev						//Device to assign memory space
									);
	if (!d3ddev)														//Return if initialization failes
		return false;

	d3ddev->GetBackBuffer				(								//Fill backbuffer memory space
										0,							//	Index of backbuffer
										0,							//	Type of buffer
										D3DBACKBUFFER_TYPE_MONO,			//	
										&backbuffer					//	Pointer to fill
									);

	D3DXCreateSprite					(							//Create sprite object
										d3ddev,						//	Device
										&sprite						//	Sprite obj
										);

	D3DXCreateSprite					(							//Create sprite object
										d3ddev,						//	Device
										&fontSprite					//	Sprite obj
										);
	
    	
	d3ddev->Clear						(								//Clear screen
										0,							//	Areas to clear
										NULL,						//	array of Rect to clear
										D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,//	Which surface to clear
										D3DCOLOR_XRGB(0,0,100),			//	Color to clear with
										1.0f,						//	value to clear z buffer to. 1 = farthest
										0							//	Clear stencil buffer to
										);

    return true;
}



//************************************************************
//	Function:	DrawSurface
//	Vars:	dest	-	The destination surface to draw to
//			x	-	The location in the destination to draw to
//			y	
//			source -	The source image to draw from
//	Process:	This draws from a source surface area to a specific
//			 locatio in the destination
//	Returns:	None
//************************************************************
void DirectX::DrawSurface(
							LPDIRECT3DSURFACE9	dest,
							float			x, 
							float			y, 
							LPDIRECT3DSURFACE9	source
					)
{
    D3DSURFACE_DESC				desc;									//Describes surface information
    source->GetDesc			(	&desc);									//get width/height data from source

    RECT						source_rect	=	{						//Source rect to use in drawing
												0,					//	x position
												0,					//	y position
												desc.Width,			//	image width
												desc.Height			//	image height
											};					    
    RECT						dest_rect		=	{						//Dest rect to use in drawing
												x,					//	x position to draw to
												y,					//	y position to draw to		
												x+desc.Width,			//	image width
												y+desc.Height			//	image height
											};				
   
    d3ddev->StretchRect		(											//Draw from source using source rect
																	// Size to dest using dest rect size
							source,									//	source texture
							&source_rect,								//	source size
							dest,									//	dest texture
							&dest_rect,								//	dest size
							D3DTEXF_NONE								//	No Texture filter								
						);
}


//************************************************************
//	Function:	LoadSurface
//	Vars:	filename	-	The path of a picture to load
//	Process:	This function Loads the image into a D3D surface
//			 pointer object. Used for background
//	Returns:	None
//************************************************************
LPDIRECT3DSURFACE9 DirectX::LoadSurface(string filename)
{
    LPDIRECT3DSURFACE9	image = NULL;
    D3DXIMAGE_INFO		info;
    
    result = D3DXGetImageInfoFromFile(filename.c_str(), &info);					//Load image information
    
    if (result != D3D_OK)												//Exit if load failed
        return NULL;

    result = d3ddev->CreateOffscreenPlainSurface(								//create surface
        info.Width,														//	width of the surface
        info.Height,													//	height of the surface
        D3DFMT_X8R8G8B8,													//	surface format
        D3DPOOL_DEFAULT,													//	memory pool to use
        &image,														//	pointer to the surface
        NULL);															//	reserved (always NULL)

    if (result != D3D_OK) return NULL;

    
    result = D3DXLoadSurfaceFromFile(										//load surface from file into 
																	// newly created surface
        image,															//	destination surface
        NULL,															//	destination palette
        NULL,															//	destination rectangle
        filename.c_str(),												//	source filename
        NULL,															//	source rectangle
        D3DX_DEFAULT,													//	controls how image is filtered
        D3DCOLOR_XRGB(0,0,0),												//	for transparency (0 for none)
        NULL);															//	source image info (usually NULL)

    if (result != D3D_OK)												//Exit if surface creation failed
		return NULL;

    return image;														//Return surface
}


//************************************************************
//	Function:	LoadTexture
//	Vars:	filename	-	The path of a picture to load
//			transcolor-	The transparancy color of the texture
//	Process:	This function Loads the image into a D3D texture
//			 pointer object
//	Returns:	None
//************************************************************
LPDIRECT3DTEXTURE9 DirectX::LoadTexture(string filename, D3DCOLOR transcolor)
{  
	LPDIRECT3DTEXTURE9	texture	= NULL;									//holds texture object data
	D3DXIMAGE_INFO		info;											//Holds actual image data
	result = D3DXGetImageInfoFromFile(filename.c_str(), &info);			//Load file data into info struct
	
	if (result != D3D_OK) 
		return NULL;										//Exit if load failed

	//create the new texture by loading a bitmap image file
	result = D3DXCreateTextureFromFileEx( 
        d3ddev,														//Direct3D device object
        filename.c_str(),											//bitmap filename
        info.Width,													//bitmap image width
        info.Height,												//bitmap image height
        1,															//mip-map levels (1 for no chain)
        D3DPOOL_DEFAULT,											//the type of surface (standard)
        D3DFMT_UNKNOWN,												//surface format (default)
        D3DPOOL_DEFAULT,											//memory class for the texture
        D3DX_DEFAULT,													//image filter
        D3DX_DEFAULT,													//mip filter
        D3DCOLOR_XRGB(255,0,255),														//color key for transparency
        &info,															//bitmap file info (from loaded file)
        NULL,															//color palette
        &texture );														//destination texture

	if (result != D3D_OK) return NULL;										//make sure the textre was 
																	// loaded correctly
	return texture;
}

//************************************************************
//	Function:	Sprite_Draw_Frame
//	Vars:	texture	-	The texture object ot paint on
//						 the screen
//			destx	-	The location to draw the object
//			desty	
//			framenum	-	The frame of the sprite sheet
//			framew	-	frame width
//			frameh	-	frame height
//			columns	-	The number of frames horizontally
//						 across the sprite sheet
//	Process:	This function returns the current horizontal 
//			 point.
//	Returns:	None
//************************************************************
void DirectX::Sprite_Draw_Frame(
							LPDIRECT3DTEXTURE9	texture, 
							int				destx, 
							int				desty, 
							int				framenum, 
							int				framew, 
							int				frameh, 
							int				columns)
{
	D3DXVECTOR3	dest (												//Set up translation vector
							(float)destx,	
							(float)desty, 
							0 
					);
	D3DCOLOR	white	=	D3DCOLOR_XRGB(255,255,255);			//Used to maintain texture colors
	RECT		rect;										//Describes area to draw from texture	

	rect.left			=	(framenum % columns) * framew;				//Set up rectangle points
	rect.top			=	(framenum / columns) * frameh;
	rect.right			=	rect.left + framew;
	rect.bottom			=	rect.top + frameh;

	sprite->Draw(														//Draw sprite
							texture,									//	Texture to draw from
							&rect,										//	Location on texture to draw
							NULL,										//	Used for 3d translations
							&dest,										//	Destination screen location
							white										//	White preserves texture colors
				);
}				

void DirectX::Sprite_Transform_Draw(LPDIRECT3DTEXTURE9	img,	float	scaling,	int		xVal,		int	yVal, 
									float				rotation,	int		width,		int	height,		int curFrame,
									int					columns,	D3DCOLOR  transColor)
{
	float xScale	=		scaling;									//Horizontal scaling value
	float yScale	=		scaling < 0?scaling	*	-1:	scaling;		//Vertical scaling value, force positive
    
	D3DXVECTOR2				scale(		xScale,			yScale);		//Scaling vector
    D3DXVECTOR2 trans		(			xVal,			yVal);			//Translation vector
    D3DXVECTOR2 center		((float)(	width		*	yScale) / 2,	//Vector that indicates the center of the image.
							((float)(	height		*	yScale) / 2));	// used in rotations
	D3DXMATRIX	mat;													//Actual Transformation matrix
    
	D3DXMatrixTransformation2D (										//Translate to new vectorspace
										&mat,							//Matrix
										NULL,							//Center of scaled matrix
										0,								//Rotated vector of scaled matrix							
										&scale,							//Scale vector
										&center,						//Center vector
										rotation,						//Amount of rotation in radians
										&trans							//Translation vector
								);
   
	sprite->SetTransform		(		&mat );							//tell sprite object to use the transform

    int			fx	=			(		curFrame	%	columns)		//Find horizontal edge of frame within texture
													*	width;									
    int			fy	=			(		curFrame	/	columns)		//Find vertical edge of frame within texture 
													*	height;
    RECT		srcRect =		{										//Rectangle of source image
										fx,								//left pixle
										fy,								//Top pixel
										fx + width,						//Right pixel
										fy + height						//Bottom pixel
								};
	
	sprite->Draw				(										//draw the sprite frame
										img,							// Texture
										&srcRect,						// where in texture to draw from
										NULL,							// Mirror vector
										NULL,							// 
										transColor							// Transparancy color
								);
	
}

//************************************************************
//	Function:	DirectInput_Init
//	Vars:	None
//	Process:	This function returns the current horizontal 
//			 point.
//	Returns:	true
//************************************************************
bool DirectX::DirectInput_Init(HWND hwnd)
{
    
    DirectInput8Create(											//initialize DirectInput object
					GetModuleHandle(NULL),						//	current window handle
					DIRECTINPUT_VERSION,						//	Dinput version
					IID_IDirectInput8,							//	DInput Class
					(void**)&dinput,							//	Return variable of function	
					NULL
				  );

    dinput->CreateDevice(										//initialize the keyboard
					 GUID_SysKeyboard,							//	Describes keyboard object
					 &dikeyboard,								//	Return variable of function
					 NULL
					);
    dikeyboard->SetDataFormat(									//Sets locale format of keyboard
					 &c_dfDIKeyboard
					);
    dikeyboard->SetCooperativeLevel(								//Sets window access object
					 hwnd, 
					 DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
					);
    dikeyboard->Acquire();										//Fills initial keyboard data

    dinput->CreateDevice(										//initialize the mouse
					GUID_SysMouse,								//	Mouse interface	
					&dimouse,									//	Return variable for mouse
					NULL
					);
    dimouse->SetDataFormat(										//Format mouse object
					&c_dfDIMouse
					);
    dimouse->SetCooperativeLevel(									//Sets window access object
					hwnd, 
					DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
					);
    dimouse->Acquire	();
    d3ddev->ShowCursor  (false);									//Hides the cursor

    return true;
}

void				MatrixLookAt2D		(	D3DXVECTOR2		src,
											D3DXVECTOR2		dest)
{
	
	
	
}

//************************************************************
//	Function:	DirectInput_Update
//	Vars:	None
//	Process:	This function Aquires the mouse and keyboard
//			 input data, the data is stores into mouse_state
//			 and keys.
//	Returns:	None
//************************************************************
void DirectX::DirectInput_Update()
{
    //update mouse
    dimouse->Poll();
    if (!SUCCEEDED(dimouse->GetDeviceState(sizeof(DIMOUSESTATE),&mouse_state)))
    {
        //mouse device lose, try to re-acquire
        dimouse->Acquire();
    }

    //update keyboard
    dikeyboard->Poll();
    if (!SUCCEEDED(dikeyboard->GetDeviceState(256,(LPVOID)&keys)))
    {
        //keyboard device lost, try to re-acquire
        dikeyboard->Acquire();
    }
}

//************************************************************
//	Function:	Mouse_X()
//	Vars:	None
//	Process:	This function returns the current horizontal 
//			 point.
//	Returns:	Mouse state
//************************************************************
int	 DirectX::Mouse_X()
{
    return mouse_state.lX;
}

//************************************************************
//	Function:	Mouse_Y()
//	Vars:	None
//	Process:	This function returns the current horizontal 
//			 point.
//	Returns:	Mouse state
//************************************************************
int  DirectX::Mouse_Y()
{
    return mouse_state.lY;
}

//***********************************************************
//	Function:	Mouse_Button
//	Vars:	button	- the mouse button pressed
//	Process:	Retuns the mouse_state structure variable
//			 indicating if the passed button was pressed
//**********************************************************
int  DirectX::Mouse_Button(int button)
{
    return mouse_state.rgbButtons[button] & 0x80;
}

//**********************************************************
//	Function:	Key_Down
//	Vars:	key		-	a keyboard key
//	Process:	Returns if the key pressed is defined within the 
//			 standard set of keys
//***********************************************************
bool DirectX::Key_Down(int key) 
{
	return (bool)(keys[key] & 0x80);
}
//*********************************************************
//	Function:	DirectX
//	Vars:	None
//	Process:	Destroys all DirectX objects during destruction
//	Returns:	None
//**********************************************************
DirectX::~DirectX()
{
	if (sprite) 
		sprite->Release();
	
	if(fontSprite)
		fontSprite->Release();
    
	if (backbuffer)
		backbuffer->Release();							//Pointer to Backbuffer
	
	if (dinput)
		dinput->Release();

	if (dimouse)
		dimouse->Release();
	
	if (dikeyboard)
		dikeyboard->Release();


	if (d3ddev) 
		d3ddev->Release();
	if (d3d) 
		d3d->Release();

			
}

LPD3DXFONT DirectX::MakeFont(string name, int size)
{
    LPD3DXFONT font = NULL;

    D3DXFONT_DESC desc = {
        size,                   //height
        0,                      //width
        0,                      //weight
        0,                      //miplevels
        false,                  //italic
        DEFAULT_CHARSET,        //charset
        OUT_TT_PRECIS,          //output precision
        CLIP_DEFAULT_PRECIS,    //quality
        DEFAULT_PITCH,          //pitch and family
        ""                      //font name
    };

    strcpy(desc.FaceName, name.c_str());

    D3DXCreateFontIndirect(d3ddev, &desc, &font);

    return font;
}

void DirectX::FontPrint(LPD3DXFONT font, int x, int y, string text, D3DCOLOR color)
{
    //figure out the text boundary
    RECT rect = { x, y , 200, 200};
    
    D3DXVECTOR3 dest (100, 100, -100);

    font->DrawText( NULL, text.c_str(), text.length(), &rect, DT_CALCRECT, color); 

    //print the text
    font->DrawText(NULL, text.c_str(), text.length(), &rect, DT_CENTER | DT_NOCLIP |DT_TOP, color); 

}