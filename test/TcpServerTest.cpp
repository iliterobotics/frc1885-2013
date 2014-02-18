#include "SocketUtils.h"
#include "TimeUtils.h"

#include <cstdlib>
#include <iostream>
#include <vector>
#include <errno.h>

using namespace std;

const char* msg = "Hello World!";
const int msgLen = strlen(msg);

struct ClientThread {
	pthread_t threadId;
	int clientFd;
};

void clientThreadHandler(void *clientFdPtr) {
	int clientFd = *((int*)clientFdPtr);
	bool error = false;
	while(!error) {
		if(SocketUtils::tcpWrite(clientFd, msg, msgLen) < 0) {
			cerr << "Failed to send to client: " << strerror(errno) << "\n";
			error = true;
		} else {
         cout << "Sent data to client\n";
			sleep(1);
		}
	}
	close(clientFd);
}

int main() {
	int serverSocketFd = SocketUtils::createTcpServer(1234);

	vector<ClientThread> clientThreads(0);

	while(true) {
		//Have the server accept the client connection, or return with -1 if no client
		sockaddr_in clientAddr;
		bzero(&clientAddr, sizeof(sockaddr));
		int clientFd = SocketUtils::acceptTcpClient(serverSocketFd, (sockaddr*)&clientAddr);
		if (clientFd < 0) {
			//cerr << "Server-Client failed to connect\n";
		} else {
			ClientThread newClientThread;
			newClientThread.clientFd = clientFd;
			if(pthread_create(&newClientThread.threadId, NULL, (void*(*)(void*))&clientThreadHandler, (void*)&newClientThread.clientFd) == 0) {
				clientThreads.push_back(newClientThread);
				cout << "Client connected!\n";
			} else {
				close(newClientThread.clientFd);
				cout << "Failed to spawn thread for client, closing...\n";
			}
		}
		sleep(1);
	}

	return 0;
}
