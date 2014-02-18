/*
 * bhs_EdgeDetector.h
 *
 *  Created on: Feb 5, 2009
 *      Author: ITRobo
 */

/*
 * Description:
 */
#ifndef _BHS_EDGEDETECTOR_H_
#define _BHS_EDGEDETECTOR_H_

class bhs_EdgeDetector
{
	
public:
	
	bhs_EdgeDetector();
	~bhs_EdgeDetector();
	
	int risingEdge(int anButtonValue);
	int fallingEdge(int anButtonValue);
	int edgeChange(int anButtonValue);

private:

	int anPrevious;
	
};
#endif
