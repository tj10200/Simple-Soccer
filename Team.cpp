#include "Team.h"


void Team::InitPlayers()
{
	players			= new Player [NUM_PLAYERS];								//Create Team
	
	for (int x = 0; x < NUM_PLAYERS; x++)
	{
		if (homeTeam)
		{
			players[x].setStartFrm	(0);
			players[x].setEndFrm	(0);
			players[x].setFrame		(0);
			players[x].setLocation	(D3DXVECTOR2(homeRegions[x + KICKOFF].x, homeRegions[x + KICKOFF].y));
			players[x].setX		(homeRegions[x	+ KICKOFF].x);
			players[x].setY		(homeRegions[x + KICKOFF].y);
		
		}
		else
		{
			players[x].setStartFrm	(24);
			players[x].setEndFrm	(24);
			players[x].setFrame		(24);
			players[x].setLocation	(D3DXVECTOR2(homeRegions[x + RECEIVE].x, homeRegions[x + RECEIVE].y));
			players[x].setX		(homeRegions[x	+ RECEIVE].x);
			players[x].setY		(homeRegions[x + RECEIVE].y);

		}

		players[x].setScale		(1.0f);
		players[x].setRotation	(0);
		players[x].setWidth		(75);
		players[x].setHeight	(75);
		players[x].setColumns	(8);
		players[x].setColor		(D3DCOLOR_XRGB(255,255,255));			//Color Mask
		players[x].setTexture	(playerTexture);
		players[x].setFace		(D3DXVECTOR2 (
										(float)players[x].getX() + (players[x].getWidth() / 2), 
										(float)players[x].getY()
										)
							);
		players[x].setCenter	();
		players[x].setRadius	();
		players[x].setPlayerSpeed(1);
		players[x].setMaxSpeed	(1);

		players[x].setTarget	(
								field->ball->getBallLocation(),
								field->ball->getTarget(),
								field->ball->getVelX(), 
								field->ball->getVelY(), 
								field->ball->getRadius()
							);
		players[x].setBall		(	field->ball);
		players[x].setHasBall	(false);

		players[x].initKickTimer(100);
	
		
	}
}

