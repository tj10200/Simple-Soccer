#ifndef PLAYER_H
#define PLAYER_H

#include "Sprite.h"
#include "Ball.h"
#include "Timer.h"

enum	PLAYER_STATE
{
	ATTACK,
	DEFEND,
	SUPPORT,
	GOALIE,
	AT_TARGET,
	WAIT,
	DRIBBLE_BALL,
	KICK_BALL,
	PURSUE_TGT,
	USER_CONTROL
};


class Player: public Sprite
{
	private:
		PLAYER_STATE	state;										//Defines the player behavior
		int			fieldRegion;									//The players starting position 
				 												// based on state
		D3DXVECTOR2	location;										//Current location on field
		D3DXVECTOR2	center;
		D3DXVECTOR2	futureLocation;								//Future location 
		D3DXVECTOR2	target;										//Location of target currently pursuing
		D3DXVECTOR2	face;										//Location indicating face of player
		D3DXVECTOR2	kickTarget;									//Used when kicking the ball
		double		playerVel,									//real player speed
					maxVel;										//Max player speed
		bool			hasBall;										//Indicates player has ball
		bool			inHomeRegion;									//Indicated that player is in home location
		bool			atTarget;										//Used to determine when at tracked target
		double		radius;
		double		tgtRadius;
		double		priorTgtDist;									//Used to set heading on recalc
		Ball			*ball;										//Pointer to ball, used during kicks and dribbling
		Timer		*ballKickTimer;
		

	public:
		Player						()				{priorTgtDist	= 0;
													 ball		= NULL;
													 ballKickTimer = NULL;
													}
		~Player						();

		PLAYER_STATE	getPlayerState		()				{return		state;}
		void			setPlayerState		(PLAYER_STATE	s)	{state		=	s;}
		int			getRegion			()				{return		fieldRegion;}
		double		getRadius			()				{return		radius;}
		void			setRadius			(double		r)	{radius		= r;}
		void			setFieldReg		(int			r)	{fieldRegion	=	r;}
		D3DXVECTOR2	getLocation		()				{return			location;}
		void			setLocation		(D3DXVECTOR2	loc)	{location		=	loc;}
		void			setLocation		()				{location.x	=	x;
													 location.y	=	y;
													}
		D3DXVECTOR2	getCenter			()				{return			center;}
		void			setCenter			(D3DXVECTOR2	cen)	{center		=	cen;}
		
		void			setCenter			()				{center.x		=	x + ((width - 20) / 2);
													 center.y		=	y + ((height - 20) / 2);
													}
		void			setRadius			()				{radius		=	((width - 20) * Sprite::scaling) 
																	/ 2;
													}
		D3DXVECTOR2	getFutureLocation	()				{return			futureLocation;}
		void			setFutureLocation	(int			iter){futureLocation =	D3DXVECTOR2(location.x + (Sprite::velx * iter), 
																			  location.y + (vely * iter)
																			  );
													} 
		void			setTarget			(D3DXVECTOR2	tgt,
									 D3DXVECTOR2	tgtHeading,
									 double		tgtVelX,
									 double		tgtVelY,
									 double		tgtRadius);
		void			setTarget			(D3DXVECTOR2	tgt)	{target		=	tgt;}
		D3DXVECTOR2	getTarget			()				{return			target;}
		void			setFace			(D3DXVECTOR2	tgt)	{face		=	tgt;}
		void			setDirection		(double		ang)	{direction	=	ang;}
		double		getDirection		()				{return			direction;}
		void			setPlayerSpeed		(double		s)	{playerVel	=	s;}
		double		getPlayerSpeed		()				{return			playerVel;}
		void			setMaxSpeed		(double		s)	{maxVel		=	s;}
		double		getMaxSpeed		()				{return			maxVel;}
		void			setHasBall		(bool		b)	{hasBall		=	b;}
		bool			playerHasBall		()				{return			hasBall;}
		void			setBall			(Ball		*b)	{ball		=	b;}
		Ball			*getBall			()				{return			ball;}
		void			initKickTimer		(int			ms)	{ballKickTimer =	new Timer(ms);}
		void			resetKickTimer		(int			ms)	{ballKickTimer->setInterval(ms);}

		//Player functions
		void			playerController	();
		void			faceTarget		();
		void			movePlayer		();
		void			pursueTarget		();
		bool			isAtTarget		();
		bool			isAtBall			();
		void			waitForInstruct	();
		void			kickBall			();
		void			dribbleBall		();
		void			carryBall			(D3DXVECTOR2 oldLoc,			//Moves the ball with the player when they 
									 D3DXVECTOR2 newLoc);			//are in control

};




#endif