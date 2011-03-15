#ifndef SUPPORTING_POS_CALCULATOR_H
#define SUPPORTING_POS_CALCULATOR_H

#include "GameHeaders.h"
#include "Player.h"

class SupportPosCalculator
{
	private:
		D3DXVECTOR2	**regions,
					goal,
					RegSize;
		Player		*teammate,
					*oppTeam,
					*ballCarrier;
		int			rows, 
					cols,
					numPlayers,
					homeTeamIdxOffset;
		double		**passPotential,
					**goalPotential,
					**supportRegions,
					**distValue;
		bool			regionsAvail;

		void			passPotentialCalc	();
		void			goalPotentialCalc	();
		void			distFromAttackerCalc();
		void			setBallCarrier		();
		

	public:
		SupportPosCalculator	(D3DXVECTOR2	**Regs,		D3DXVECTOR2	Goal,	Player	*sameTeam,
							 Player		*opposing,	int			RegRows,	int		RegCols,	
							 int			players,		bool			isHomeTeam, int	regOffset){
																regions		= Regs;
																goal			= Goal;
																teammate		= sameTeam;
																oppTeam		= opposing;
																passPotential	= new double *[RegRows];
																goalPotential	= new double *[RegRows];
																supportRegions = new double *[RegRows];
																distValue		= new double *[RegRows];

																for (int x = 0; x < RegRows; x++)
																{
																	passPotential[x] = new double [RegCols];
																	goalPotential[x] = new double [RegCols];
																	supportRegions[x]= new double [RegCols];
																	distValue[x]	  = new double [RegCols];
																}

																rows			= RegRows;
																cols			= RegCols;
																numPlayers	= players;

																homeTeamIdxOffset = regOffset;
																regionsAvail	=	false;

													}
		D3DXVECTOR2	getBestSupportPos		();
		double		getPassPotential		(	int			regX, 
											int			regY);
		double		getGoalPotential		(	int			regX, 
											int			regY);
		Player		*getClosestTeammate		(	Player		p, 
											Player		*team);
		
		Player		*getClosestToTgt		(	Player		*team,
											D3DXVECTOR2	tgt);
		D3DXVECTOR2	getGoalShot			(	Player		*p);
		D3DXVECTOR2	getPassShot			(	Player		*p,
											Player		*team);
		void			calcSupportingRegions	(	Player		*ballCarrier);
		D3DXVECTOR2	getSupportPos			(	Player		*p, 
											Player		*ball);
		bool			isShotSafe		(	D3DXVECTOR2 startLoc, 
										D3DXVECTOR2 endLoc);


};


#endif