void Team::InitHomeRegions()
{
	homeRegions		= new D3DXVECTOR2[NUM_PLAYERS * NUM_REGIONS];
	
	if (homeTeam)
	{
		homeRegions[0 + NEUTRAL]	= field->regions[3][7];					//Upper Left
		homeRegions[1 + NEUTRAL]	= field->regions[22][7];					//Lower Left
		homeRegions[2 + NEUTRAL]	= field->regions[12][14];				//Center

		homeRegions[0 + DEFENCE]	= field->regions[5][12];					//Upper Front
		homeRegions[1 + DEFENCE]	= field->regions[17][12];				//Lower Front
		homeRegions[2 + DEFENCE]	= field->regions[9][6];					//Upper Rear

		homeRegions[0 + OFFENCE]	= field->regions[3][15];					//Upper Front
		homeRegions[1 + OFFENCE]	= field->regions[22][15];				//Lower Front
		homeRegions[2 + OFFENCE]	= field->regions[12][18];				//Rear  Center

		homeRegions[0 + KICKOFF]	= field->regions[5][8];					//Rear Defensive
		homeRegions[1 + KICKOFF]	= field->regions[11][17];				//Upper Center
		homeRegions[2 + KICKOFF]	= field->regions[15][18];				//Lower Center
	
		homeRegions[0 + RECEIVE]	= field->regions[9][14];					//Rear Defensive
		homeRegions[1 + RECEIVE]	= field->regions[16][14];				//Upper Center
		homeRegions[2 + RECEIVE]	= field->regions[12][10];				//Lower Center
	}
	else
	{
		//Neutral Regions
		homeRegions[0 + NEUTRAL]	= field->regions[3][32];					//Upper right
		homeRegions[1 + NEUTRAL]	= field->regions[22][32];				//Lower right
		homeRegions[2 + NEUTRAL]	= field->regions[12][25];				//Center

		homeRegions[0 + DEFENCE]	= field->regions[5][27];					//Upper Front
		homeRegions[1 + DEFENCE]	= field->regions[17][27];				//Lower Front
		homeRegions[2 + DEFENCE]	= field->regions[9][33];					//Upper Rear
		
		homeRegions[0 + OFFENCE]	= field->regions[3][25];					//Upper Front
		homeRegions[1 + OFFENCE]	= field->regions[22][25];				//Lower Front
		homeRegions[2 + OFFENCE]	= field->regions[12][21];				//Rear  Center
		
		homeRegions[0 + KICKOFF]	= field->regions[5][31];					//Rear Defensive
		homeRegions[1 + KICKOFF]	= field->regions[11][22];				//Upper Center
		homeRegions[2 + KICKOFF]	= field->regions[15][21];				//Lower Center
		
		homeRegions[0 + RECEIVE]	= field->regions[9][25];					//Rear Defensive
		homeRegions[1 + RECEIVE]	= field->regions[16][25];				//Upper Center
		homeRegions[2 + RECEIVE]	= field->regions[12][30];					//Lower Center		
	}

}
void Team::InitSupportCalc()
{
	if (homeTeam)	//Different Goals
		supCalc	=	new SupportPosCalculator(field->regions,
										field->Goals[1], 
										players, 
										oppTeam, 
										REG_ROWS, 
										REG_COLS,
										NUM_PLAYERS,
										homeTeam,
										22
										);
	else
		supCalc	=	new SupportPosCalculator(field->regions,
										field->Goals[0], 
										players, 
										oppTeam, 
										REG_ROWS, 
										REG_COLS,
										NUM_PLAYERS,
										homeTeam,
										0
										);
	
}
void Team::DrawTeam()
{
	for (int x = 0; x < NUM_PLAYERS; x++)
	{
		xObj->Sprite_Transform_Draw(							//Draw players to screen
								players[x].getTexture(),
								players[x].getScale(),
								players[x].getX(),
								players[x].getY(),
								players[x].getRotation(),
								players[x].getWidth(),
								players[x].getHeight(),
								players[x].getFrame(),
								players[x].getColumn(),
								players[x].getColor()
							);
	}

	DEBUG_SCRIPT	();
}

void	Team::setClosestPlayer()
{
	double	oldDist	= 9999999999,
			distSq	= 0,
			xDist	= 0,
			yDist	= 0;

	for (int x = 0; x < NUM_PLAYERS; x++)
	{
		if (players[x].playerHasBall())
		{
			closestPlayer = ballHandler = &players[x];
			x		    = NUM_PLAYERS;
		}
		else
		{
			xDist =	(players[x].getX() - field->ball->getX()) *	(players[x].getX() - field->ball->getX());
			yDist =	(players[x].getY() - field->ball->getY()) *	(players[x].getY() - field->ball->getY());

			distSq = xDist + yDist;

			if (distSq < oldDist)
			{
				oldDist		= distSq;
				closestPlayer	= &players[x];
			}
		}
	}
}

