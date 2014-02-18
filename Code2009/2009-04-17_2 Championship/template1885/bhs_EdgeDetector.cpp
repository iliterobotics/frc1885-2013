/*
 * bhs_EdgeDetector.cpp
 *
 *  Created on: Feb 5, 2009
 *      Author: ITRobo
 */

#include "bhs_EdgeDetector.h"

/*
 * Default constructor
 */
bhs_EdgeDetector::bhs_EdgeDetector()
{
	anPrevious=0;
}

/*
 * Default destructor
 */
bhs_EdgeDetector::~bhs_EdgeDetector()
{
	
}
int bhs_EdgeDetector::risingEdge(int anButtonValue)
{
	if(anButtonValue!=0 && anPrevious==0)
	{
		anPrevious=anButtonValue;
		return 1;
	}
	else
	{
		anPrevious=anButtonValue;
		return 0;
	}
}
int bhs_EdgeDetector::fallingEdge(int anButtonValue)
{
	if(anButtonValue==0&&anPrevious!=0)
	{
		anPrevious=anButtonValue;
		return 1;
	}
	else
	{
		anPrevious=anButtonValue;
		return 0;
	}
}
int bhs_EdgeDetector::edgeChange(int anButtonValue)
{
	if(anButtonValue!=anPrevious)
	{
		anPrevious=anButtonValue;
		return 1;
	}
	else
	{
		return 0;
	}
}
