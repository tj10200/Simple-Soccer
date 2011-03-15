#include "GameEngine.h"
//*********************************************************************
//	Function: Game_Init
//	Vars:	window	-	The Window handle
//	Process:	Initializes all game pointer variables
//	Returns:	True if successful
//********************************************************************
bool Game::Game_Init(HWND window)
{
	//Seed Random
	srand(time(NULL));										//Used in ball kicks
	
	//Initialize Direct3D Objects
	xObj->Direct3D_Init		(window,							//Window Handle
						 screenWidth,						//Width and height to resize window
						 screenHeight,
						 false							//No Full screen
						 );
	//Initialize Controllers
	xObj->DirectInput_Init	(window);

	CreateBackground		();								//Create background image object
	CreateSprites			();
	CreateReferee			();

	
	helpFont	= xObj->MakeFont("Arial", 18);
	helpStr	<< "Keyboard controls: "
			<< "\n<Return> - Begin Game / Resume"
			<< "\n<Space> - Pause Game"
			<< "\nR - Reset Ball"
			<< "\nA - Switch Player"
			<< "\nF - Pass Ball (Hold)"
			<< "\nG - Goal Shot (Hold)";

	return true;
}

//********************************************************************
//	Function:	Game_Run
//	Vars:	window	-	 The window Handle
//	Process:	This function handles all of the updating and rendering
//			 of the game objects during each frame of animation
//	Returns:	None
//********************************************************************
void Game::Game_Run(HWND window)
{

	if (!xObj->getDevice()) return;								//make sure the Direct3D device is valid
	
	xObj->DirectInput_Update();									//update input devices

	xObj->ClearScene();											//Clear all screen objects before repainting

	if (!ref->isGameEnded())
		CalcSprites();											//Calculate new positions
	
	if (xObj->beginDev())										//Start rendering
    {
			
		xObj->BeginSpriteDraw();									//Set screen lock 

		DrawBackground();										//Plack background onto scene
	
		DrawSprites();
		
		xObj->EndSpriteDraw();									//Remove screen lock
	
		xObj->endDev();										//Stop Rendering
	}

    if (xObj->Key_Down(DIK_ESCAPE))								//Check if escape key is pressed
		gameover	= true;										// exit on escape key

    	if (xObj->Key_Down(DIK_SPACE))
		if (inputTimer->isTime())
			if (paused)
				paused = false;
			else 
				paused = true;

	if (xObj->Key_Down(DIK_RETURN))
	{
		if (!ref->isGameStarted())
			ref->startGame();

		paused		= false;
		field->gameOn	= true;
	}
	if (xObj->Key_Down(DIK_R))
	{
		field->gameOn	=	false;
		ref->checkBall();
	}
}

//***********************************************************************
//	Function:	~Game
//	Vars:	None
//	Process:	Deconstructor - Destroys all initialized memory objects
//	Returns:	None
//***********************************************************************
Game::~Game()
{
	if (homePlayers)
		delete homePlayers;

	if (awayPlayers)
		delete awayPlayers;

	if (xObj)
		delete xObj;

	if (field)
		delete field;

	if (frameTimer)
		delete frameTimer;

	if (playerTimer)
		delete playerTimer;

	if (ref)
		delete ref;

	if (uc)
		delete uc;

}
//***********************************************************************
//	Function: CreateBackground
//	Vars:	None
//	Process:	This function creates and loads a background image
//	Returns:	None
//***********************************************************************
void Game::CreateBackground()
{
	srcRect.top		= 0;											//Top Horizontal
	srcRect.left	= 0;											//Top Vertical
	srcRect.right	= screenWidth;									//Right horizontal
	srcRect.bottom	= screenHeight;									//Right vertical
		
	dstRect.top		= 0;											//Top Horizontal
	dstRect.left	= 0;											//Top Vertical
	dstRect.right	= screenWidth;									//Right horizontal
	dstRect.bottom	= screenHeight;									//Right vertical

	backGround	= xObj->LoadSurface(bGroundPath);					//Load background pointer
		
	
}

void	Game::CreateReferee()
{
	ref	=	new Referee();
	ref->setHomeTeam	(homePlayers);
	ref->setAwayTeam	(awayPlayers);
	ref->setField		(field);
	ref->setDirectXObj	(xObj);
	ref->setGoalFont	();
	ref->setGameClockFont();
	ref->setGameOverFont();

}
void	Game::CreateSprites()
{
	playerTexture	= xObj->LoadTexture	(spritePath);
	ballTexture	= xObj->LoadTexture	(ballPath);

	field->InitRegions();
	field->ball->setTexture	(ballTexture);								//Set the texture for the ball
	
	homePlayers		=	new Team(playerTexture, field, xObj, true);				//Create home players
	awayPlayers		=	new Team(playerTexture, field, xObj, false);				//Create away players

	//Set opposing players on both sides
	homePlayers->setOpposingPlayers	(awayPlayers->getPlayers());
	awayPlayers->setOpposingPlayers	(homePlayers->getPlayers());

	homePlayers->InitSupportCalc		();
	awayPlayers->InitSupportCalc		();

	uc->setTeam					(homePlayers);
	uc->switchPlayer				();
	uc->setDirectXDev				(xObj);
}
//*********************************************************
//	Function:	DrawBackground
//	Vars:	None
//	Process:	Calls upon the Draw surface function to 
//			 specifically draw the background surface
//	Returns:	None
//**********************************************************
void Game::DrawBackground()
{
	xObj->DrawSurface (xObj->getBackBuffer(), 0, 0, backGround);
}

void Game::CalcSprites()
{
	if (playerTimer->isTime() && !paused)
	{
		ref->updateAllChecks();
		field->updateBall();										//move ball to new location

		if (field->gameOn)
		{
			
			uc->ballHandlerSwitch ();									//Move player to ball controller

			uc->cycleStates();
			
			homePlayers->setPlayerStates();
			awayPlayers->setPlayerStates();


		}
		else
		{
			homePlayers->cyclePlayerStates();
			awayPlayers->cyclePlayerStates();
		}
	}
}
void Game::DrawSprites()
{
	homePlayers->DrawTeam();
	awayPlayers->DrawTeam();

	xObj->Sprite_Transform_Draw(
							field->ball->getTexture(),
							field->ball->getScale(),
							field->ball->getX(),
							field->ball->getY(),
							field->ball->getRotation(),
							field->ball->getWidth(),
							field->ball->getHeight(),
							field->ball->getFrame(),
							field->ball->getColumn(),
							field->ball->getColor()
						);

	ref->DrawScoreBoard();
	ref->DrawGameClock();
	
	if (ref->isGameEnded())
		ref->DrawGameOver();

	xObj->FontPrint(helpFont, 
				HELP_PLACEMENT.x, 
				HELP_PLACEMENT.y, 
				helpStr.str(),
				0xFFFFFFFF);
	
	//paused	=	true;	
}