void Team::setPlayerStates()
{
	bool		passBall = true;
	double	passPotent,
			goalPotent;
	D3DXVECTOR2 playerPos,
			  supportTgt,
			  goalRegion,									//Used to reset target after collision
			  ballHandlerTgt;								//Used to point player in best direction when
														// has ball
	double stuckCollisionCounter = 0;							//Used to break infinite loop found in collision

	setTeamHasBall ();										//sets homeTeam flag if in control
	setClosestPlayer();										//Sets player closest to ball. Sets ballHandler if avail
	SupportingRegion = supCalc->getBestSupportPos();				//Sets support region assuming team has the ball
	
	for (int x = 0; x < NUM_PLAYERS; x++)
	{	
		playerPos			=	players[x].getCenter();
		checkForCollision		(&players[x]);						//checks for and resolves sprite collisions						

		if (players[x].getPlayerState()	==	USER_CONTROL)		//Only calc supporting player metrics
		{
			players[x].setTarget(field->ball->getBallLocation());

			if (players[x].playerHasBall())
				ballHandler	=	&players[x];
			
			if (supportRegTimer->isTime())					
					supCalc->calcSupportingRegions(&players[x]);		//Calculates supporting region every second

		}
		else if (hasBall)
		{
			
			if(players[x].playerHasBall())					//If player has ball then calc if
			{											// should pass, make shot, or make move to goal
				ballHandler	=	&players[x];

				if (supportRegTimer->isTime())					
					supCalc->calcSupportingRegions(&players[x]);	//Calculates supporting region every second

				handleBall		(&players[x], playerPos);	

				
			}											
			else											//Player doesn't have ball
			{
				supportPlayer	=	supCalc->getClosestToTgt(players, ballHandler->getCenter());

				supportTgt	=	supCalc->getSupportPos(&players[x], ballHandler);
				
				if (&players[x]	==	supportPlayer	&&
					players[x].getPlayerState() != USER_CONTROL)
				{				
					players[x].setTarget(supportTgt);
					players[x].setPlayerState(SUPPORT);
				}
				else
				{
					players[x].setTarget(homeRegions[x + DEFENCE]);
					players[x].setPlayerState(WAIT);
				}
			}
		}
		else 
		{
			closestPlayer		=	supCalc->getClosestToTgt(players, field->ball->getCenter());
			
			closestPlayer->setTarget(field->ball->getBallLocation(),
								field->ball->getTarget(),
								field->ball->getVelX(), 
								field->ball->getVelY(), 
								field->ball->getRadius());
		
			if (players[x].getCenter()!= closestPlayer->getCenter())
			{
				players[x].setTarget(homeRegions[x + DEFENCE]);
				players[x].setPlayerState(DEFEND);
			}
			else 
				closestPlayer->setPlayerState(ATTACK);		
		}
	}
	
	cyclePlayerStates();

}
void	Team::setTeamHasBall()
{
	D3DXVECTOR2	prevTgt;
	hasBall		= false;

	for (int x = 0; x < NUM_PLAYERS && !hasBall; x++)
	{
		if (players[x].playerHasBall())
		{
			prevTgt	= players[x].getTarget();
			
			players[x].setTarget(field->ball->getCenter());
			
			if	(players[x].isAtTarget())
				hasBall	= true;
			else
				players[x].setHasBall(false);
			
			players[x].setTarget(prevTgt);
		}
	}
}
void Team::cyclePlayerStates()
{
	for (int x = 0; x < NUM_PLAYERS; x++)
		players[x].playerController();
}

bool	Team::isPlayerThreatened (D3DXVECTOR2 playerLoc)
{
	double			dist		= 0,
					radius;
	D3DXVECTOR2		oppTeamLoc;
	bool				threat	= false;

	for (int x	= 0; x < NUM_PLAYERS && !threat; x++)
	{
		oppTeamLoc	=	D3DXVECTOR2((float)oppTeam[x].getX(), (float)oppTeam[x].getY());
		radius		=	oppTeam[x].getRadius();

		dist			=	pow((float)(playerLoc.x - oppTeamLoc.x), 2) +
						pow((float)(playerLoc.y - oppTeamLoc.y), 2);
		
		if ((dist	- (2 * radius)) < MIN_THREAT_DIST)
			return true;									//If any opp player is within range, return
														// true
	}

	return	false;
}	
Player *Team::getClosestPlayer(D3DXVECTOR2 playerLoc, Player *team)
{
	double			dist_sq		= 0,
					minDist	= 99999999;
	D3DXVECTOR2		oppLoc;
	Player			*closest;

	for (int x = 0;	x < NUM_PLAYERS; x++)
	{
		oppLoc	=	team[x].getCenter();

		dist_sq		=	(pow(oppLoc.x - playerLoc.x, 2) + pow(oppLoc.y - playerLoc.y, 2));
		if			(dist_sq < minDist)
		{
			minDist	= dist_sq;
			closest	= &team[x];
		}
	}

	return closest;
}

