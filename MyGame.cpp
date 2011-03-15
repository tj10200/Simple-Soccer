// Beginning Game Programming, Third Edition
// MyGame.cpp
/*

#include "GameHeaders.h"



//Sprite linkwalk (200.0f, 300.0f, 0, 13, 420/13, 125/3, 3.0f, 0.0f, 16, 25, 100, 1, .75f, .75f, asteroidColor);
//Link linkwalk (200.0f, 300.0f, 0, 13, 420/13, 125/3, 3.0f, 0.0f, 16, 25, 500, 1, .75f, .75f, asteroidColor);
LPDIRECT3DTEXTURE9 imgLinkWalk	= NULL;

//Direct X Object 



bool Game_Init(HWND window)
{
	//Initialize Direct3D Objects
	xObj.Direct3D_Init		(window, 
							 SCREENW, 
							 SCREENH, 
							 false);
	//Initialize Controllers
	xObj.DirectInput_Init	(window);

	//Initialize image
	imgLinkWalk = xObj.LoadTexture ("linkWalk.png");
	if (!imgLinkWalk) 
		return false;

	//Initialize sprite object with sprite device pointer
	//linkwalk.Sprite_Init	(xObj.getSprite());
	
	return true;
}

void Game_Run(HWND window)
{
    //make sure the Direct3D device is valid
    if (!xObj.getDevice()) return;

    //update input devices
    xObj.DirectInput_Update();

	xObj.ClearScene();

	//Move the ship up/down with arrow keys
	/*if (xObj.Key_Down(DIK_UP))
	{	
		ship.setY (ship.getY() - 1.0f);
		if (ship.getY() < 0) 
			ship.setY(0);
	}
	*/
	//Move and animate the asteroid
	/*
	linkwalk.setX (linkwalk.getX() + linkwalk.getVelX());
	if(linkwalk.getX() < 0 || (linkwalk.getX() > SCREENW - linkwalk.getWidth()))
	{
		linkwalk.setDirect	(linkwalk.getDirection() * -1);
		linkwalk.setVelX	(linkwalk.getVelX()	  * -1);
		linkwalk.setScale   (linkwalk.getScale() * -1);
	}
	*/
/*
	if (xObj.Key_Down(DIK_UP))
	{
//		linkwalk.MoveSprite(UP);
//		linkwalk.setY (linkwalk.getY() - linkwalk.getVelY());
	}
	else if (xObj.Key_Down(DIK_DOWN))
	{
//		linkwalk.MoveSprite(DOWN);
//		linkwalk.setY (linkwalk.getY() - linkwalk.getVelY());
	}
	else if (xObj.Key_Down(DIK_LEFT))
	{
//		linkwalk.MoveSprite(LEFT);
//		linkwalk.setX (linkwalk.getX() + linkwalk.getVelX());
	}
	else if (xObj.Key_Down(DIK_RIGHT))
	{
//		linkwalk.MoveSprite(RIGHT);
//		linkwalk.setX (linkwalk.getX() + linkwalk.getVelX());
	}
	else
//		linkwalk.MoveSprite(STOP);

//	linkwalk.Sprite_Animate();

	
	//start rendering
    if (xObj.beginDev())
    {
		xObj.BeginSpriteDraw();
		
//	    linkwalk.Sprite_Transform_Draw (imgLinkWalk);

		xObj.EndSpriteDraw();
	
		//stop rendering
        xObj.endDev();
	}

    //exit when escape key is pressed
    if (xObj.Key_Down(DIK_ESCAPE)) gameover = true;

    //controller Back button also ends
    if (xObj.controllers[0].wButtons & XINPUT_GAMEPAD_BACK)
        gameover = true;

}

void Game_End()
{
    //free memory and shut down
	if (imgLinkWalk) 
		imgLinkWalk->Release();
    xObj.DirectInput_Shutdown();
    xObj.Direct3D_Shutdown();
}


*/