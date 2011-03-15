#ifndef VECTOR_H
#define VECTOR_H

class vector
{
	public:
		double				x,												//actual x val
							y,												//actual y val
							normX,											//normalized x
							normY,											//normalized y
							mag;												//magnitude
	
		vector				()						{	x		= 0;	
														y		= 0;}
		vector				(double xVal,				
							 double yVal)				{	x		= xVal;		
														y		= yVal;}
		void			normalize ()						{
														mag		= sqrt(pow(x,2) + pow(y,2));
														normX	= x / mag;
														normY	= y / mag;
													}
		double		dot		(vector v2)				{	return	((v2.normX * normX) + (v2.normY * normY));}
};
		
#endif