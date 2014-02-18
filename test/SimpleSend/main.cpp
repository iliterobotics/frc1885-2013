#include <sys/socket.h>
#include <sys/select.h>
#include <signal.h>
#include <iostream>
#include <cstring>
#include <errno.h>

#include "../../common/SocketUtils.h"

typedef struct Values {
	double a;
	double b;
	double c;
} Values;

int main () {
	int r, maxfd;
	fd_set rd;
	struct timeval tv;

	Values vals;
	vals.a = 1.1;
	vals.b = 2.2;
	vals.c = 3.3;

	int serv = -1;
	if ((serv = SocketUtils::createTcpServer(2221)) < 0) {
		cerr << "Could not create server." << endl;
		return EXIT_FAILURE;
	}

	sockaddr addr;
	memset(&addr, 0, sizeof(sockaddr));
	// only accepts connection once
	cout << "Enter to accept connection." << endl;
	cin.get();

	int client = -1;
	if ((client = SocketUtils::acceptTcpClient(serv, &addr)) < 0) {
		cerr << "Could not accept connection." << endl;
		return EXIT_FAILURE;
	}

	for (;;) {
		FD_ZERO(&rd);
		FD_SET(client, &rd);
		maxfd = client;
		tv.tv_sec = 120;
		tv.tv_usec = 0;

		r = select(maxfd + 1, &rd, 0, 0, &tv);
		if (r < 0) {
			perror("Select error");
			return EXIT_FAILURE;
		} else if (FD_ISSET(client, &rd)) {
			int i;
			if (read(client, &i, sizeof(i)) != sizeof(i)) {
				perror("Read error");
			}
			write(client, &vals, sizeof(vals));
		}
	}
}
