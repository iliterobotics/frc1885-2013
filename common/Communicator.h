#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H
#include <vector>
#include <thread>

class Communicator {
	// See `Communicator.cpp` for function documentation
	public:
		Communicator();
		~Communicator();

		std::thread* init(int robotPort);

		bool needsData();
		bool setNeedsData(bool val);

		void setData(const double set_angle, const double set_bearing,
			const double set_launcher_speed);

		void send();

	private:
		// File descriptors
		int robotServerFd;
		int robotServerClientFd;

		// Internal data
		double angle;
		double bearing;
		double launcher_speed;

		// Set to 'true' when the Communicator needs internal data to send to the robot
		bool needsInternalData;
};
#endif // header guard
