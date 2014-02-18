#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <algorithm>

#include <arpa/inet.h>

#include "../../comm/cpp/textchunkwriter.h"
#include "../../comm/cpp/textchunkreader.h"
#include "../../comm/cpp/binarychunkreader.h"
#include "../../comm/cpp/binarychunkwriter.h"
#include "../../comm/cpp/binaryencoding.h"
#include "../../comm/cpp/chunk.h"
#include "../../comm/cpp/chunkrwfactory.h"
#include "../../comm/cpp/tests/testdriver.h"

#include "../../common/SocketUtils.h"

using namespace std;

template <typename T>
void tellDeserialized(net::types::Type ty, T value) {
	cout << "Deserialized a(n) " << net::types::stringFor(ty) << "! "
		<< "Value: (" << value << ")" << endl;
}

template <typename T>
string toHexString(const T* ch) {
	ostringstream oss;
	oss << ch->size() << ":";
	oss << hex << uppercase << setfill('0');
	for_each(ch->begin(), ch->end(), [&] (int c) { oss << setw(2) << c; });
	return oss.str();
}

template <typename T>
string toHexString(const T* ch, int size) {
	ostringstream oss;
	oss << size << ":";
	oss << hex << uppercase << setfill('0');
	for_each(ch, ch + size, [&] (int c) { oss << setw(2) << c; });
	return oss.str();
}

string toString(const net::Chunk* ch) {
	return string(ch->begin(), ch->end());
}

int main(int argc, const char * argv[]) {
	int serverSocketFd = SocketUtils::createTcpServer(1234);
	int clientFd = SocketUtils::createTcpClient("localhost", 1234);

	sockaddr clientAddr;
	bzero(&clientAddr, sizeof(sockaddr));
	int serverClientFd = SocketUtils::acceptTcpClient(serverSocketFd, &clientAddr);

	while(1) {
		net::ChunkWriter* tcw = net::chunkrw::preferredWriter();
		string label;

		label = "skfi";
		tcw->writeString(&label);
		tcw->writeInt(127);

		label = "skfd";
		tcw->writeString(&label);
		tcw->writeInt(111);

		net::Chunk* chnk = tcw->takeChunk();
		const net::ubyte* msg = &chnk->front();

		//cout << "Serialized: " << toHexString(chnk) << endl;

		char* buf[chnk->size()];

		// Have the server accept the client connection, or return with 1 if no
		//  client
		if (clientFd < 0) {
			cerr << "Server did not accept client connection." << endl;
			return 1;
		} else {
			cout << "Server accepted client connection." << endl;
		}

		// Attempt to send a message from the server to client, if the client never
		//  connected, this will fail, obviously...
		if (send(serverClientFd, msg, chnk->size(), 0) < 0) {
			cerr << "Failed to send " << msg << " to client...\n";
		} else if (recv(clientFd, buf, chnk->size(), 0) < 0) {
			cerr << "Failed to receive message from server\n";
		} else {
			cout << "Client received '" << string((const char*)msg, chnk->size())
				<< "' from server." << endl;
		}

		// Attempt to send a message from the client to the server, if the client
		//  never connected, this will fail, obviously...
		bzero(buf, chnk->size());
		if (send(clientFd, msg, chnk->size(), 0) < 0) {
			cerr << "Failed to send " << msg << " to server...\n";
		} else if (recv(serverClientFd, buf, chnk->size(), 0) < 0) {
			cerr << "Failed to receive message from client\n";
		} else {
			cout << "Server received '" << string((const char*)msg, chnk->size())
				<< "' from client." << endl;
		}

		net::Chunk r(msg, msg + chnk->size());
		net::ChunkReader* tcr = net::chunkrw::readerFor(&r);

		try {
			cout << endl;
			while(!tcr->atEnd()) {
				net::types::Type t;
				int e = 0;

				// Get label type
				if (tcr->nextType(&t)) {
					throw runtime_error("Invalid label type found.");
				}

				// Verify that the incoming label is a string.
				if (t != net::types::String) {
					throw runtime_error("Potential label mismatch.");
				} else {
					string label;
					if((e = tcr->nextString(&label)))
						throw runtime_error(net::error::stringFrom(e));
					cout << "Label: '" << label << "'" << endl;

					// Get value type
					if (tcr->nextType(&t)) {
						throw runtime_error("Unable to determine a value type.");
					}

					// Take actions and set values according to the values being received
					if (t == net::types::Integer) {
						int i;
						if((e = tcr->nextInt(&i)))
							throw runtime_error(net::error::stringFrom(e));

						if (label == "skpx") {
							cout << "Received kinect x-position: " << i << endl;
						} else if (label == "skpy") {
							cout << "Received kinect y-position: " << i << endl;
						} else if (label == "skpz") {
							cout << "Received kinect z-position: " << i << endl;
						} else if (label == "skpa") {
							cout << "Received kinect yaw: " << i << endl;
						} else if (label == "skfd") {
							cout << "Received kinect depth feed frame rate: " << i << endl;
						} else if (label == "skfi") {
							cout << "Received kinect image feed frame rate: " << i << endl;
						} else {
							cerr << "Received an unknown label-type combination for '" << label
								<< "'." << endl;
						}
					} else {
						cerr << "Received an unused value type.";
					}
				}
			}
		} catch(runtime_error& e) {
			cerr << e.what() << endl;
		}

		tcr->takeChunk();
		delete tcr;

		sleep(1);
		cout << endl;
	}
	return 0;
}
