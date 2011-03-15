#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "GameHeaders.h"
#include "DirectX.h"
#include "Team.h"
#include "Ball.h"
#include "Referee.h"
#include "UserControl.h"


class Game
{
	private:
		bool					gameover;					//Used to pass windows quit message
		bool					paused;
		string				appTitle;					//Name for windows class and title bar
		string				bGroundPath;				//Path to background image
		string				spritePath;				//Path to player sprite sheet
		string				ballPath;					//Path to the ball sprite image
		int					screenWidth;				//Initial Screen Width
		int					screenHeight;				//Initial Screen Height
		DirectX				*xObj;					//Direct X object
		Team					*homePlayers;				//Holds all home player sprites
		Team					*awayPlayers;				//Holds all away player sprites
		Field				*field;					//The playfield
		RECT					srcRect,					//Used for DirectX screen transfers
							dstRect;	
		Timer				*frameTimer;				//Timer used to control framerate
		Timer				*playerTimer;				//Timer used to control player calculations
		LPDIRECT3DSURFACE9		backGround;				//Pointer for background
		LPDIRECT3DTEXTURE9		playerTexture;				//Sprite Sheet used for players
		LPDIRECT3DTEXTURE9		ballTexture;				//Holds the image used for the ball
		LPD3DXFONT			helpFont;					//Displays a helpful message about resetting the players to the user
		Referee				*ref;
		std::ostringstream		helpStr;					//String used to display help to user
		UserControl			*uc;
		Timer				*inputTimer;

	public:
						Game()								{
															gameover		=	false;			//Set quit option to false
															appTitle		=	"Robo Soccer";		//Set app title
															bGroundPath	=	"SoccerField.jpg";	//Set path to background
															spritePath	=	"SoccerBots.bmp";	//Set path to players
															ballPath		=	"SoccerBall.bmp";	//Set Path to ball
															screenWidth	=	SCREEN_WIDTH;		//Set screen width 
															screenHeight	=	SCREEN_HEIGHT;		//Set screen Height
															xObj			=	new DirectX;		//Initialize Direct X Obj
															frameTimer	=	new Timer(1000 / 33);//24 FPS
															playerTimer	=	new Timer(5);		//Calc every 100 miliseconds
															field		=	new Field();		//Crate field object
															uc			=	new UserControl();	//Create user control object
															paused		=	true;			//Pause game at start
															
															inputTimer	=	new Timer(1000);	//Create the input timer

															homePlayers	=	NULL;			//Other class pointers set up 
															awayPlayers	=	NULL;			// in game initialization phase
															ref			=	NULL;
															}
						~Game();
		
		//Game window functions
		string			getAppTitle		()					{return		appTitle;}	//Return AppTitle
		int				getScreenW		()					{return		screenWidth;}	//Return Screen Width
		int				getScreenH()							{return		screenHeight;}	//Return Screen Height
		
		//Game Engine Functions
		bool				Game_Init			(HWND		window);							//Game initializer
		void				Game_Run			(HWND		window);							//Main game functon
		bool				isGameOver			()					{return		gameover;}//Return game over object	
		void				setGameOver			(bool		gOver)	{gameover	=	gOver;}	//Set game over variable
								
		void				CreateBackground	();											//Load and render background image
		void				CreateSprites		();											//Initialize all player sprite objects
		void				CreateReferee		();											//Initialize the referee object
		void				CalcSprites		();											//Calculate the sprites at each step
		void				DrawSprites		();											//Draw sprites to the screen
		void				DrawBackground		();											//Draw the field to the screen
				
};	

#endif