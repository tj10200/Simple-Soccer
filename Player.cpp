#include "Player.h"


//**********************************************************
//	Function: setTarget
//	Vars:	tgt		- The actual value of the target vector
//			tgtVelX	- The velocity in the X direction
//			tgtVelY	- The velocity in the Y direction
//			tgtRad	- The target's radius
//	Process:	The function calculates a future position of a target
//			 vector based on the current velocity of the target.
void	Player::setTarget(D3DXVECTOR2 tgt, D3DXVECTOR2	tgtHeading, double tgtVelX, double tgtVelY, double tgtRad)
{
	double tgtDist_sq	 =	pow((float)(tgtHeading.x	- tgt.x), 2) +			//Get squared dist from target to target heading
						pow((float)(tgtHeading.y	- tgt.y), 2),
		  pDist_sq	 =	pow((float)(tgt.x			- center.x), 2) +		//Get squared dist from player to target
						pow((float)(tgt.y			- center.y), 2),
		  tgtSlope,
		  dist,
		  timeToDist;

	tgtRadius		= tgtRad;										//Set target radius
	
	if (priorTgtDist < pDist_sq)
		target	= tgt;
	else if (tgtDist_sq	>	pDist_sq)									//Test if target heading is farther than target
	{
		if (tgtHeading.x	>	center.x	&&						//Test if player is between in X direction
			tgt.x		<	center.x	||
			tgtHeading.x	<	center.x	&&
			tgt.x		>	center.x)
			if(tgtHeading.y >	center.y	&&						//Test if player is between in Y direction
				tgt.y	<	center.y  ||
				tgtHeading.y < center.y  &&
				tgt.y	   > center.y)
				target	= tgt;								//Player is between target and target heading
			else												// move strait to target. Otherwise change directly
			{
				dist				=	sqrt(pDist_sq),					//Calc distance to target
				timeToDist		=	dist / playerVel;					//Calc the time to get to dist
					  
				if (dist			<	pow(radius * 4, 2))					//If within two body lengths
					target		=	tgt;								// chase target directly
				else
					target			=	D3DXVECTOR2 ((float)(tgt.x += (tgtVelX * timeToDist)),	//Set target to future location
												   (float)(tgt.y += (tgtVelY * timeToDist)));
			}
		else
		{
			dist				=	sqrt(pDist_sq),					//Calc distance to target
			timeToDist		=	dist / playerVel;					//Calc the time to get to dist
				  
			if (dist			<	pow(radius * 4, 2))						//If within two body lengths
				target		=	tgt;								// chase target directly
			else
				target			=	D3DXVECTOR2 ((float)(tgt.x += (tgtVelX * timeToDist)),	//Set target to future location
											   (float)(tgt.y += (tgtVelY * timeToDist)));
		}
	}
	else
	{

		dist				=	sqrt(pDist_sq),					//Calc distance to target
		timeToDist		=	dist / playerVel;					//Calc the time to get to dist
			  
		if (dist			<	pow(radius * 4, 2))						//If within two body lengths
			target		=	tgt;								// chase target directly

		target			=	D3DXVECTOR2 ((float)(tgt.x += (tgtVelX * timeToDist)),	//Set target to future location
									   (float)(tgt.y += (tgtVelY * timeToDist)));
	}

	priorTgtDist			=	pDist_sq;
}

//**********************************************************
//	Function: pursueTarget
//	Vars:	None
//	Process:	Faces a target and moves the player toward the
//			 the indicated target
void Player::pursueTarget()
{
	if (target != NULL)
	{
		faceTarget();
		movePlayer();
	}
}

