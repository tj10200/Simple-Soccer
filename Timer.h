#ifndef TIMER_H
#define TIMER_H

#include <iostream>

class Timer
{
	private:
		INT64		frequency,
					startTime,
					endTime,
					interval,
					curTime,
					pauseTime;
		double		resolution,
					timeDiff;
		
	public:
		Timer		(INT64 timerVal)		{	setFreq					();					//Doesn't change no need to repul at init
											setInterval				(timerVal);
											initClock					();}
		INT64		getFreq		()		{	return					frequency;}
		INT64		getStartTime	()		{	return					startTime;}
		INT64		getEndTime	()		{	return					endTime;}
		INT64		getCurTime	();
		INT64		getInterval	()		{	return					interval;}

		void			setFreq		()		{	QueryPerformanceFrequency	((LARGE_INTEGER *)&frequency);
											resolution			=	(double)1 / (double)frequency;
										}
		void			setStartTime	()		{	QueryPerformanceCounter		((LARGE_INTEGER *)&startTime);}
		void			setEndTime	()		{	QueryPerformanceCounter		((LARGE_INTEGER *)&endTime);}		//Convert interval to microseconds
		void			setCurTime	()		{	QueryPerformanceCounter		((LARGE_INTEGER *)&curTime);}
		void			setInterval	(INT64 mili){	interval				=	mili;}
		void			initClock		()		{	
											setStartTime				();
											setEndTime				();
											pauseTime					= 0;
										}			
		bool			isTime		()		{	
											timeDiff				= ((endTime - startTime) *	//Get seconds of elapsed time 
																	resolution);	
											timeDiff				*= 1000;					//Convert timeDiff to miliseconds
											
											if	(timeDiff			>  interval)
											{
												initClock();
												return true;
											}
											else
												setEndTime();
											
											return false;
										}
		void		pauseTimer			()	{	startTime				++;
											endTime				++;}
		
		int		totMilisRemaining		() {
											timeDiff				= ((endTime - startTime) *	//Get seconds of elapsed time 
																	resolution);
											
											timeDiff				*= 1000;					//Convert to miliseconds
											
											timeDiff				= interval - timeDiff;		//Subtract timeDiff from interval

											if (timeDiff			<	0)
												return				0;
											else
												return				timeDiff;
										}
		
		int		getMilisRemaining		()	{	return				totMilisRemaining()	% 1000;}
		int		getSecondsRemaining		()	{	return				(totMilisRemaining()	% 
																(60 * 1000)) /
																1000;
										}
		int		getMinutesRemaining		()	{	return				(totMilisRemaining()	% 
																(60 * 60 * 1000)) /
																(60 * 1000);
										}
		double	getFramesPerSec		()		
										{
											setCurTime				();												//Update current time
											timeDiff				=	(double)((curTime - startTime) *						//Return time converted to miliseconds
																			resolution);		
											initClock					();					//Reset Clock
											return timeDiff;
										}	

		

};

#endif