#ifndef _TIME_UTILS_H_
#define _TIME_UTILS_H_

#include <sys/time.h>

class TimeUtils {
public:
	static timeval currTime();
	static double currTimeD();
	static timeval timeDifference(timeval &pRecent, timeval &pPast);
	static timeval timeSince(timeval &pPreviousTime);
	static double timeSince(double &pPreviousTime);
	static timeval toTimeval(double &pTime);
	static double toDouble(timeval &pTime);
};

#endif
