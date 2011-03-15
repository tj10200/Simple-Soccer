#include "PlayField.h"
Field::Field()
{
	double		xLoc = (int)FIELD_WIDTH	/ 2 - 25,
				yLoc	= (int)FIELD_HEIGHT	/ 2 - 25;

	regions			= NULL;				
	
	Goals		=	new D3DXVECTOR2[2];
	Goals[0].x	=	55;						//Left Goals
	Goals[0].y	=	308;		
	Goals[1].x	=	1179;					//Right Goals
	Goals[1].y	=	308;
	ball			=	new Ball ();
	ball->setX		(xLoc);
	ball->setY		(yLoc);
	ball->setScale		(.7f);
	ball->setRotation	(0);
	ball->setWidth		(50);
	ball->setHeight	(50);
	ball->setFrame		(0);
	ball->setStartFrm	(0);
	ball->setEndFrm	(0);
	ball->setColumns	(1);
	ball->setVelY		(0);
	ball->setVelX		(0);
	ball->setRadius	();
	ball->setAccel		(-.1);
	ball->setInitVel	(0);
	ball->setBallVel	(0);
	ball->setTimer		(200);												//Set to 1 second for acceleration decay
	ball->setTarget	(D3DXVECTOR2 ((float)xLoc + 25, (float)yLoc + 25));
	ball->setCenter	(D3DXVECTOR2 ((float)xLoc + 25, (float)yLoc + 25));
	ball->setKickTimer	(10);
	
	InitRegions		();

	inPlayTimer		= new Timer(100);
	gameOn			= false;
}
void Field::InitRegions	()
{
	regions	=	new D3DXVECTOR2 *[REG_ROWS];


	for (int w = 0; w < REG_ROWS; w++)
	{
		regions[w]		=	new D3DXVECTOR2[REG_COLS];
		for (int z = 0; z < REG_COLS; z ++)
		{
			regions[w][z].x = 50 + (z * REG_WIDTH);
			regions[w][z].y = 50 + (w * REG_HEIGHT);
		}
	}
}
void	Field::updateBall()
{	
	if (ball->getBallVel() > 0)
		ball->moveBall();
	
}
bool Field::IsBallInField()
{
	bool		isInField		=	true;
	double	ballX		=	ball->getX(),
			ballY		=	ball->getY(),
			ballWidth		=	ball->getWidth()	*	ball->getScale(),
			ballHeight	=	ball->getHeight()	*	ball->getScale();
	
	if ((ballX + ballWidth + 50)	>	FIELD_WIDTH  + 50	||			//give extra padding on x regions 
	     ballX				<	50)								// so goals can be picked up
	{
	    ball->reverseXDir();

	    if ((ballX + ballWidth + 50)	> FIELD_WIDTH + 70	||
		    ballX					< 30)
			isInField		=	false;
	}
	if ((ballY + ballHeight)			> (FIELD_HEIGHT + 50))
	{
		ball->reverseYDirTop();

		if ((ballY + ballHeight)		> (FIELD_HEIGHT + 70))
			isInField		=	false;
	}

	if (ballY						< 50)
	{	
	    ball->reverseYDir();

	    if (ballY					< 30)
			isInField		=	false;
	}


	return isInField;
}

void	Field::resetBall()
{
	ball->setX			((int)FIELD_WIDTH	/ 2 - ball->getWidth()	/ 2);
	ball->setY			((int)FIELD_HEIGHT	/ 2 - ball->getHeight()  / 2);
	ball->setCenter		(D3DXVECTOR2 (ball->getX() + ball->getWidth() / 2, 
								    ball->getY() + ball->getHeight() / 2));
	ball->setVelX			(0);
	ball->setVelY			(0);
	ball->setInitVel		(0);
	ball->setBallVel		(0);
	ball->setTarget		(ball->getCenter());
	ball->initTimer		();

	gameOn			=	false;
}

Field::~Field()
{
	if (regions)
	{	
		for (int w = 0; w < REG_ROWS; w++)
			delete [] regions[w];

		delete [] regions;
	}

	if (Goals)
		delete [] Goals;					
		
	if (ball)
		delete ball;
	
	if (inPlayTimer)
		delete inPlayTimer;

}