#ifndef USER_CONTROL_H
#define USER_CONTROL_H


#include "GameHeaders.h"
#include "Team.h"
#include "Timer.h"

enum	MOVEMENT
{
	UP,
	UP_RIGHT,
	UP_LEFT,
	DOWN,
	DOWN_RIGHT,
	DOWN_LEFT,
	LEFT,
	RIGHT,
	GOAL_KICK,
	PASS_KICK,
	SWITCH,
	NO_INPUT
};

class UserControl
{
	private:
		Player		*curPlayer;							//Player currently selected
		int			curPlayerIdx;							//The index of the current selected player
		Team			*team;								//The team of players to select from
		D3DXVECTOR2	movement;								//Target to move player in
		MOVEMENT		curMovement;							//Defines current movement
		MOVEMENT		curAction;
		double		curDir;								//The current direction
		DirectX		*xObj;
		Timer		*inputTimer;
		Timer		*switchTimer;

	public:
		UserControl							()					{
																	curPlayer		=	NULL;
																	team			=	NULL;
																	xObj			=	NULL;
																	curPlayerIdx	=	0;
																	inputTimer	=	new Timer(100);
																	switchTimer	=	new Timer(200);
																}
		~UserControl							();
		
		void			setTeam					(Team		*t)		{	team			=	t;}
		Team			*getTeam					()					{return				team;}
		void			setPlayer					(Player		*p, 
											 int			idx)		{
																	curPlayer		=	p;
																	curPlayerIdx	=	idx;
																	p->setPlayerState(USER_CONTROL);
																}
		Player		*getPlayer				()					{return				curPlayer;}
		void			setDirectXDev				(DirectX		*x)		{xObj			=	x;}
		void			setMovement				(D3DXVECTOR2	m)		{movement			=	m;}
		D3DXVECTOR2	getMovement				()					{return				movement;}
		void			setCurAction				(MOVEMENT		m)		{curAction		=	m;}
		MOVEMENT		getCurAction				()					{return				curAction;}

		
		void			checkInput				();			//Cycles through all available states to see if a
														// key was pressed
		void			switchPlayer				();			//Allows a user to select another player on the field
		void			movePlayer				();			//Set the movement vector
		void			kickBall					();			//Allows the player to kick the ball to the goal or pass
		void			cycleStates				();			//Control user behavior
		void			ballHandlerSwitch			();			//Switched to player controlling the ball.
		D3DXVECTOR2	getFacingShot				();			//Calculates a target depending on which direction the user is traveling in.




};	
#endif
