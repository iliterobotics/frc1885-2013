#include "TimeUtils.h"

#include <stddef.h>

// Returns the current time
timeval TimeUtils::currTime() {
	timeval currTime;
	gettimeofday(&currTime, NULL);
	return currTime;
}

// Returns the current time
double TimeUtils::currTimeD() {
	timeval curr = currTime();
	return toDouble(curr);
}

// Returns the difference between two times
timeval TimeUtils::timeDifference(timeval &pRecent, timeval &pPast) {
	timeval diff = {pRecent.tv_sec - pPast.tv_sec, pRecent.tv_usec - pPast.tv_usec};
	if (diff.tv_usec > 1000000) {
		diff.tv_sec += diff.tv_usec/1000000;
		diff.tv_usec = diff.tv_usec%1000000;
	}
	return diff;
}

// Returns the time since a previous time
timeval TimeUtils::timeSince(timeval &pPreviousTime) {
	timeval curr = currTime();
	return timeDifference(curr, pPreviousTime);
}

// Returns the time since a previous time
double TimeUtils::timeSince(double &pPreviousTime) {
	return currTimeD() - pPreviousTime;
}

// Returns a double time as a timeval
timeval TimeUtils::toTimeval(double &pTime) {
	timeval result;
	result.tv_sec = (int)pTime;
	result.tv_usec = (pTime-(int)pTime)*1000000;
	return result;
}

// Returns a timeval time as a double
double TimeUtils::toDouble(timeval &pTime) {
	return pTime.tv_sec + pTime.tv_usec/1e6;
}
