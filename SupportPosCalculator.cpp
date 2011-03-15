#include "SupportPosCalculator.h"

D3DXVECTOR2	SupportPosCalculator::getBestSupportPos()
{
	double	LargestPoints		= 0,
			tempPoints		= 0;
	int		largeX			= 0, 
			largeY			= 0;
	
	for(int x = 0; x < rows; x++)
	{
		for(int y = 0; y < cols; y++)
		{	
			tempPoints		= passPotential[x][y] + goalPotential[x][y];	//Temporary placeholder

			if (tempPoints		> LargestPoints)						//If larger value found
			{													// then reinit values
				LargestPoints	= tempPoints;			
				
				if (largeX > rows)
					largeX	=	rows / 2;
				if (largeY > cols)
					largeY	=	cols / 2;

				largeX		= x;
				largeY		= y;
			}
		}
	}

	return D3DXVECTOR2 ((float)regions[largeX][largeY].x, (float)regions[largeX][largeY].y);
}

void	SupportPosCalculator::calcSupportingRegions(Player *p)
{
	double		points			= 0,
				distToTgt			= 0,
				bodyLength;
	
	int			numGoalRegions		=((goal.y + GOAL_HEIGHT) - goal.y) / REG_HEIGHT;			//Define the number of regions in 
																				// the goal area to test
	D3DXVECTOR2	startPos,
				endPos,
				ballCarrierPos;

	ballCarrierPos					= p->getCenter();									//start with ball carrier location
	bodyLength					= p->getRadius() * 2;								//Calculate diameter of player

	for(int x = 0; x < rows; x++)
	{
		for(int y = homeTeamIdxOffset; y < ceil((double)(cols / 2 + homeTeamIdxOffset)); y++)	//Calculate only goal half of field
		{	
			startPos				= ballCarrierPos;
			endPos				= regions[x][y];									//End with the region

			if (isShotSafe	(startPos, endPos))											//Test pass to region
				points			++;												//1 point for possible pass

			distToTgt				= pow(endPos.x - startPos.x, 2) + 
								  pow(endPos.y + startPos.y, 2);

			if (distToTgt			> MIN_PASS_DIST &&
			    distToTgt			< MAX_PASS_DIST)
				points			+= sqrt(distToTgt) / bodyLength;						//Calc body lengths from opponent
																				//1 point for each body length
			startPos				= endPos;											//Reset start position
																				// at region
			for(int w = 0; w < numGoalRegions; w++)										//Test goal passing potential
			{
				endPos.x		=	goal.x;											
				endPos.y		=	goal.y	+	(x	*	REG_HEIGHT);
				
				if (isShotSafe (startPos,	endPos))
					points++;														//1 point for each possible
			}																	// goal shot
			
			supportRegions[x][y]	= points;

			points				= 0;
		}
	}

	regionsAvail = true;
}

D3DXVECTOR2 SupportPosCalculator::getSupportPos (Player *p, Player *ball)
{
	D3DXVECTOR2	playerPos		=	p->getCenter(),
				ballPos		=	ball->getCenter(),
				testPos;

	double		tempPoints	=	0,
				largePoints	=	0,
				tempDist		=	999999999,
				bestDist		=	999999999;
	int			bestX		=	0,
				bestY		=	0;	
	
	for(int x = 0; x < rows; x++)
	{
		for(int y = homeTeamIdxOffset; y < ceil((double)(cols / 2 + homeTeamIdxOffset)); y++)	//Calculate only goal half of field
		{
			tempPoints	=	supportRegions[x][y];
			
			if (tempPoints >	largePoints)
				largePoints	=	tempPoints;
		}
	}

	for(int x = 0; x < rows; x++)
	{
		for(int y = homeTeamIdxOffset; y < ceil((double)(cols / 2 + homeTeamIdxOffset)); y++)	//Loop again looking for largest point regions
		{
			tempPoints	=	supportRegions[x][y];
			
			if (tempPoints  ==	largePoints)
			{			
				testPos		=	regions[x][y];
				tempDist		=	pow(testPos.x - playerPos.x, 2) + 
								pow(testPos.y - playerPos.y, 2);
				if (tempDist	<	bestDist)
				{
					bestDist	=	tempDist;											//Find shortest distance
					bestX	=	x;
					bestY	=	y;
				}
			}
		}
	}
				
	return D3DXVECTOR2 ((float)regions[bestX][bestY].x, (float)regions[bestX][bestY].y);
}
void	SupportPosCalculator::passPotentialCalc()
{
	double		points		= 0,
				dist			= 0,
				bodyLength_sq	= pow(oppTeam[0].getRadius() * 2, 2);
	D3DXVECTOR2	attacker;

	for(int x = 0; x < rows; x++)
	{
		for(int y = 0; y < cols; y++)
		{	
			for (int w = 0; w < numPlayers; w++)
			{
				attacker	=	oppTeam[w].getCenter();							//Set attacker to opposing player
				dist		+=	pow(attacker.x - regions[x][y].x, 2) + 
							pow(attacker.y - regions[x][y].y, 2);				//Add Squared dist to target
										
			}
			
			dist			/=	bodyLength_sq;									//Get number of body lengths
			
			points		+=	dist;										//Add points for distance from targets

			dist			=	0;

			
			dist			-=	pow(regions[rows/2][cols/2].x - regions[x][y].x, 2) + 
							pow(regions[rows/2][cols/2].y - regions[x][y].y, 2);	//Subtract squared dist to middle field
			dist			-=	pow(goal.x - regions[x][y].x, 2) + 				//Subtract squared dist to goal
							pow(goal.y - regions[x][y].y,2);

			passPotential[x][y] = dist;										//Add dist as points to pass potential metric
		}
		
		points = 0;														//Reinit points
		dist	  = 0;														//Reinit Dist
	}
}

