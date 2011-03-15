#ifndef SPRITE_CPP
#define SPRITE_CPP

#include "Sprite.h"

//Default Constructor
Sprite::Sprite()
{
	frame			= 1;
	columns			= 1;
	width = height  = 1;
	scaling			= 1.0f;
	rotation		= 0.0f;
	startframe		= endframe = 1;
	direction		= 1;
	starttime		= delay = 1;
	velx			= vely  = 0.0f;
	color			= D3DCOLOR_XRGB (255,255,255);

}
//Basic Non Animated Sprite Constructor
Sprite::Sprite (float xVal, float yVal, int widthVal, int heightVal, LPDIRECT3DTEXTURE9 img)
{
	frame			= 0;
	columns			= 1;
	scaling			= 1.0f;
	rotation		= 0.0f;
	startframe		= endframe = 0;
	direction		= 1;
	starttime		= delay = 0;
	velx			= vely  = 0.0f;
	color			= D3DCOLOR_XRGB (255,255,255);

	x				= xVal;
	y				= yVal;
	width			= widthVal;
	height			= heightVal;
	
	spriteImg		= img;
}

//Animated Sprite Constructor with scaling and rotation 
Sprite::Sprite (float		xVal, 
				float		yVal, 
				int			frameVal, 
				int			colVal, 
				int			widthVal, 
				int			heightVal, 
				float		scaleVal, 
				float		rotateVal,
				int			startFrameVal,
				int			endFrameVal,
				int			delayVal,
				int			directVal,
				float		xVel,
				float		yVel,				
				D3DCOLOR	colorVal)
{

	x			= xVal;
	y			= yVal;
	frame		= frameVal;
	columns		= colVal;
	width		= widthVal;
	height		= heightVal;
	scaling		= scaleVal;
	rotation	= rotateVal;
	startframe	= startFrameVal;
	endframe	= endFrameVal;
	delay		= delayVal;
	direction	= directVal;
	velx		= xVel;
	vely		= yVel;
	color		= colorVal;

}

//Animated Sprite Constructor with scaling and rotation 
Sprite::Sprite (float		xVal, 
				float		yVal, 
				int			frameVal, 
				int			colVal, 
				int			widthVal, 
				int			heightVal, 
				int			startFrameVal,
				int			endFrameVal,
				int			delayVal,
				int			directVal,
				float		xVel,
				float		yVel,
				D3DCOLOR	colorVal)
{

	x			= xVal;
	y			= yVal;
	frame		= frameVal;
	columns		= colVal;
	width		= widthVal;
	height		= heightVal;
	startframe	= startFrameVal;
	endframe	= endFrameVal;
	delay		= delayVal;
	direction	= directVal;
	velx		= xVel;
	vely		= yVel;
	color		= colorVal;
	

}

void Sprite::Sprite_Animate()
{
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		frame += direction;
		if (frame > endframe) 
			frame = startframe;
		if (frame < startframe) 
			frame = endframe;
	}	
}


//create sprite object
/*
int	 Sprite::Collision (Sprite s)
{
	RECT rect1;
	rect1.left		= this->x;
	rect1.top		= this->x;
	rect1.right		= this->x + this->width	 * this->scaling;
	rect1.bottom	= this->y + this->height * this->scaling;

	RECT rect2;
	rect2.left		= (long)s.getX();
	rect2.top		= (long)s.getY();
	rect2.right		= (long)s.getX() + s.getWidth()	 * s.getScale();
	rect2.bottom	= (long)s.getY() + s.getHeight() * s.getScale();

	RECT dest;
	return IntersectRect (&dest, &rect1, &rect2);
}
*/
#endif