void	Team::checkForCollision		(Player *p)
{
	static int stuckCollisionCounter = 0;
	static D3DXVECTOR2 prevTgt;
	PLAYER_STATE s	=	p->getPlayerState();

	prevTgt	=	p->getTarget();

	if (isCollision(p))					
		p->playerController();								//Cycle one time with new target
		
	p->setTarget(prevTgt);
	p->setPlayerState(s);

}
bool	Team::isCollision		(Player *p)
{
	D3DXVECTOR2	oppTeamTgt,
				oppTeamHeading,
				teamTgt,
				teamHeading,
				center,
				playerHeading;
	double		tgtRadius,
				teamRadius,
				radius;
	isCollisionFound = false;
		
	for (int x = 0; x < NUM_PLAYERS; x++)
	{
		oppTeamTgt=	oppTeam[x].getCenter();
		tgtRadius	=	oppTeam[x].getRadius();
		oppTeamHeading = oppTeam[x].getTarget();
		teamTgt	=	players[x].getCenter();
		teamRadius=	players[x].getRadius();
		teamHeading =	players[x].getTarget();
		center	=	p->getCenter();
		radius	=	p->getRadius();
		playerHeading=	p->getTarget();
		
		//Opposing player tests
		if (boundingBoxCollide(center, radius, oppTeamTgt, tgtRadius))
			if (radiusDistCollide(center, radius, oppTeamTgt, tgtRadius))
				{
					collisionReaction(p, center, radius, &oppTeam[x], oppTeamTgt, tgtRadius);
					isCollisionFound = true;
				}
		
		//Teammate tests
		if (!((center.x	== teamTgt.x) && (center.y == teamTgt.y)))
			if (boundingBoxCollide(center, radius, teamTgt, teamRadius))
				if (radiusDistCollide(center, radius, teamTgt, teamRadius))
					{
						collisionReaction(p, center, radius, &players[x], teamTgt, teamRadius);
						isCollisionFound = true;
					}

			

		
	}

	return isCollisionFound;
}
bool Team::boundingBoxCollide	(D3DXVECTOR2 center, double radius, D3DXVECTOR2 testCenter, double testRadius)
{
	//Test moving from left or right for opposite team.
	if ((testCenter.x - testRadius	< center.x + radius &&						//Within left-right boundaries
		testCenter.x + testRadius	> center.x - radius) ||						// test from right 
	    (testCenter.x + testRadius	> center.x - radius &&						
		testCenter.x - testRadius	< center.x + radius))						// test from left
	{
		if ((testCenter.y + testRadius	> center.y - radius &&					//Within top-bottom boundaries
			testCenter.y - testRadius	< center.y + radius) ||					// test from top
		    (testCenter.y - testRadius	< center.y + radius &&
			testCenter.y + testRadius	> center.y - radius))					// test from bottom
			return true;													//Collision found
	}

	return false;															//No collision

}
//Tests actual radius for collision. Removes "fat" from bounding box
bool Team::radiusDistCollide	(D3DXVECTOR2 center, double radius, D3DXVECTOR2 testCenter, double testRadius)				
{
	double	a	= testCenter.y - center.y,
			b	= testCenter.x - center.x,
			a_sq = a * a,
			b_sq = b * b,
			dist = sqrt(a_sq + b_sq),
			radi_sum = radius + testRadius;

	if (dist <= (radius + testRadius))
		return true;
	else
		return false;
}
//Tests if collision is caused by player and if
// a reaction is necessary
bool	Team::playerInBack			(D3DXVECTOR2 center, D3DXVECTOR2 target, D3DXVECTOR2 testCenter, D3DXVECTOR2 testTarget)				
{
	double	a1		=	target.y - center.y,
			a2		=	testTarget.y - testCenter.y,
			b1		=	target.x - center.x,
			b2		=	testTarget.x - testCenter.x,
			a1_sq	=	a1 * a1,
			a2_sq	=	a2 * a2,
			b1_sq	=	b1 * b1,
			b2_sq	=	b2 * b2,
			dist1_sq	=	a1_sq + b1_sq,
			dist2_sq	=	a2_sq + b2_sq;

	if (dist1_sq > dist2_sq)
		return true;
	else
		return false;
}
//Sets new target for colliding player
void Team::collisionReaction		(Player		*player,		D3DXVECTOR2	center,		double radius, 
							 Player		*testPlayer,	D3DXVECTOR2	testCenter,	double testRadius)
{	
	double	intercectX,
			intercectY,
			newTargetX,
			newTargetY,
			tanSlope,
			newY,
			newX,
			diameter;
	
	D3DXVECTOR2 newHeading;

	int		curDir;															//Current direction

	if		(radius	!=	testRadius)											//recalc radius if different
		diameter	=	2 * (testRadius / radius);
	else
		diameter	=	2 * radius;

	if (player->getTarget().x > center.x)											//Check if moving left or
		curDir	=	1;														// right
	else
		curDir	=	-1;

	if (center.x	==	testCenter.x)												//Vertical intercection
		intercectX =	center.x;
	else
		intercectX =	(center.x + testCenter.x) / 2;

	if (testCenter.y - center.y <= .000001)											//Horizontal intercection
		intercectY =	center.y;
	else
		intercectY =	-intercectX * ((testCenter.x - center.x)/(testCenter.y - center.y)) + 
					((pow(testCenter.x,2) - pow(center.x, 2) + pow(testCenter.y, 2) - pow(center.y, 2)) /
					 (2 * (testCenter.y - center.y)));
	
	if (intercectX	>	center.x)
	{
		player->setX(player->getX() - 1);
		testPlayer->setX(testPlayer->getX() + 1);
	}
	else if (intercectX	< center.x)
	{	
		player->setX(player->getX() + 1);
		testPlayer->setX(testPlayer->getX() - 1);
	}
	if (intercectY	>	center.y)
	{	
		player->setY(player->getY() - 1);
		testPlayer->setY(testPlayer->getY() + 1);
	}
	else if (intercectY < center.y)
	{	
		player->setY		(player->getY() + 1);
		testPlayer->setY	(testPlayer->getY() - 1);
	}
	player->setCenter();
	player->setLocation();

	testPlayer->setCenter();
	testPlayer->setLocation();

	//Vertical slope
	if (intercectY - center.y <= .000001)
	{
		if (player->getTarget().y > center.y) 	
			newY	=	center.y + diameter * radius;
		else
			newY =	center.y - diameter * radius;

		newX		=	center.x;
	}
	else
	{

		tanSlope	=	-((intercectX - center.x) /(intercectY - center.y));

		newX		=	center.x + (diameter * curDir * 10);
		newY		=	newX * tanSlope * curDir	* 10;
	
	}

	newHeading.x	=	newX;
	newHeading.y	=	newY;
	player->setTarget(newHeading);

}
void	Team::collisionReaction (Player *front,	Player *back)
{
	double		xDiff			=	front->getX()	-							//Difference in x coordinates 
									back->getX(),				
				yDiff			=	front->getY()	-							//Difference in y coordinates
									back->getY(),				
				cenDiff			=	sqrt(pow(xDiff,2)			+				//Distance between centers
										pow(yDiff, 2)),							
				intercectDiff		=	(pow(cenDiff,2)			+				//distance from center to line joining intercect points
									pow(back->getRadius(),2)	-
									pow(front->getRadius(),2))	/
									(2 * cenDiff),
				backXVal			=	back->getX(),
				backYVal			=	back->getY(),
				backTargetY		=	back->getTarget().y,
				frontRadius		=	front->getRadius(),
				radToIntercectDiff	=	sqrt(abs((frontRadius *  frontRadius)- 
										(intercectDiff * intercectDiff))),
				xVal				=	backXVal				+				//new target x val
									((xDiff * intercectDiff)/cenDiff) +
									(yDiff / cenDiff)			*
									radToIntercectDiff,				
				yVal				=	backYVal				+				//new target x val
									((yDiff * intercectDiff)/cenDiff) -
									(xDiff / cenDiff)			*
									radToIntercectDiff,				//new target y val
				
									
				xTanVal			=	(frontRadius - (yVal * front->getY())) /
									 xVal;
			/*
			The point we know is on the line is (3,4).

So the generic equation of the line is:

(y-y0) = m(x-x0), where
(x0, y0) is the point we know is on the line, or (3,4) in this case
m is the slope, -3/4 in this case
x,y remain as variables

(y - 4) = (-3/4)(x - 3)
y - 4 = (-3/4)x + 9/4
y = (-3/4)x + 25/4*/
				back->setTarget	(D3DXVECTOR2 ((float)xTanVal, (float)backTargetY), D3DXVECTOR2 ((float)xTanVal, (float)backTargetY), 0, 0, 1);
				back->pursueTarget();
}
D3DXVECTOR2 Team::getBestBallMovement(D3DXVECTOR2	playerLoc, D3DXVECTOR2 goalLoc, D3DXVECTOR2 opponentLoc,
									 double		playerRadius)
{
	double	distToOpp_sq	=	pow(playerLoc.x - opponentLoc.x , 2) + 			//Distance to opponent
							pow(playerLoc.y - opponentLoc.y, 2),  
			bestOppDist	=	-99999999,									//Best distance so far
			distToGoal_sq	=	bestOppDist,									//Distance to goal
			bestGoalDist	=	bestOppDist,								//Best distance so far
			points		=	0,
			bestPoints	=	-99999999,
			LocX,													//Location used in calcs
			LocY,													
			newX,													//Values passed back 
			newY,
			forward_points =	pow(playerRadius * 4, 2);					//Give additional points if moving forward
			

	playerRadius		=	27.5;

	for (int x = 0; x < 8; x++)											//Check 7 locations around player
	{
		switch (x)
		{
			case 0:													//Top left
				LocX		=	playerLoc.x	-	playerRadius;
				LocY		=	playerLoc.y	-	playerRadius;
				break;
			case 1:													//Top
				LocX		=	playerLoc.x;
				LocY		=	playerLoc.y	-	playerRadius;
				break;
			case 2:													//Top Right
				LocX		=	playerLoc.x	+	playerRadius;
				LocY		=	playerLoc.y	-	playerRadius;
				break;
			case 3:													//Left middle
				LocX		=	playerLoc.x	-	playerRadius;
				LocY		=	playerLoc.y;
				break;
			case 4:													//Right middle
				LocX		=	playerLoc.x	+	playerRadius;
				LocY		=	playerLoc.y;
				break;
			case 5:													//Bottom Left
				LocX		=	playerLoc.x	-	playerRadius;
				LocY		=	playerLoc.y	+	playerRadius;
				break;
			case 6:													//Bottom Middle
				LocX		=	playerLoc.x;
				LocY		=	playerLoc.y	+	playerRadius;
				break;
			case 7:													//Bottom Right
				LocX		=	playerLoc.x	+	playerRadius;
				LocY		=	playerLoc.y	+	playerRadius;
				break;
		}
		
		if	(	LocX		>	FIELD_WIDTH	||
				LocX		<	0			||
				LocY		>	FIELD_HEIGHT	||
				LocY		<	0)
				continue;												//Throw out case if invalid
		else
		{
			distToOpp_sq	=	pow(	LocX		-	opponentLoc.x, 2) +			//Calc squared dist to opponent
							pow(	LocY		-	opponentLoc.y, 2);
			distToGoal_sq	=	pow(	LocX		-	goalLoc.x, 2) +			//Calc squared dist to goal
							pow(	LocY		-	goalLoc.y, 2);

			if(distToOpp_sq	<	bestOppDist)
				bestOppDist	=	distToOpp_sq;
			
			if(distToGoal_sq	<	bestGoalDist)
				bestGoalDist	=	distToGoal_sq;
			
			points			=	distToOpp_sq	- distToGoal_sq;

			if (LocX			>	playerLoc.x)							//Add points for moving forward
				points		+=	forward_points;						
			else if(LocX		<	playerLoc.x)							//Remove points if moving backward
				points		-=	forward_points;

			if(points			>	bestPoints)
			{
				bestPoints	=	points;
				newX			=	LocX;
				newY			=	LocY;
			}
		}
	}

	return	D3DXVECTOR2			(newX, newY);
}


