#include "Communicator.h"

// TODO: stop using these socket utils.
#include "../common/SocketUtils.h"
#include <memory>
#include <signal.h>
#include <iostream>

#include "../comm/cpp/chunk.h"
#include "../comm/cpp/chunkrwfactory.h"
#include "../comm/cpp/chunktransmission.h"

// should be 0 to 3
//  0  no debugging
//  1  important messages
//  2  important and non-important status messages
//  3  important, non-important status, and almost entirely useless messages
// TODO: create a decent logging system.
#ifndef DEBUG
# define DEBUG 3
#endif

Communicator::Communicator() {
	angle = 0.0;
	bearing = 0.0;
	launcher_speed = 0.0;

	needsInternalData = false;
}

Communicator::~Communicator() {
	close(robotServerClientFd);
	close(robotServerFd);
}

// Initializes the tcp server and starts the send thread.
// A pointer to the send thread is returned by the function.
// NULL is returned if initialization fails.
std::thread* Communicator::init(int robotPort) {
	static bool didInit = false;

	// The initialization sequence may only be performed once
	if (didInit) {
		return NULL;
	} else {
		didInit = true;

		// `SIGPIPE` is ignored
		if ((robotServerFd = SocketUtils::createTcpServer(robotPort)) < 0) {
			return NULL;
		}
		signal(SIGPIPE, SIG_IGN);

		// The thread is passed back to the caller of the function,
		//  which should handle it appropriately.
		std::thread* th = new std::thread([=] () { this->send(); });

#if DEBUG >= 1
		std::cout << "Server up on port " << robotPort << "." << std::endl;
#endif

		return th;
	}
}

// Returns whether or not the Communicator needs data to send to the robot
bool Communicator::needsData() {
	return needsInternalData;
}

// Sets the `needsInternalData` flag explicitly.
bool Communicator::setNeedsData(bool val) {
	return needsInternalData = val;
}

// Sets the internal data and sets the `needsInternalData` flag to false.
void Communicator::setData(double set_angle, double set_bearing,
		double set_launcher_speed) {
	angle = set_angle;
	bearing = set_bearing;
	launcher_speed = set_launcher_speed;

	needsInternalData = false;
}

// Connects and sends data to the robot continuously.
// This function is blocking.
//
// 1. Wait for a connection from the robot
// 2. Wait for a data request (go signal)
// 3. Set the internal data request flag (`needsInternalData`)
// 4. Wait until the data request flag is toggled (usually via `setData()`)
// 5. Send the internal data to the robot
// 6. Loop back to step 2.
//
// A closed socket is handled by looping back to step 1.
// TODO: we should probably use select.
void Communicator::send() {
	for (;;) {
		// Block until we're connected
		for (;;) {
			sockaddr addr;
			bzero(&addr, sizeof(sockaddr));
			robotServerClientFd = SocketUtils::acceptTcpClient(robotServerFd, &addr);

			if (robotServerClientFd >= 0) {
#if DEBUG >= 1
				std::cout << "Robot connection successful." << std::endl;
#endif
				break;
			} else {
#if DEBUG >= 1
				std::cerr << "Robot connection failed." << std::endl;
#endif
				usleep(2000000);
			}
		}

		// Wait for go signal, wait for internal data, send internal data, repeat
		int netError = 0;
		while (netError != net::error::SOCKETCLOSED) {
			netError = 0;

#if DEBUG >= 3
			std::cout << "Blocking until go signal is received..." << std::endl;
#endif
			// Block until we get the go signal
			while ((!netError || netError == net::error::NODATA) && !needsInternalData) {
				net::Chunk* chunk;

				netError = net::chunktransmission::readChunk(robotServerClientFd, &chunk, true);
				if (!netError) {
#if DEBUG >= 3
					std::cout << "Read a chunk." << std::endl;
#endif
					std::unique_ptr<net::ChunkReader> reader(net::chunkrw::readerFor(chunk));
					net::types::Type t;

					// Get the type
					if (reader->nextType(&t)) {
#if DEBUG >= 1
						std::cerr << "Could not determine received type." << std::endl;
#endif
					} else {
#if DEBUG >= 3
						std::cout << "Got type." << std::endl;
#endif
					}

					// The data request flag must be a boolean.
					//  'true' sets the internal data request flag
					//  'false' is ignored
					// TODO: Allow this to accept any data type.
					//       There's no reason to force data requests to be of boolean types.
					if (t == net::types::Boolean) {
						bool b;
						reader->nextBoolean(&b);
						needsInternalData = b;
#if DEBUG >= 2
						std::cout << "Received flag: " << b << std::endl;
#endif
					} else {
#if DEBUG >= 1
						std::cerr << "Invalid type received." << std::endl;
#endif
						// Just in case the robot is sending a bunch of bad data, sleep a bit
						usleep(2000);
					}
				} else {
#if DEBUG >= 1
               if (netError != net::error::NODATA) {
                  std::cerr << "Network error: " << net::error::stringFrom(netError) << std::endl;
               }
#endif
				}
			}

#if DEBUG >= 3
			std::cout << "Blocking until internal data set..." << std::endl;
#endif
			// Block until the data is updated (I know, it's sketchy)
			while (needsInternalData && !netError);

#if DEBUG >= 2
			std::cout << "Sending data over network..." << std::endl;
#endif

			// Chunk contains [ launcher angle, robot bearing, launcher speed ]
			std::unique_ptr<net::ChunkWriter> writer(net::chunkrw::preferredWriter());
			writer->writeDouble(angle);
			writer->writeDouble(bearing);
			writer->writeDouble(launcher_speed);
#if DEBUG >= 3
         std::cout << "Angle: " << angle << std::endl
         << "Bearing: " << bearing << std::endl
         << "Speed: " << launcher_speed << std::endl;
#endif
			netError = net::chunktransmission::writeChunk(robotServerClientFd, writer->chunk(), true);

#if DEBUG >= 1
			if (netError) {
				std::cerr << "Network error: " << net::error::stringFrom(netError) << std::endl;
			}
#endif
		}
	}
}
