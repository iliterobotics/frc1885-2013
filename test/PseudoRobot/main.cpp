#include "../../common/SocketUtils.h"
#include <memory>

#include "../../comm/cpp/textchunkwriter.h"
#include "../../comm/cpp/textchunkreader.h"
#include "../../comm/cpp/binarychunkreader.h"
#include "../../comm/cpp/binarychunkwriter.h"
#include "../../comm/cpp/binaryencoding.h"
#include "../../comm/cpp/chunk.h"
#include "../../comm/cpp/chunkrwfactory.h"
#include "../../comm/cpp/chunktransmission.h"

int main(int argc, char** argv) {
	int rPort;
	const char* ip;

	// Parse arguments
	// host, port
	if (argc == 2) {
		ip = argv[1];
	} else if (argc == 3) {
		ip = argv[1];
		rPort = atoi(argv[2]);
	} else {
		ip = "localhost";
		rPort = 2221;
	}

	// Connect to server
	std::cout << "Connecting to " << ip << " on port " << rPort;
	int clientFd = SocketUtils::createTcpClient(ip, rPort);

	for (;;) {
		std::cout << "Press Enter to send a data request..." << std::endl;
		cin.get();

		std::cout << "Sending data..." << std::endl;
		std::unique_ptr<net::ChunkWriter> writer(net::chunkrw::preferredWriter());
		writer->writeBoolean(true);
		net::chunktransmission::writeChunk(clientFd, writer->chunk());

		net::Chunk* chunk;
		std::cout << "Receiving..." << std::endl;
		if (!(net::chunktransmission::readChunk(clientFd, &chunk))) {
			std::unique_ptr<net::ChunkReader> reader(net::chunkrw::readerFor(chunk));
			net::types::Type t;

			double val;
			if (reader->nextDouble(&val)) {
				std::cerr << "Invalid value." << std::endl;
			} else {
				cout << "Angle: " << val << std::endl;
			}

			if (reader->nextDouble(&val)) {
				std::cerr << "Invalid value." << std::endl;
			} else {
				cout << "Bearing: " << val << std::endl;
			}

			if (reader->nextDouble(&val)) {
				std::cerr << "Invalid value." << std::endl;
			} else {
				cout << "Speed: " << val << std::endl;
			}
		}
	}

	return 0;
}