bool	Team::isOppentCollision		(Player *p, double oppRadius)
{
	D3DXVECTOR2	oppCenter,
				oppTeamHeading,
				playerCenter,
				playerHeading;
	double		playerRadius;
		
	for (int x = 0; x < NUM_PLAYERS; x++)
	{
		oppCenter	=	oppTeam[x].getCenter();
		oppTeamHeading = oppTeam[x].getTarget();
		playerCenter	=	p->getCenter();
		playerRadius	=	p->getRadius();
		playerHeading	=	p->getTarget();
		
		//Opposing player tests
		if (boundingBoxCollide(playerCenter, playerRadius, oppCenter, oppRadius))
			if (radiusDistCollide(playerCenter, playerRadius, oppCenter, oppRadius))
			{
				collisionReaction(p, playerCenter, playerRadius, &oppTeam[x], oppCenter, oppRadius);
				return true;
			}
	}

	return false;
}
void	Team::moveToGoal(Player *p, D3DXVECTOR2 goal)
{
	int stuckCollisionCounter = 0;
	static D3DXVECTOR2 prevTgt(goal);
	
	if (goalCollisionTimer->isTime())
		if (!isOppentCollision(p, p->getRadius() * 4))		//Measure collision using an inflated opponent radius					
			p->setTarget(goal);
	p->setPlayerState(ATTACK);
}
void	Team::handleBall		(Player	  *p, D3DXVECTOR2 pos)
{
	D3DXVECTOR2	curTgt,
				goalTgt,
				passTgt;
	
	pos			= p->getCenter					();						//Set position of current player
	curTgt		= p->getTarget					();
					
	if (isPlayerThreatened						(pos))					//Test if opponent is within threat range
	{
		passTgt	= supCalc->getPassShot			(p, players);				//Locate the best pass shot
		goalTgt	= supCalc->getGoalShot			(p);
			
		if (goalTgt	==	NO_SHOT)
			if (curTgt == passTgt)
				p->setPlayerState				(DRIBBLE_BALL);			//No pass or goal shot avail
			else
			{	oss	<< "\n\nplayer at " << p->getCenter().x << ", " << p->getCenter().y
					<< " passed to " << passTgt.x << ", " << passTgt.y;
				p->setTarget					(passTgt);				//Pass to teammate
				p->setPlayerState				(KICK_BALL);
				p->kickBall();
				hasBall				=		false;
			}
		else
		{
			p->setTarget						(goalTgt);				//Make Goal shot
			p->setPlayerState					(KICK_BALL);
			p->kickBall();
			hasBall					=		false;
		}
	}
	else																//No threat detected
	{
		goalTgt			= supCalc->getGoalShot	(p);						//Calc goal target
			
		if (goalTgt ==	NO_SHOT)											//If no goal region set
		{										
			if (moveGoalTimer->isTime			())						//Move toward goal
				if (homeTeam)
					moveToGoal				(p,	field->Goals[1]);		//Sets a new movement target
				else
					moveToGoal				(p,	field->Goals[0]);
		
			p->setPlayerState					(DRIBBLE_BALL);			//No goal shot yet, keep moving 
		}															// toward the goal
		else
		{
			p->setTarget						(goalTgt);				//Make Goal shot
			p->setPlayerState					(KICK_BALL);				//Goal region is set -- kick ball
			p->kickBall						();
			hasBall				=			false;
			
		}
	}
}
Player	*Team::getBallHolder		()
{	
	Player *p	= NULL;

	if (hasBall)
		for(int x = 0; x < NUM_PLAYERS; x++)
			if (players[x].playerHasBall())
			{
				p	=	&players[x];
				x	=	NUM_PLAYERS;
			}
	return p;
}