//**********************************************************
//	Function: faceTarget
//	Vars:	None
//	Process:	This function tracks the target based off of current 
//			 target vector. NOTE: DX rotation function is right 
//			 handed, so all are clockwise from face NOT counter clockwise
void Player::faceTarget()
{
	double		adj	= 0,										//Used to calculate
				opp	= 0,										// the rotation needed to
				theta = 0;									// face target
	int			rotateX,
				rotateY;

	if (target != NULL)
	{
		if (target.x > center.x)									//Facing right
		{
			if(target.y > center.y)								//Facing right-down
			{
				opp		= target.y	- center.y;
				adj		= target.x	- center.x;
				theta	= PI_OVER_2	+ atan(opp / adj);			//Add 180 minus remaining angle
			}
			else if (target.y < center.y)							//Facing right-Up
			{
				adj		= center.y	- target.y;
				opp		= target.x	- center.x;
				theta	= atan(opp / adj);
			}
			else												//equal height and to right
				theta	= PI+ atan(opp / adj);
		}
		else if(target.x < center.x)								//Facing left
		{
			if(target.y > center.y)								//Facing left-down
			{
				adj		= target.y	- center.y;
				opp		= center.x	- target.x;
				theta	= PI	+ atan(opp / adj);					//Add 90 Plus remaining angle
			}
			else if (target.y < center.y)							//Facing left-Up
			{
				opp		= center.y		- target.y;
				adj		= center.x		- target.x;
				theta	= PI			+ PI_OVER_2 + atan(opp / adj);//Add 270 + angle
			}
			else												//equal height and to left
				theta	= PI			+ PI_OVER_2;
		}
		
		rotateX		= center.x + (face.x - center.x) * cos(theta) - (face.y - center.y) * sin(theta);
		rotateY		= center.y + (face.x - center.x) * sin(theta) + (face.y - center.y) * cos(theta); 
		face.x		= rotateX;
		face.y		= rotateY;
		rotation		= theta;
	}
}

//**********************************************************
//	Function: movePlayer
//	Vars:	None
//	Process:	Moves the player toward the target vector in 
//			accordance to the slope away from the target
void Player::movePlayer()
{
	if (target.x > center.x)										//Test if right of target
	{
		velx		= playerVel;
		vely		= (target.y - center.y) / (target.x - center.x);
	}
	else	if (target.x < center.x)									//Left of target
	{	
		velx		= -playerVel;
		vely		= (target.y - center.y) / (center.x - target.x);
	}
	else														//Equal to target x
	{
		velx		= 0;											//Do not move to side if above or below target
		
		if (target.y > center.y)									//Test if above
			vely	= playerVel;									//Set velocity 
		else
			vely = -playerVel;									//Negate velocity
	}

	if (vely		> maxVel)
		vely		= maxVel;
	else if (vely  < -maxVel)
		vely		= -maxVel;

	
	x			+= velx;										//Move all player calculable vectors
	y			+= vely;

	setCenter		();
	setLocation	();
}


//**********************************************************
//	Function: isAtTarget
//	Vars:	None
//	Process:	Performs collision test by way of box testing
//			 the edges of each circle are simply taken
//			 from the radius
//			
bool	Player::isAtTarget		()
{
			
	if (tgtRadius == 0)
		return false;													//Do not test if 
	
																	// target is not initialized
	
	//Test if target is ball
	if (target	==	ball->getBallLocation());							//If target is the ball, then 
		return isAtBall();												//Perform target check ising sides of ball

	//Test moving from left or right.
	if (target.x	== center.x)
	{	
		if (target.y == center.y)
		/*if ((target.x - tgtRadius	< center.x + radius &&						//Within left-right boundaries
		target.x + tgtRadius	> center.x - radius))						// test from left
	{
		if ((target.y + tgtRadius	> center.y - radius &&					//Within top-bottom boundaries
			target.y - tgtRadius	< center.y + radius))					// test from bottom
	*/	{
			return true;												//Within range
		}
		else		
				return false;												//No collision
	}
	else 
		return		false;											//No collision
}
bool	Player::isAtBall()
{
	D3DXVECTOR2 ballCen	=	ball->getBallLocation(),
			  ballLoc =	D3DXVECTOR2 ((float)ball->getX(), (float)ball->getY()); 
	double	 ballRad	=	ball->getRadius();

	//Test moving from left or right.
	if ((ballLoc.x	<= center.x + radius + 5 &&										//Within left-right boundaries
		ballLoc.x	>= center.x - radius - 5) ||										// test from right 
	    (ballCen.x + ballRad	> center.x - radius &&						
	     ballCen.x - ballRad	< center.x + radius))									// test from left
	{
		if ((ballLoc.y	>= center.y - radius - 5 &&									//Within top-bottom boundaries
			ballLoc.y	<= center.y + radius + 5) ||									// test from top
		    (ballCen.y - ballRad	< center.y + radius &&
		     ballCen.y + ballRad	> center.y - radius))								// test from bottom
		{
			hasBall			= true;											//Allow ball kick
			return true;														//Within range
		}
		else		
				return false;													//No	collision
	}
	else 
		return		false;													//No collision
}
void	Player::waitForInstruct	()
{
	faceTarget();
	velx = 0;
	vely = 0;
}

