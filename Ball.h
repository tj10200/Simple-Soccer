#ifndef BALL_H
#define BALL_H

#include "gameHeaders.h"
#include "Sprite.h"
#include "Timer.h"

class Ball: public Sprite
{
	private:
		double		accel,														//Acceleration of ball
					ballVel,														//Balls Current Velocity
					initVel,														//Initial velocity 
					xDir;														//Direction ball is being kicked
		int			time;														//Used to calc velocity
		float		radius;														//Balls Radius
		Timer		*physicsTimer;													//Timer used in deceleration
		Timer		*kickTimer;													//Prevents ball from being double kicked
		D3DXVECTOR2	target,														//Ball's location (sprite)
					center;														//Ball's center location (object)
		D3DXVECTOR3	slope;														//Used when ball is kicked

		void			calcVelocity	();												//Calculates and updates ballVel & slope
		
	public:
		Ball							(): Sprite()			{						//default Construct
														physicsTimer	=	NULL;	//Init pointed timer
														time			=	0;		//Set time to 0 to prevent random movement
														kickTimer		=	NULL;	//Init pointed timer
														}								
		~Ball						();											//Default deconstruct
		void			updateVelocity		();											//decelerates ball
		
		//Sets and gets for all class variables
		void			setRadius			()					{radius		=	((width  - 2) * scaling) / 2 * .5;}
		double		getAccel			()					{return			accel;}
		void			setAccel			(double		a)		{accel		=	a;}
		double		getRadius			()					{return			radius;}
		
		//Returns the center based off of the sprite location
		D3DXVECTOR2	getBallLocation	()					{D3DXVECTOR2		loc (x + ((width * getScale()) / 2),
																			y + ((height * getScale()) / 2));
														 return			loc;}
		void			setTimer			(int			milis)	{physicsTimer	=	new Timer(milis);}
		void			initTimer			()					{physicsTimer->initClock();
														 time		=	0;
														}
		void			setKickTimer		(int			milis)	{kickTimer	=	new Timer(milis);
														 kickTimer->initClock();
														}
		void			setTarget			(D3DXVECTOR2	tgt);
		void			setCenter			(D3DXVECTOR2	cen)		{center		=	cen;}
		D3DXVECTOR2	getTarget			()					{return			target;}
		D3DXVECTOR2	getCenter			()					{return			getBallLocation();}
		double		getBallVel		()					{return			ballVel;}
		void			setBallVel		(double		vel)		{ballVel		=	vel;}
		double		getInitVel		()					{return			initVel;}
		void			setInitVel		(double		vel)		{initVel		=	vel; ballVel	= initVel;}
		double		getSlope			()					{return			slope.z;}
		void			setSlope			(double		s)		{slope.z		=	s;}
		
		
		//Returns if the minimum time has alapsed between kicks
		bool			ballKickAllowed	()					{if (kickTimer)
															return		kickTimer->isTime();
														 else
															return		true;
														}
		
		//Moves the ball along the slopes path
		void			moveBall			();

		//Attempts to keep the ball within field by bouncing it off of the field lines
		// TODO: Fix timing issue where ball is tested after it crosses the line
		void			reverseYDir		();
		void			reverseXDir		();
		void			reverseYDirTop		();

};

#endif