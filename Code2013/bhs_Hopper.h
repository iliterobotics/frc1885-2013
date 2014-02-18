#ifndef __BHS_HOPPER__H__
#define __BHS_HOPPER__H__
#include "GlobalData.h"

class GlobalData;

class bhs_Hopper
{
public:
	bhs_Hopper();
	~bhs_Hopper();

	bool isDown(bool forceUpdate = true);
	void putDown();	

	bool isUp(bool forceUpdate = true);
	void putUp();
	
private:
	GlobalData* m_globals;
	
	void updateState();
};

#endif // __BHS_HOPPER__H__
