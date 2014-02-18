#include "../../common/Communicator.h"
#include <unistd.h>
#include <thread>
#include <iostream>

int main(int argc, const char * argv[]) {
	int rPort = 2221;
	Communicator comm = Communicator();
	std::thread* th = comm.init(rPort);
	if (th == NULL) {
		std::cout << "Could not initialize the communicator.";
		return 1;
	}

	for (;;) {
		if (comm.needsData()) {
			comm.setData(3.43, 1.12, 100.1231);
		}
		usleep(200000);
	}

	// We never get here, but oh well.
	th->join();

	return 0;
}
