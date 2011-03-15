#include "Ball.h"

void	Ball::setTarget (D3DXVECTOR2	tgt)		
{	
	if (tgt.x		>	center.x)
	{	
		slope.y	=	tgt.y	-	center.y;
		slope.x	=	tgt.x	-	center.x;
		xDir		=	1;
	}
	else if (tgt.x <	center.x)
	{
		slope.y	=	center.y	-	tgt.y;
		slope.x	=	center.x	-	tgt.x;
		xDir		=	-1;
	}
	else if (tgt.x	==	center.x)
	{
		slope.x	=	.001;
		slope.y	=	tgt.y	-	center.y;
		xDir		=	0;
	}
	slope.z		=	slope.y	/	slope.x;

	target		=	tgt;

}
void	Ball::updateVelocity()					
{

	if (physicsTimer->isTime())
	{
		time++;
		ballVel		=	initVel		+	.5	* accel * (time * time);
				
		if (ballVel	<	0)
		{
			time		=	0;
			ballVel	=	0;
			target.x	=	center.x;
			target.y	=	center.y;
			physicsTimer->initClock();
		}
	}
}



//**********************************************************
//	Function: moveBall
//	Vars:	None
//	Process:	Moves the ball toward the target vector in 
//			accordance to the slope away from the target
void Ball::moveBall()
{
		
	updateVelocity();

	velx		=	ballVel * xDir;
	vely		=	ballVel * xDir * slope.z;

	if (vely	>	ballVel)
		vely	=	ballVel;
	else if (vely < -ballVel)
		vely =	-ballVel;

	x			+= velx;										//Move all player calculable vectors
	center.x		+= velx;
	target.x		+= velx;

	y			+= vely;
	center.y		+= vely;
	target.y		+= vely;
}

void	Ball::reverseYDir ()
{	
	slope.z		     = -(1.0f / slope.z);
}

void	Ball::reverseYDirTop()
{	
	slope.z		     = (1.0f / slope.z);
	setY				  (getY() - 50);
}


void Ball::reverseXDir ()
{	xDir				*= -1;}

Ball::~Ball()
{

	if (physicsTimer)
		delete physicsTimer;

	if (kickTimer)
		delete kickTimer;

}