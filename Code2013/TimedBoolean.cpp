#include "TimedBoolean.h"

TimedBoolean::TimedBoolean(bool initial)
{
	immediateSet(initial);
}
	
void TimedBoolean::delaySet(bool setTo, double inSec)
{
    double newSecMark = inSec + Timer::GetFPGATimestamp();
	// If setTo is already our value, then skip the whole timing part.
	if(setTo == m_setTo)
	{
		if(m_secMark > newSecMark)
		{
			m_secMark = newSecMark;
		}
	}
	else
	{
		// we're setting new setTo. Yayz.
		m_setTo = setTo;
		m_secMark = newSecMark;
	}
}

void TimedBoolean::immediateSet(bool setTo)
{
	delaySet(setTo, -1);
}
	
bool TimedBoolean::get()
{
	if(Timer::GetFPGATimestamp() >= m_secMark)
	{
		return m_setTo;
	}
	
	return !m_setTo;
}
