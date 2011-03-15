#ifndef PLAY_FIELD_H
#define PLAY_FIELD_H

#include "GameHeaders.h"
#include "Ball.h"
#include "Timer.h"


class Field
{
	public:
		D3DXVECTOR2			**regions;				//All regions on the field
		D3DXVECTOR2			*Goals;					//The goal regions
		Ball					*ball;					//Class containing the ball data
		bool					 gameOn;					//Game has started
		Timer				*inPlayTimer;

		Field				();						
		~Field				();
		void InitRegions		();
		void	updateBall		();
		bool IsBallInField		();						//Moves ball back into play
		void	resetBall			();

};

#endif