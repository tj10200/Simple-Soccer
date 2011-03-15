#ifndef  SPRITE_H
#define  SPRITE_H

#include "GameHeaders.h"
#include "DirectX.h"

class Sprite
{
	protected:
		float		x,													//Screen horizontal location
					y;													//Screen vertical location
		int			frame,												//current frame in sheet
					columns,												//Number of frame columns in sheet
					width,												//Frame width
					height;												//Frame height
		float		scaling,												//Relative size to output
					rotation;												//How much rotation to output
		int			startframe,											//Beginning frame in sheet
					endframe,												//Ending frame in sheet
					starttime,											//Used to track animation
					delay,												//Used to delay animation
					direction;											//Direction object is moving
		float		velx,												//horizontal speed of sprite
					vely;												//vertical speed of sprite
		float		maxVel;												//Maximum speed of sprite

		D3DCOLOR	color;													//Mask color in sheet
		LPDIRECT3DTEXTURE9 spriteImg;											//Image for sprite render
		
	public:
		Sprite		();
		Sprite		(float			xVal, 
					 float			yVal, 
					 int			widthVal, 
					 int			heightVal,
					 LPDIRECT3DTEXTURE9 img);
		Sprite		(float			xVal, 
					 float			yVal, 
					 int			frameVal, 
					 int			colVal, 
					 int			widthVal, 
					 int			heightVal, 
					 float			scaleVal, 
					 float			rotateVal,
					 int			startFrameVal,
					 int			endFrameVal,
					 int			delayVal,
					 int			directVal,
					 float			xVel,
					 float			yVel,
					 D3DCOLOR		transColor);
		Sprite		(float			xVal, 
					 float			yVal, 
					 int			frameVal, 
					 int			colVal, 
					 int			widthVal, 
					 int			heightVal, 
					 int			startFrameVal,
					 int			endFrameVal,
					 int			delayVal,
					 int			directVal,
					 float			xVel,
					 float			yVel,
					 D3DCOLOR		colorVal);

		float		 getX			()					{return		x;}
		float		 getY			()					{return		y;}
		int			 getFrame		()					{return		frame;}
		int			 getColumn		()					{return		columns;}
		int			 getWidth		()					{return		width;}
		int			 getHeight		()					{return		height;}
		float		 getScale		()					{return		scaling;}
		float		 getRotation	()					{return		rotation;}
		int			 getStartFrame	()					{return		startframe;}
		int			 getEndFrame	()					{return		endframe;}
		int			 getStarttime	()					{return		starttime;}
		int			 getDelay		()					{return		delay;}
		int			 getDirection	()					{return		direction;}
		float		 getVelX		()					{return		velx;}
		float		 getVelY		()					{return		vely;}
		D3DCOLOR	 getColor		()					{return		color;}
		LPDIRECT3DTEXTURE9 getTexture()					{return		spriteImg;}

		void		 setX			(float		newX)		{x			=	newX;}
		void		 setY			(float		newY)		{y			=	newY;}
		void		 setFrame		(int		newFrame)	{frame		=	newFrame;}
		void		 setScale		(float		newScale)	{scaling	=	newScale;}
		void		 setRotation	(float		newRot)		{rotation	=	newRot;}
		void		 setStartFrm	(int		newFrame)	{startframe =	newFrame;}
		void		 setEndFrm		(int		newFrame)	{endframe	=	newFrame;}
		void		 setStartTime	(int		newTime)	{starttime  =	newTime;}
		void		 setDelay		(int		newDelay)	{delay		=	newDelay;}
		void		 setDirect		(int		newDirect)	{direction  =	newDirect;}
		void		 setVelX		(float		newVel)		{velx		=	newVel;}
		void		 setVelY		(float		newVel)		{vely		=	newVel;}
		void		 setColor		(D3DCOLOR	col)		{color		=	col;}
		void		 setTexture		(LPDIRECT3DTEXTURE9 img) {spriteImg	=	img;}
		void		 setWidth		(int		newWidth)	{width		=	newWidth;}
		void		 setHeight		(int		newHeight)	{height		=	newHeight;}
		void		 setColumns		(int		newColVal)	{columns	=	newColVal;}
		void		 Sprite_Init	(LPD3DXSPRITE s);							//Initialize pointers
		void		 Sprite_Animate	();											//Animation function
		void		 Sprite_Transforms (LPDIRECT3DTEXTURE9 image);				//Perform matrix transforms
		bool		 Collision		(Sprite	s);									//Test if collided with another sprite
};
#endif 

