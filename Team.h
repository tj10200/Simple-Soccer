#ifndef TEAM_H
#define TEAM_H

#include "gameHeaders.h"
#include "Player.h"
#include "Ball.h"
#include "PlayField.h"
#include "DirectX.h"
#include "SupportPosCalculator.h"



enum			REGIONS
{	
			NEUTRAL = 0,								//All regions are multiple of NUM_PLAYERS in order to
			DEFENCE = 1 * NUM_PLAYERS,					// use as offset in home regions array
			OFFENCE = 2 * NUM_PLAYERS,
			KICKOFF = 3 * NUM_PLAYERS,
			RECEIVE = 4 * NUM_PLAYERS
};

class Team
{
	private:
		Player				*players;					//Player objects on team
		Player				*closestPlayer;			//Pointer for player that is closest to ball. Used for supporting calcs
		Player				*supportPlayer;			//Pointer to supporting player
		Player				*ballHandler;				//Player that has the ball
		Player				*receiver;				//Player that was passed to			
		Player				*goalie;					//Will hold pointer for goalTending Player. Will prevent player state changes 
		Player				*oppTeam;					//Players on opposing team
		Player				*closestOpponent;

		Field				*field;					//The playing field
		DirectX				*xObj;					//Used to draw direct x objects
		
		SupportPosCalculator	*supCalc;					//Calculates the best supporting shot

		LPDIRECT3DTEXTURE9		playerTexture;				//Sprite Sheet used for players
		D3DXVECTOR2			*homeRegions;
		D3DXVECTOR2			SupportingRegion;			//Holds the region that a support player will use
		bool					homeTeam;					//Is home team
		bool					hasBall;					//Team has the ball
		bool					isCollisionFound;			//Used to halt player movement
	
		
		void					setClosestPlayer();
		bool					isPlayerThreatened(D3DXVECTOR2	playerLoc);//Test if player should move toward goal, or 
															  // pass/shoot
		Player				*getClosestPlayer(	D3DXVECTOR2	playerLoc, 
											Player		*team);	  //Get opponent closest to player
		D3DXVECTOR2			 getBestBallMovement(D3DXVECTOR2	playerLoc, 
											 D3DXVECTOR2	goalLoc,
											 D3DXVECTOR2	oppLoc,
											 double		playerRad
											 );
		Timer				*moveGoalTimer;
		Timer				*supportRegTimer;
		Timer				*goalCollisionTimer;
		void					handleBall		(Player		*p,
											 D3DXVECTOR2	pos);
		
		void					DEBUG_SCRIPT		();					
		
		/*DEBUG CODE*/
			LPD3DXFONT font;
			std::ostringstream	oss;
		/*DEBUG CODE*/
	
	public:
		Team(LPDIRECT3DTEXTURE9	playerTex,
			Field			*playField,
			DirectX			*xDevice,
			bool				isHomeTeam)				
													{	
														players			= new Player[NUM_PLAYERS];
														playerTexture		= playerTex;
														field			= playField;
														xObj				= xDevice;
														homeTeam			= isHomeTeam;
														hasBall			= false;

														InitHomeRegions();
														InitPlayers	();
														
														font				= xObj->MakeFont("Arial", 12);
														isCollisionFound	= false;
														srand(time(NULL));
														moveGoalTimer		= new Timer(100);
														supportRegTimer	= new Timer(1000);
														goalCollisionTimer	= new Timer(1000);


														closestPlayer		= NULL;
														supportPlayer		= NULL;
														ballHandler		= NULL;	
														receiver			= NULL;
														goalie			= NULL;
														oppTeam			= NULL;
														closestOpponent	= NULL;
														supCalc			= NULL;
													}
		~Team();
		void	InitPlayers		();						//Creates teammates
		void InitHomeRegions	();						//Inits regions used for home starting position
		void InitSupportCalc	();						//Inits supporting shot calc
		void DrawTeam			();						//Draws team to screen
		void	setPlayerStates	();
		void	setTeamHasBall		();						//Cycle through team to see if has ball
		void cyclePlayerStates	();						//Send message to player indicating that should act
		void	checkForCollision	(Player		*p);			//Checks for and resolves collision
		bool	isCollision		(Player		*p);			//Tests a player to see if they will collide with another player
		bool boundingBoxCollide	(D3DXVECTOR2	center,		//Tests bounding box for collion
							 double		radius, 
							 D3DXVECTOR2	testCenter, 
							 double		testRadius);				
		bool radiusDistCollide	(D3DXVECTOR2	center,		//Tests actual radius for collision. Removes "fat" from bounding box
							 double		radius, 
							 D3DXVECTOR2	testCenter, 
							 double		testRadius);			
		bool	playerInBack		(D3DXVECTOR2	center, 
							 D3DXVECTOR2	target, 
							 D3DXVECTOR2	testCenter, 
							 D3DXVECTOR2	testTarget);	// a reaction is necessary
		void collisionReaction	(Player		*player,		
							 D3DXVECTOR2	center,		
							 double		radius, 
							 Player		*testPlayer,	
							 D3DXVECTOR2	testCenter,	
							 double		testRadius);
		void collisionReaction	(Player		*p,			//Sets new target for colliding player
							 Player		*t);
		void	moveToGoal		(Player		*p,			//Moves a player toward the goal
							 D3DXVECTOR2	goal);		// while avoiding the player
		bool isOppentCollision	(Player		*p,			//Tests a collision using a specified
							 double		oppRadius);	// radius

		void		PlayerKickOff	();						//Moves players toward region set for them.
		void		teamReceiveBall();						//Sets team into receive position
		void		teamKickOff	();						//Sets team into kickoff position
		
		//Get next player for user switch
		Player	*getNextPlayer	(int		&curIdx)				{	
															curIdx		++;
															curIdx		=	curIdx % NUM_PLAYERS;
															return			&players[curIdx];
														}
		


		//Variable sets and returns
		void		setOpposingPlayers	(Player *OppTeam)			{	oppTeam		=	OppTeam;}
		Player	*getOpposingPlayers	()						{	return			oppTeam;}
		void		setPlayers		(Player *teamMates)			{	players		=	teamMates;}
		Player	*getPlayers		()						{	return			players;}
		void		setTeamHasBall		(bool	b)				{	hasBall		=	b;}
		bool		getTeamHasBall		()						{	return			hasBall;}
		Player	*getBallHolder		();
		D3DXVECTOR2 getGoalShot		(Player *p)				{	return			supCalc->getGoalShot(p);}
		D3DXVECTOR2 getPassShot		(Player *p)				{	return			supCalc->getPassShot(p, players);}
	
		
};

#endif











