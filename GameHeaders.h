#ifndef GAME_HEADERS_H
#define GAME_HEADERS_H

#pragma warning(disable : 4244)
#pragma warning(disable : 4800)

//header files
#define WIN32_EXTRA_LEAN					//Remove unnessesary windows stuff
#define DIRECTINPUT_VERSION 0x0800			//Declare DX version
#include <windows.h>					
#include <d3d9.h>						
#include <d3dx9.h>
#include <dinput.h>
#include <xinput.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
using namespace std;

//libraries
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib") 

const int			SCREEN_WIDTH		=	1230;					//Width in Pixels
const int			SCREEN_HEIGHT		=	790;						//Height in Pixels
const double		PI				=	3.14159265;				//Rounded PI Calculation
const double		PI_OVER_2			=	PI / 2;					//90 degrees
const double		PI_OVER_4			=	PI / 4;					//45 degrees
const double		PI_OVER_180		=	PI / 180;					//Used for degree to radian calc
const double		COLLISION_DIST		=	0.0000001;				//Distance between two circles that make them touch.
const double		MAX_KICK_VEL		=	10;
const double		MIN_KICK_VEL		=	8;
const double		MAX_DRIB_VEL		=	10;
const double		MIN_DRIB_VEL		=	8;	
const int			MIN_THREAT_DIST	=	200 * 200;				//If opponent is within x pixels
const int			FIELD_WIDTH		=	1190;					//Width in Pixels
const int			FIELD_HEIGHT		=	690;						//Height in Pixels
const int			REG_COLS			=	45;						//Number of regions horizontally
const int			REG_ROWS			=	30;						//Number of regions vertically
const int			REG_WIDTH			=	FIELD_WIDTH	/ REG_COLS;	//Region width in pixels
const int			REG_HEIGHT		=	FIELD_HEIGHT	/ REG_ROWS;	//Region height in pixels
const int			NUM_REGS			=	REG_COLS		* REG_ROWS;	//Number of regions total
const int			GOAL_HEIGHT		=	198;						//Height in Pixels
const int			GOAL_SHOT_DIST		=	300 * 300;				//Squared dist needed to make goal shot
const int			MIN_PASS_DIST		=	300 * 300;				//Minimum dist squared required to pass to teammate
const int			MAX_PASS_DIST		=	500 * 500;				//Maximum dist squared to pass to teammate
const int			MIN_MOVEMENT		=	50;						//Player must move 50 points during test
const D3DXVECTOR2	SCORE_BOARD_HEADER		((float)SCREEN_WIDTH / 2 - 50, 10.0f);//Location where the scoreboard is placed
const D3DXVECTOR2	SCORE_BOARD_HOMETEAM	(100.0f, 10.0f);
const D3DXVECTOR2	SCORE_BOARD_AWAYTEAM	((float)SCREEN_WIDTH - 200, 10.0f);
const D3DXVECTOR2	GAME_TIMER_PLACEMENT	((float)SCREEN_WIDTH - 200, (float)SCREEN_HEIGHT - 100);
const D3DXVECTOR2	HELP_PLACEMENT			(75.0f, (float)(SCREEN_HEIGHT - 200.0f));
const D3DXVECTOR2	NO_SHOT				(0.0f, 0.0f);
const D3DXVECTOR2	GAME_OVER_PLACEMENT		((float)SCREEN_WIDTH / 2 - 275, 0);
const int			NUM_PLAYERS		=	3;						//Number of players on a team
const int			NUM_REGIONS		=	5;						//Defined a defensive, offensive, neutral and kick off 
const int			MAX_DIST_FOR_GOAL	=	200;
const int			MAX_DIST_FOR_GOAL_SQ =	MAX_DIST_FOR_GOAL * MAX_DIST_FOR_GOAL;
const int			GAME_TIME			=	1000	* 60 * 3;				// Mili Seconds * Seconds * Minutes 
#endif