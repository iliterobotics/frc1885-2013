#ifndef __BHS_SPATULA__H__
#define __BHS_SPATULA__H__
#include "GlobalData.h"
#include "TimedBoolean.h"

#include <Math.h>

class bhs_Spatula
{
public:
	bhs_Spatula();
	~bhs_Spatula();

	void moveUp();
	bool isUp() const;

	void moveDown();
	bool isDown() const;

	void setIntakeOn();
//	bool isIntakeOn() const;

	void setIntakeOff();
//	bool isIntakeOff() const;
	
private:
	GlobalData* m_globals;
	TimedBoolean timedUp;
	bool _isUp;
};

#endif // __BHS_SPATULA__H__