double SupportPosCalculator::getPassPotential(int regX, int regY)
{
	double		points = 0,
				dist	  = 0;
	D3DXVECTOR2	attacker;

	for (int w = 0; w < numPlayers; w++)
	{
		attacker	=	oppTeam[w].getCenter();						//Set attacker to opposing player
		dist		+=	pow(attacker.x - regions[regX][regY].x, 2) +			//Add Squared dist to target
					pow(attacker.y - regions[regX][regY].y, 2);	
	}
		
	return dist;													//Add dist as points to pass potential metric
}
void	SupportPosCalculator::goalPotentialCalc()
{
	double		points = 1,
				m	  = 0,
				intercect = 0;
	D3DXVECTOR2	attacker;

	for(int x = 0; x < rows; x++)
	{
		for(int y = 0; y < cols; y++)
		{	
			m		= (goal.y - regions[x][y].y)/(goal.x - regions[x][y].x);//Get slope to goal
			attacker	= oppTeam[x].getCenter();

			for (int w = 0; w < numPlayers; w++)
			{
				intercect		= attacker.y / m;						//Point where attacker would intercect the line to goal
				
				if (intercect <= regions[x][y].x || intercect >= goal.x)
					points += pow(attacker.x - regions[x][y].x, 2) + pow(attacker.y - regions[x][y].y, 2); //Add square dist
			}
			
			goalPotential[x][y] = points;								//Add dist as points to pass potential metric
		}
		
		points = 1;												//Reinit Dist
	}

}


double SupportPosCalculator::getGoalPotential(int regX, int regY)
{
	double		points = 1,
				m	  = 0,
				intercect = 0;
	D3DXVECTOR2	attacker;

	for (int w = 0; w < numPlayers; w++)
	{
		m		= (goal.y - regions[regY][regX].y)/(goal.x - regions[regY][regX].x);//Get slope to goal
		attacker	= oppTeam[w].getCenter();
		intercect		= attacker.y / m;								//Point where attacker would intercect the line to goal
		
		if (intercect <= regions[regY][regX].x || intercect >= goal.x)		//If intercect is < current loc or passed the goal
			points *= pow(attacker.x - regions[regY][regX].x, 2) + pow(attacker.y - regions[regY][regX].y, 2); //Mult by square dist
	}

	return points;

}

Player *SupportPosCalculator::getClosestTeammate(Player p, Player *team)
{
	double tempDist	= 0,											//Holds the distance during each iter
		  lgDist		= 0;											//Holds the largest distance
	int	  largeX;
	for (int x = 0; x < numPlayers; x++)
	{
		tempDist	=	pow(p.getCenter().x - team[x].getCenter().x, 2) + 
					pow(p.getCenter().y - team[x].getCenter().y, 2);

		if (tempDist > lgDist)
		{
			lgDist =	tempDist;
			largeX =	x;
		}
	}

	return &team[largeX];
}
Player *SupportPosCalculator::getClosestToTgt (Player *team, D3DXVECTOR2 tgt)
{
	double		tempDist		= 0,									//Holds the distance during each iter
				bestDist		= 999999999;							//Holds the best distance
	int			largeX;											//X value of closest player
	D3DXVECTOR2	playerLoc;										//Location of teammate
		
	for (int x = 0; x < numPlayers; x++)
	{
		if (team[x].getCenter()		!=	tgt)							//Do not calc for player with ball
		{
			tempDist	=	pow(tgt.x - team[x].getCenter().x, 2) + 
						pow(tgt.y - team[x].getCenter().y, 2);

			if (tempDist < bestDist)
			{
				bestDist	=	tempDist;
				largeX	=	x;
			}
		}
	}

	return &team[largeX];
}
void SupportPosCalculator::setBallCarrier()
{
	for (int x = 0; x < numPlayers; x++)
		if (teammate[x].playerHasBall())
			ballCarrier = &teammate[x];

}

