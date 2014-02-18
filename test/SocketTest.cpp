#include "SocketUtils.h"

#include <iostream>
using namespace std;

int main() {
	const char* msg = "Hello World!";
	char* buf[strlen(msg)];
	int serverSocketFd = SocketUtils::createTcpServer(1234);
	int clientFd = SocketUtils::createTcpClient("localhost", 1234);

	//Have the server accept the client connection, or return with -1 if no client
	sockaddr clientAddr;
	bzero(&clientAddr, sizeof(sockaddr));
	int serverClientFd = SocketUtils::acceptTcpClient(serverSocketFd, &clientAddr);
	if (clientFd < 0) {
		cerr << "Server-Client failed to connect\n";
	} else {
		cout << "Server-Client connected!\n";
	}

	//Attempt to send a message from the server to client, if the client never connected, this will fail, obviously...
	if (send(serverClientFd, msg, strlen(msg), 0) < 0) {
		cerr << "Failed to send " << msg << " to client...\n";
	} else if (recv(clientFd, buf, strlen(msg), 0) < 0) {
		cerr << "Failed to receive message from server\n";
	} else {
		cout << "Received " << msg << " from server\n";
	}

	//Attempt to send a message from the client to the server, if the client never connected, this will fail, obviously...
	bzero(buf, strlen(msg));
	if (send(clientFd, msg, strlen(msg), 0) < 0) {
		cerr << "Failed to send " << msg << " to server...\n";
	} else if (recv(serverClientFd, buf, strlen(msg), 0) < 0) {
		cerr << "Failed to receive message from client\n";
	} else {
		cout << "Received " << msg << " from client\n";
	}
}
