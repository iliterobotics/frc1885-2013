#ifndef __TIMED_BOOLEAN__H__
#define __TIMED_BOOLEAN__H__

#include "WPILib.h"

class TimedBoolean
{
public:
	TimedBoolean(bool initial = false);
	
	void delaySet(bool setTo, double inSec);
	void immediateSet(bool setTo);
	
	bool get();
	
private:
	double m_secMark;
	bool m_setTo;
};

#endif // defined(__TIMED_BOOLEAN__H__)