void Team::teamReceiveBall ()
{	
	for(int x = 0; x < NUM_PLAYERS; x++)
	{
		players[x].setX(homeRegions[x + RECEIVE].x);	
		players[x].setY(homeRegions[x + RECEIVE].y);
		players[x].setLocation();
		players[x].setCenter();
		players[x].setTarget(field->ball->getBallLocation());
		players[x].setPlayerState(WAIT);
	}
}

void Team::teamKickOff ()
{
	for(int x = 0; x < NUM_PLAYERS; x++)
	{
		players[x].setX(homeRegions[x + KICKOFF].x);	
		players[x].setY(homeRegions[x + KICKOFF].y);
		players[x].setLocation();
		players[x].setCenter();
		players[x].setTarget(field->ball->getBallLocation());
		players[x].setPlayerState(WAIT);
	}
}

Team::~Team()
{

	if (field)
		field	= NULL;

	if (players)
		delete []	players;

	if (supCalc)
		delete	supCalc;

	if (homeRegions)
		delete []	homeRegions;

	if (moveGoalTimer)
		delete	moveGoalTimer;

	if (supportRegTimer)
		delete	supportRegTimer;

	if (goalCollisionTimer)
		delete	goalCollisionTimer;

	playerTexture	= NULL;
	closestPlayer	= NULL;
	supportPlayer	= NULL;
	oppTeam		= NULL;
	closestOpponent= NULL;
	field		= NULL;
	xObj			= NULL;					

}


