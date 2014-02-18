#include "SocketUtils.h"

#include <iostream>
using namespace std;

const char* msg = "Hello World!";
const int msgLen = strlen(msg);

int main() {
	bool error = false;
	char buf[msgLen];

	int serverFd = SocketUtils::createTcpClient("localhost", 1234);
   if (serverFd < 0) {
      cerr << "Failed to connect to server\n";
   } else {
      while (!error) {
         if (SocketUtils::tcpRead(serverFd, buf, strlen(msg), 0, -1) < strlen(msg)) {
            cerr << "Failed to receive message from server\n";
            error = true;
         } else {
            cout << "Received \"" << msg << "\" from server\n";
         }
      }
   }

	close(serverFd);

	return 0;
}
