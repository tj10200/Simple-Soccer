#include "Referee.h"

Referee::Referee()
{
	homeGoals		=	0;
	awayGoals		=	0;
	changeInMovement =	0;
	positionChecked = false;									
	movementTimer	=	new Timer(10000);						//Set timer to 10 sec.

	gameStarted	=	false;
	gameEnded		=	false;

	
	centerField	=	D3DXVECTOR2	((float)(FIELD_WIDTH	/ 2  + 50),
								 (float)(FIELD_HEIGHT	/ 2  + 50));
	ballHolder	=	NULL;
	homeTeam		=	NULL;
	awayTeam		=	NULL;
	field		=	NULL;
	goalFont		=	NULL;
	gameClockFont	=	NULL;
	gameClock		=	NULL;
	gameOverFont	=	NULL;
	
}

void	Referee::updateAllChecks	()
{
	checkMovements			();
	
	if (field->gameOn)													
		checkScores		();

	if (field->gameOn)													//Only check when ball is in play
		checkBall			();	

	if (field->gameOn)
		checkTime			();
	else if (gameClock)
		gameClock->pauseTimer();

}


void	Referee::setBallHolder	()
{
	if (homeTeam)
	{
		if (homeTeam->getTeamHasBall())
			ballHolder	= homeTeam->getBallHolder();
	}
	else if(awayTeam)
	{
		if (awayTeam->getTeamHasBall())
			ballHolder	= awayTeam->getBallHolder();
	}
	else
		ballHolder		= NULL;

}

bool Referee::noMovementChange()
{
	double	xDist	=	playerLoc.x - playerPrevLoc.x,
			yDist	=	playerLoc.y - playerPrevLoc.y,
			dist		=	sqrt(pow(xDist, 2) + pow(yDist, 2));

	if (dist			<	MIN_MOVEMENT)
		return		true;
	else
		return		false;
}
void	Referee::checkMovements	()
{
	setBallHolder();

	if (ballHolder)
	{	
		if (!positionChecked)
		{
			setBallHolderInitPos();
			positionChecked = true;
		}
		else
			setballHolderPos();

	}
}
void	Referee::checkScores	()
{
	//Home Team Check
	if (field->ball->getY() >	field->Goals[0].y	&&
	    field->ball->getY() < (	field->Goals[0].y	+ 
							GOAL_HEIGHT		- 
							field->ball->getHeight()
						)
	   
	)
	{

		//Home Team Check
		if ((field->ball->getX() + field->ball->getWidth())	>=	FIELD_WIDTH)
		{
			homeGoals ++;
			field->resetBall();	
			homeTeam->teamReceiveBall();
			awayTeam->teamKickOff();
		}
		else if (field->ball->getX()	<=	50)
		{		
			awayGoals ++;
			field->resetBall();
			homeTeam->teamKickOff();
			awayTeam->teamReceiveBall();
		}
	}	

}

void Referee::checkBall		()
{

	if (field->inPlayTimer->isTime() && field->gameOn)
		field->gameOn = field->IsBallInField();
	
	if (!field->gameOn)
	{
		homeTeam->teamReceiveBall();
		awayTeam->teamReceiveBall();
		field->resetBall();
	}
		
}
void	Referee::checkTime		()
{
	if (gameStarted && !gameEnded)
		if (gameClock->isTime())
			gameEnded		=	true;

}
void	Referee::DrawScoreBoard	()
{
	goalString.str("");

	goalString	<<	"Score Board";

	xObj->FontPrint	(goalFont, 
					SCORE_BOARD_HEADER.x, 
					SCORE_BOARD_HEADER.y, 
					goalString.str(), 
					D3DXCOLOR(255,255,0,255)
					);
	goalString.str("");

	goalString	<<	"Orange Team"
				<<	"\n"
				<<	homeGoals;

	xObj->FontPrint	(goalFont, 
					SCORE_BOARD_HOMETEAM.x, 
					SCORE_BOARD_HOMETEAM.y, 
					goalString.str(), 
					D3DXCOLOR(255,255,0,255)
					);
	goalString.str("");
	goalString	<<	"Blue Team"
				<<	"\n"
				<<	awayGoals;

	xObj->FontPrint	(goalFont, 
					SCORE_BOARD_AWAYTEAM.x, 
					SCORE_BOARD_AWAYTEAM.y, 
					goalString.str(), 
					D3DXCOLOR(255,255,0,255)
					);


	
}
void Referee::DrawGameClock	()
{
	if (!gameStarted)
		return;
	
	if (gameClock		==	NULL)
		return;

	//Declare vars once clock is initialized
	int	min	=	gameClock->getMinutesRemaining(),
		sec	=	gameClock->getSecondsRemaining(),
		mili	=	gameClock->getMilisRemaining();

	
	gameClockString.str("");
	
	if (min	<	10)
		gameClockString	<<	"0";

	gameClockString		<<	min
						<<	":";

	if (sec	<	10)
		gameClockString	<<	"0";

	gameClockString		<<	sec
						<<	":";

	if (mili	<	100)
		if (mili	<	10)	
			gameClockString <<	"00";
		else
			gameClockString <<	"0";

	gameClockString		<<	mili;

	

	
	xObj->FontPrint		(gameClockFont, 
						GAME_TIMER_PLACEMENT.x, 
						GAME_TIMER_PLACEMENT.y, 
						gameClockString.str(), 
						D3DXCOLOR(255,255,255,255)
						);	

}
void	Referee::DrawGameOver	()
{
	string gameOver;

	gameOver			=	"GAME OVER";

	if (homeGoals		>	awayGoals)
		gameOver		+=	"\nYou Win";
	else if(homeGoals	<	awayGoals)
		gameOver		+=	"\nYou Lose";
	else
		gameOver		+=	"\nDraw";


	xObj->FontPrint		(gameOverFont, 
						GAME_OVER_PLACEMENT.x, 
						GAME_OVER_PLACEMENT.y, 
						gameOver, 
						D3DXCOLOR(255,255,255,255)
						);	

}
void	Referee::startGame()
{
	gameStarted		=	true;
	gameClock			=	new Timer(GAME_TIME);				//Set interval to 1 sec * 60 seconds * 3 minutes	=  18000 miliseconds
	
	if (gameClock)
		DrawGameClock	();
}
Referee::~Referee()
{
	ballHolder	=	NULL;
	xObj			=	NULL;
	field		=	NULL;
	homeTeam		=	NULL;
	awayTeam		=	NULL;

	if (movementTimer)
		delete		movementTimer;

	if (goalFont)
		goalFont->Release();

	if (gameClock)
		delete		gameClock;

	if (gameClockFont)
		gameClockFont->Release();

	if (gameOverFont)
		gameOverFont->Release();
}