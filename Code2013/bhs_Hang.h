#ifndef __BHS_HANG__H__
#define __BHS_HANG__H__
#include "GlobalData.h"

// I am so sorry
class GlobalData;

class bhs_Hang
{
public:
	bhs_Hang();
	~bhs_Hang();

	void extend();
	bool isExtended() ;

	void retract();
	bool isRetracted();

private:
	static GlobalData* m_globals;
};

#endif //__BHS_HANG__H__