void	Team::DEBUG_SCRIPT	()
{
	int locX, locY;

	if (homeTeam)
	{
		locX = 0;
		locY = 0;
	}
	else
	{
		locX = 200;
		locY = 0;
	}

	/*	BALL TARGET DEBUGGER */
	
	oss	<< "Ball Location: " << field->ball->getBallLocation().x << ", " << field->ball->getBallLocation().y
		<< "\nBall Target: " << field->ball->getTarget().x	  << ", " << field->ball->getTarget().y;
			

	for (int x = 0; x < NUM_PLAYERS; x++)
	{
		oss	<< "\nPlayer:"		<<	x + 1
			<< "\nPlayer Pos: "	 << players[x].getX()			  << ", " << players[x].getY() 
			<< "\nPlayer Target: " << players[x].getTarget().x	  << ", " << players[x].getTarget().y
			<< "\nPlayer hasBall: " << players[x].playerHasBall()	  
			<< "\nSupporting Reg: " << SupportingRegion.x		  << ", " << SupportingRegion.y
			<< "\nPlayer State: ";

		switch (players[x].getPlayerState())
		{
			case ATTACK:
				oss << "ATTACK";
				break;
			case PURSUE_TGT:
				oss << "PURSUE_TGT";
				break;
			case DEFEND:
				oss << "DEFEND";
				break;
			case SUPPORT:
				oss << "SUPPORT";
				break;
			case KICK_BALL:
				oss << "KICK_BALL";
				break;
			case DRIBBLE_BALL:
				oss << "DRIBBLE_BALL";
				break;
			case AT_TARGET:
				oss << "AT_TARGET";
				break;
			case WAIT:
				oss << "WAIT";
				break;
			case USER_CONTROL:
				oss << "USER_CONTROL";
				break;
			default:
				oss << "UNKNOWN";
				break;
		}
	}

	xObj->FontPrint(font, locX, locY, oss.str(), D3DXCOLOR(0,0,0,255));

	oss.str("");
}