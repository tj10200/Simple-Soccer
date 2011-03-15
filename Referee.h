#ifndef REFEREE_H
#define REFEREE_H

#include "GameHeaders.h"
#include "DirectX.h"
#include "PlayField.h"
#include "Team.h"
#include "Player.h"
#include "Timer.h"

class Referee
{
	private:
		Player		*ballHolder;
		D3DXVECTOR2	playerPrevLoc;									//Used for testing movement
		D3DXVECTOR2	playerLoc;									// changes over time
		D3DXVECTOR2	centerField;									//Moves ball here if players are stuck	

		Field		*field;

		Team			*homeTeam;									//The home team
		Team			*awayTeam;									//The away team

		double		changeInMovement;								//Testing movement changes
		
		Timer		*movementTimer;								//Used for timing the movement tests
		Timer		*gameClock;									//Used to countdown to the end of the game

		int			homeGoals,									//Tracks home goals
					awayGoals,									//Tracks away goals
					testedPlayer;									//The players index
		
		bool			positionChecked;
		bool			gameStarted;									//Allows the clock to be initialized
		bool			gameEnded;									//Prevents the clock from being restarted 

		ostringstream	goalString;									//Holds the string that will be pushed into font
		ostringstream	gameClockString;								//Holds the string that will be pushed into font
		

		LPD3DXFONT	goalFont;										//Font used to display goal information
		LPD3DXFONT	gameClockFont;									//Font used to display the game time remaining	
		LPD3DXFONT	gameOverFont;										//Font used to display game over message

		DirectX		*xObj;										//The Dx class pointer

		void			homeScored				()					{homeGoals		++;}//Increase home scores
		void			awayScored				()					{awayGoals		++;}//Increase away score\s
		
		void			setBallHolderInitPos		()					{if (ballHolder)	playerPrevLoc	= ballHolder->getCenter();}
		void			setballHolderPos			()					{if (ballHolder)	playerLoc		= ballHolder->getCenter();}
		void			checkMovements				();
		
	public:
		Referee								();
		~Referee								();

		//Sets and Gets for private members
		void			setPlayerPrevPos			(D3DXVECTOR2	loc)		{playerPrevLoc		=	loc;}
		D3DXVECTOR2	getPlayerPrevPos			()					{return				playerPrevLoc;}
		void			setPlayerPos				(D3DXVECTOR2	loc)		{playerLoc		=	loc;}
		D3DXVECTOR2	getPlayerPos				()					{return				playerLoc;}
		void			setHomeTeam				(Team		*t)		{homeTeam			=	t;}
		Team			*getHomeTeam				()					{return				homeTeam;}
		void			setAwayTeam				(Team		*t)		{awayTeam			=	t;}
		Team			*getAwayTeam				()					{return				awayTeam;}
		void			setDirectXObj				(DirectX		*x)		{xObj			=	x;}
		DirectX		*getDirectXObj				()					{return				xObj;}
		void			setGoalFont				()					{goalFont			=	xObj->MakeFont("Arial", 22);}
		void			setGameClockFont			()					{gameClockFont		=	xObj->MakeFont("Arial", 22);}
		void			setGameOverFont			()					{gameOverFont		=	xObj->MakeFont("Arial", 100);}
		void			setBallHolder				();
		Player		*getBallHolder				()					{return				ballHolder;}
		void			setTestedPlayer			(int			t)		{testedPlayer		=	t;}
		int			getTestedPlayer			()					{return				testedPlayer;}
		void			setField					(Field		*f)		{field			=	f;}
		Field		*getField					()					{return				field;}
		bool			isGameStarted				()					{return				gameStarted;}
		void			startGame					();
		bool			isGameEnded				()					{return				gameEnded;}
		void			endGame					()					{gameEnded		=	true;}

		//Referee field tests
		void			updateAllChecks			();
		bool			noMovementChange			();
		void			checkScores				();
		void			checkBall					();
		void			checkTime					();
		
		//Rendering function
		void			DrawScoreBoard				();
		void			DrawGameClock				();
		void			DrawGameOver				();

};

#endif