//**********************************************************
//	Function: kickbALL
//	Vars:	None
//	Process:	Returns a random velocity indicating that a
//			 ball was kicked.
void Player::kickBall()
{
	D3DXVECTOR2	cen	= center,
				f	= face;

	double		rise,
				run,
				slope,
				xVal,
				yVal;

	if (!ball->ballKickAllowed())								//If outside of the time range, do not kick ball
		return;

	if (target.x	==	center.x)
		run		=	(target.x + 1) - center.x;
	else
		run		=	target.x	-	center.x;

	rise			=	target.y	-	center.y;
	
	slope		=	rise		/	run;
	
	if (target.x	>	center.x)
		xVal		=	target.x	+	(5 * radius);
	else
		xVal		=	target.x	-	(5 * radius);

	yVal			=	xVal		*	slope;	
	
	ball->setInitVel	(4);
	ball->setTarget	(target);
	ball->moveBall();

	hasBall		=	false;													//Indicate no longer has ball
	
	setTarget			(ball->getBallLocation(),
					 ball->getTarget(),
					 ball->getVelX(), 
					 ball->getVelY(), 
					 ball->getRadius()
					 );
	setPlayerState		(ATTACK);


}
//**********************************************************
//	Function: dribleBall
//	Vars:	None
//	Process:	Returns a random velocity indicating that a
//			 ball was being kicked in a direction.
void Player::dribbleBall()
{

	D3DXVECTOR2 oldLoc	=	getCenter(),
			  newLoc;
	

	movePlayer();

	newLoc			=	getCenter();

	carryBall				(oldLoc, newLoc);

}

void	Player::carryBall(D3DXVECTOR2 oldLoc, D3DXVECTOR2 newLoc)
{
	
	if		(oldLoc	==	newLoc)
		return;
		
	if	(oldLoc.x		>	newLoc.x)							//Moving Left
		ball->setX(center.x	- ball->getWidth());
	else if (oldLoc.x	<	newLoc.x)							//Moving Right
		ball->setX(center.x + radius + 5);
	else
		ball->setX(newLoc.x);								//No Horizontal movement
	
	if	(oldLoc.y		>	newLoc.y)							//Moving up
		ball->setY(center.y - ball->getHeight());
	else if (oldLoc.y	<	newLoc.y)							//Moving down
		ball->setY(center.y + radius + 5);
	else
		ball->setY(newLoc.y);								//No vertical movement

	ball->setCenter		(D3DXVECTOR2 (ball->getX() + ((ball->getWidth() * ball->getScale()) / 2), 
								    ball->getY() + ((ball->getHeight() * ball->getScale()) / 2)));

}
//**********************************************************
//	Function: playerController
//	Vars:	None
//	Process:	calls appropriate functions depending on the 
//			 player's current state.
void	Player::playerController	()
{
	switch (state)
	{
		case ATTACK:
			if (isAtTarget())		
			{
				atTarget	= true;
				
				if (hasBall)
				{
					ball->setInitVel(0);
					setPlayerState(DRIBBLE_BALL);
				}
			}
			else
			{
				atTarget	= false;
				pursueTarget();
			}
			break;													// velocity
		case PURSUE_TGT:
			if (isAtTarget())		
				atTarget	= true;
			else
			{
				atTarget	= false;
				pursueTarget();
			}
			break;	
		case DEFEND:													//Lets players try to stop a opposing player
			if (isAtTarget())		
			{			
				atTarget	= true;
				setTarget(ball->getCenter(),
						ball->getTarget(),
						ball->getVelX(),
						ball->getVelY(),
						ball->getRadius()
						);
				setPlayerState(WAIT);
			}	
			else
			{
				atTarget	= false;
				pursueTarget();
			}
			break;
		case SUPPORT:													//Lets player get into support position
			if (isAtTarget())		
			{
				atTarget	= true;
				setTarget(ball->getCenter(), ball->getTarget(),0,0, ball->getRadius());
			}
			else
			{
				atTarget	= false;
				pursueTarget();
			}
			break;
		case GOALIE:													//Allows the player to remain at goal
			//NEEDS WORK
			break;
		case KICK_BALL:												//Kick ball at target
			if (ballKickTimer->isTime())
				kickBall();
			break;
		case DRIBBLE_BALL:												//Allows player to make small kicks
			dribbleBall();
			break;
		case AT_TARGET:												//Indicates that a state change is needed
			atTarget	= true;
			setPlayerState(WAIT);
			break;
		case WAIT:
			waitForInstruct();
			break;
		case USER_CONTROL:												//No action needed	
			if (ballKickTimer->isTime())
				isAtBall();
			faceTarget();
			break;	
	}
}

Player::~Player()
{
	
	if (ball)
		ball = NULL;
	
	if (ballKickTimer)
		delete ballKickTimer;

	ballKickTimer	=	 NULL;
}