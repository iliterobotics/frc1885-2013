#include "TimeUtils.h"

#include <iostream>

using namespace std;

int main() {
	timeval time1 = TimeUtils::currTime();
	struct tm* timeinfo;
	timeinfo = localtime(&time1.tv_sec);
	cout << asctime(timeinfo) << "Epoch time: " << TimeUtils::toDouble(time1) << "\n";

	timeval time2 = TimeUtils::timeSince(time1);
	cout << "Elapsed time: " << TimeUtils::toDouble(time2) << "\n";

	timeval time3 = time1;
	time3.tv_sec += 1;
	time3.tv_usec += 500000;
	timeval diff = TimeUtils::timeDifference(time3, time1);
	cout << "Check time difference (1.5): " << TimeUtils::toDouble(diff) << "\n";

	time3.tv_usec -= 1000000;
	diff = TimeUtils::timeDifference(time3, time1);
	cout << "Check time difference (0.5): " << TimeUtils::toDouble(diff) << "\n";

	diff = TimeUtils::timeDifference(time1, time3);
	cout << "Check time difference (-0.5): " << TimeUtils::toDouble(diff) << "\n";
}