//Cycle through all goal regions to see if safe for shot
D3DXVECTOR2 SupportPosCalculator::getGoalShot(Player *p)
{
	int numGoalRegions	=	((goal.y + GOAL_HEIGHT) - goal.y) / REG_HEIGHT;
	D3DXVECTOR2			endLoc,
						startLoc	=	p->getCenter(),
						prevTgt	=	goal,
						noShot		(0.0f, 0.0f);
	double				distToTgt_sq;
	
	//if (	pow(goal.x - startLoc.x, 2)	>	GOAL_SHOT_RANGE)				//Return original value if 
	//	return NO_SHOT;											// outside of minimum possible
																// distance
	for(int x = 0; x < numGoalRegions; x++)
	{
		endLoc.x		=	goal.x;
		endLoc.y		=	goal.y	+	(x	*	REG_HEIGHT) + 25;
		
		distToTgt_sq	=	pow(startLoc.x	- endLoc.x, 2) +
						pow(startLoc.y - endLoc.y, 2);

		if (distToTgt_sq	> GOAL_SHOT_DIST && p->getPlayerState() != USER_CONTROL)
			continue;

		if (isShotSafe (startLoc,	endLoc))
			return	endLoc;
	}
	
	return	NO_SHOT;
}
//Cycle through all goal regions to see if safe for shot
D3DXVECTOR2 SupportPosCalculator::getPassShot(Player *p, Player *team)
{
	D3DXVECTOR2			endLoc,
						startLoc	=	p->getCenter(),
						prevTgt	=	p->getTarget(),
						closeTgt	=	NO_SHOT,
						newTgt	=	NO_SHOT;
	double				distToTgt_sq,
						bestDist	=	999999999;
	
	for(int x = 0; x < numPlayers; x++)
	{
		endLoc		=	team[x].getCenter();
		
		if (startLoc.x		==	endLoc.x &&
		    startLoc.y		==	endLoc.y)										//Do not calc for self
			continue;
		

		distToTgt_sq	=	pow(startLoc.x	- endLoc.x, 2) +
						pow(startLoc.y - endLoc.y, 2);
		
		if (distToTgt_sq <	MIN_PASS_DIST)
			continue;

		if (distToTgt_sq < bestDist)
		{
			bestDist	=	distToTgt_sq;
			closeTgt	=	endLoc;
		}

		if (isShotSafe (startLoc,	endLoc))
			return	endLoc;
	}

	if (closeTgt	==	NO_SHOT)
	{
		if (p->getCenter() == team[0].getCenter())
			closeTgt		= team[1].getCenter();
		else
			closeTgt		= team[0].getCenter();
	}

	return closeTgt;
}
bool	SupportPosCalculator::isShotSafe(D3DXVECTOR2 startLoc, D3DXVECTOR2 endLoc)
{
	double		run		=	endLoc.x - startLoc.x,
				rise		=	endLoc.y - startLoc.y,
				slope,
				threatRange =	oppTeam[0].getRadius(),
				testDist;
				
	D3DXVECTOR2	oppLoc,
				testLoc;
				
	if (run		==	0)
		run		=	1;
	
	slope		=	rise / run;

	for (int x	= 0; x < numPlayers; x++)
	{
		oppLoc	=	oppTeam[x].getCenter();
		oppLoc.y	=	oppLoc.x	*	slope;
		testLoc.y	=	oppLoc.x *	slope;
		testLoc.x =	oppLoc.x;
		
		if (testLoc.y		==	0)
			continue;
		
		if ((oppLoc.x		>	endLoc.x		&&							//opponent is not between two elements
			oppLoc.x		>	startLoc.x)	||
			(oppLoc.x		<	endLoc.x		&&
			oppLoc.x		<	startLoc.x))
			continue;
		else if ((oppLoc.y	>	startLoc.y	&&
			    oppLoc.y	>	endLoc.y)		||
			    (oppLoc.y	<	startLoc.y	&&
			    oppLoc.y	<	endLoc.y))
			    continue;
		else
			return false;
		

	}

	return true;											//All opponents safe dist from shot
}
