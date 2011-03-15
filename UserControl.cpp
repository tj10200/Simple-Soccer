#include "UserControl.h"


void	UserControl::checkInput	()
{
	//Test movement keys
	if (xObj->Key_Down(DIK_UPARROW))									//Up 
	{
		if(xObj->Key_Down		(DIK_LEFTARROW))						//Up + Left
			curMovement	=	UP_LEFT;
		else if(xObj->Key_Down	(DIK_RIGHTARROW))						//Up + Right
			curMovement	=	UP_RIGHT;
		else
			curMovement	=	UP;									//Up Only
	}
	else if (xObj->Key_Down		(DIK_DOWNARROW))						//Down
	{
		if(xObj->Key_Down		(DIK_LEFTARROW))						//Down + Left
			curMovement	=	DOWN_LEFT;
		else if(xObj->Key_Down	(DIK_RIGHTARROW))						//Down + Right
			curMovement	=	DOWN_RIGHT;
		else
			curMovement	=	DOWN;								//Down Only
	}
	else if (xObj->Key_Down		(DIK_LEFTARROW))						//Left Only
		curMovement		=	LEFT;
	else if (xObj->Key_Down		(DIK_RIGHTARROW))						//Right Only
		curMovement		=	RIGHT;
	else
		curMovement		=	NO_INPUT;								//No movement key pressed

	//Test actions
	if (xObj->Key_Down			(DIK_D))								//D == Kick Ball
		curAction			=	PASS_KICK;
	else if(xObj->Key_Down		(DIK_F))								//A == Switch players
		curAction			=	GOAL_KICK;
	else if (xObj->Key_Down		(DIK_A))
		curAction			=	SWITCH;
	else
		curAction			=	NO_INPUT;

}

void	UserControl::switchPlayer ()
{
	if (curPlayer)
	{
		curPlayer->setPlayerState(WAIT);
		curPlayer->setStartFrm	(0);
		curPlayer->setEndFrm	(0);
		curPlayer->setFrame		(0);
	}
			

	setPlayer	(team->getNextPlayer(curPlayerIdx), 
			 curPlayerIdx);

	curPlayer->setStartFrm	(9);
	curPlayer->setEndFrm	(9);
	curPlayer->setFrame		(9);
	curPlayer->setPlayerState(USER_CONTROL);

}

void	UserControl::movePlayer	()
{
	D3DXVECTOR2	playerLoc		=	curPlayer->getCenter();
	D3DXVECTOR2	newDirection		(0.0f, 0.0f);
	double		playerVel		=	curPlayer->getPlayerSpeed(),
				newRotation	=	0,
				radius		=	curPlayer->getRadius(),
				curX			=	curPlayer->getX(),
				curY			=	curPlayer->getY();
	

	switch (curMovement)
	{
		case UP_LEFT:
			newDirection.x			--;
			newDirection.y			--;
			newRotation			=	270 * PI_OVER_180;
			break;
		case UP_RIGHT:
			newDirection.x			++;
			newDirection.y			--;
			newRotation			=	45 * PI_OVER_180;
			break;
		case UP:
			newRotation			=	0;
			newDirection.y			--;
			break;
		case DOWN_LEFT:
			newRotation			=	225 * PI_OVER_180;
			newDirection.x			--;
			newDirection.y			++;
			break;
		case DOWN_RIGHT:
			newRotation			=	135 * PI_OVER_180;
			newDirection.x			++;
			newDirection.y			++;
			break;
		case DOWN:
			newRotation			=	180 * PI_OVER_180;
			newDirection.y			++;
			break;
		case LEFT:
			newRotation			=	270 * PI_OVER_180;
			newDirection.x			--;
			break;
		case RIGHT:
			newRotation			=	90 * PI_OVER_180;
			newDirection.x			++;
			break;
		default:
			break;
	}

	curPlayer->setX				(curX + (newDirection.x * playerVel));
	curPlayer->setY				(curY + (newDirection.y * playerVel));
	curPlayer->setCenter			();
	curPlayer->setLocation			();
	curPlayer->setRotation			(newRotation);

	if (curPlayer->playerHasBall())
	{
		playerLoc				=	curPlayer->getCenter();
		movement.x			=	playerLoc.x + (newDirection.x * radius);
		movement.y			=	playerLoc.y + (newDirection.y * radius);

		curPlayer->carryBall		(playerLoc, movement);
	}
}


void UserControl::kickBall()
{
	D3DXVECTOR2	tempTgt,
				ballLoc,
				pCenter;
	double		dir;

	if (curAction	==	GOAL_KICK)
	{
		tempTgt	=	team->getGoalShot(curPlayer);
		
		if (tempTgt	==		NO_SHOT)
		{
			ballLoc = curPlayer->getBall()->getBallLocation();
			pCenter = curPlayer->getCenter();

			if (ballLoc.x	==	pCenter.x)
			{			
				tempTgt.x	=	ballLoc.x;
				
				if (ballLoc.y	> pCenter.y)
					tempTgt.y	= ballLoc.y + 2	* curPlayer->getHeight();
				else
					tempTgt.y = ballLoc.y - 2	* curPlayer->getHeight();
			}
			else
			{
				dir		=	(ballLoc.y		- pCenter.y) / (ballLoc.x	-	pCenter.x); 
				
				if (ballLoc.x	> pCenter.x)
					tempTgt.x = ballLoc.x + 2	* curPlayer->getWidth();
				else
					tempTgt.x = ballLoc.x - 2	* curPlayer->getWidth();

				tempTgt.y		= tempTgt.x		* dir;
			}
		}
	}
	else if (curAction		==	PASS_KICK)
		tempTgt			=	team->getPassShot(curPlayer);

	
	curPlayer->setTarget		(tempTgt);
	curPlayer->kickBall			();
	curPlayer->setPlayerState	(USER_CONTROL);
	team->setTeamHasBall		(false);
}

void	UserControl::cycleStates()
{
		
	checkInput	();
	
	if (curMovement	!=	NO_INPUT)
		movePlayer	();
	
	if (switchTimer->isTime())
	{
		if (curAction		==	SWITCH)
		switchPlayer	();
	
		else if(	curAction	==	GOAL_KICK ||
				curAction	==	PASS_KICK)
			if(	curPlayer->playerHasBall())	
				kickBall		();
	}
}
void	UserControl::ballHandlerSwitch ()
{
	Player	*temp	=	team->getBallHolder();

	if (team->getTeamHasBall())
		while (temp			!=	curPlayer &&
			  team->getTeamHasBall	())
		{
			switchPlayer			();
			team->setTeamHasBall	();
		}
}

UserControl::~UserControl()
{															
	curPlayer		=	NULL;
	team			=	NULL;
	xObj			=	NULL;

	if (inputTimer)
		delete	inputTimer;
}