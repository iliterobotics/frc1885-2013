#include "SocketUtils.h"

#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int SocketUtils::createTcpClient(const char* pHost, int pPort) {
	int sockFd = -1;
	struct hostent *server;
	struct sockaddr_in servaddr;
	bzero((char*)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(pPort);

	if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		//Failed to create socket
      cerr << __FUNCTION__ << " failed to create socket: " << strerror(errno) << "\n";
	} else if ((server = gethostbyname(pHost)) == NULL) {
		//Failed to find server name
      cerr << __FUNCTION__ << " failed to resolve hostname: " << strerror(errno) << "\n";
		close(sockFd);
      sockFd = -1;
	} else {
		memcpy((char*)&(servaddr.sin_addr.s_addr), (char*)(server->h_addr), server->h_length);
		if (connect(sockFd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
			//Failed to connect to server
         cerr << __FUNCTION__ << " failed to connect to server: " << strerror(errno) << "\n";
			close(sockFd);
         sockFd = -1;
		} else {
			//Success!
		}
	}

   return sockFd;
}

int SocketUtils::createTcpServer(int pPort) {
	int sockFd = -1;

	//Create a socket file descriptor if one does not already exists
	if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		//Failed to create socket
		cerr << __FUNCTION__ << " failed to create socket: " << strerror(errno) << "\n";
	} else {
		//Set socket to non-blocking so accept returns immediately
		fcntl(sockFd, F_SETFL, O_NONBLOCK);

      if (SocketUtils::setReuse(sockFd, true) < 0) { //Set socket reuse so we can create socket on port immediately after close
         //Failed to set socket reuse
         close(sockFd);
         sockFd = -1;
      } else if (SocketUtils::bindAll(sockFd, pPort) < 0) { //Bind socket to port and listen for clients
         //Failed to bind server socket
         close(sockFd);
         sockFd = -1;
      } else if (listen(sockFd, 1) < 0) { //Queue up one client connection attempt
         //Failed to listen
         cerr << __FUNCTION__ << " failed to listen on socket: " << strerror(errno) << "\n";
         close(sockFd);
         sockFd = -1;
      } else {
         //Success!
      }
	}

   return sockFd;
}

int SocketUtils::acceptTcpClient(int pSockFd, struct sockaddr *pClientAddr) {
	int clientFd = -1;
	socklen_t addrLen = sizeof(socklen_t);

   if (pSockFd >= 0) {
      if ((clientFd = accept(pSockFd, pClientAddr, &addrLen)) < 0) {
         //No client trying to connect
         //cerr << "Failed to accept on server socket: " << strerror(errno) << "\n";
      } else {
         //Client accepted, file descriptor created for connection
      }
   }

   return clientFd;
}

int SocketUtils::tcpRead(int pSockFd, char* pData, size_t pLen, int pFlags, int pTimeout) {
   // Pointer to where data should be read into
   char *currData = pData;

   // Length of data remaining to read
   int unreadLen = pLen;

   // Check for valid socket
   if (pSockFd >= 0) {

      // Keep writing until all bytes are read or an error occurs
      bool error = false;
      while (unreadLen > 0 && !error) {

         // For large amounts of data, TCP/IP protocol will send chunks
         // and wait for ACK before sending more.
         if (canRead(pSockFd, pTimeout) <= 0) {
            error = true;
         } else {

            // If there is something to read
            int nRead = recv(pSockFd, currData, unreadLen, pFlags);
            if (nRead < 0) {
               if (errno == EINTR) {
                  // Interrupted
					} else if (errno == EPIPE) {
						return SOCKET_CLOSED;
               } else {
                  error = true;
               }
            } else if (nRead == 0) {
               // EOF or socket closed
               error = true;
            } else {
               unreadLen -= nRead;
               currData += nRead;
            }
         }
      }
   } else {
      // Invalid socket
   }

   // total bytes read
   return pLen - unreadLen;
}

int SocketUtils::tcpWrite(int pSockFd, const char* pData, size_t pLen, int pFlags) {
   // Pointer to which data needs to be written
   const char *currData = pData;

   // Length of data remaining to be written
   int unwrittenLen = pLen;

   // Check for valid socket
   if (pSockFd >= 0) {

      // Keep writing until all bytes are written or error occurs
      bool error = false;
      while (unwrittenLen > 0 && !error) {
         int nWritten = send(pSockFd, currData, unwrittenLen, pFlags | MSG_NOSIGNAL);
         if (nWritten < 0) {
            if (errno == EINTR) {
               // Interrupted
				} else if (errno == EPIPE) {
					return SOCKET_CLOSED;
            } else {
               return nWritten; // return the result of write
            }
         }
         unwrittenLen -= nWritten;
         currData += nWritten;
      }
   } else {
      // Invalid socket
   }

   // total bytes written
   return pLen - unwrittenLen;
}

int SocketUtils::createUdpServer(int pPort) {
   int sockFd = -1;

   if ((sockFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      //Failed to create socket
      cerr << __FUNCTION__ << " failed to create socket: " << strerror(errno) << "\n";
   } else if (SocketUtils::setReuse(sockFd, true) < 0) {
      //Failed to set socket reuse
      close(sockFd);
      sockFd = -1;
   } else if (SocketUtils::bindAll(sockFd, pPort) < 0) {
      //Failed to bind socket
      close(sockFd);
      sockFd = -1;
   } else {
      //Success!
   }

   return sockFd;
}

int SocketUtils::createUdpClient() {
   int sockFd = -1;

   if ((sockFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      //Failed to create socket
      cerr << __FUNCTION__ << " failed to create socket: " << strerror(errno) << "\n";
   } else {
      //Success!
   }

   return sockFd;
}

string SocketUtils::getHostname(const string &pIpAddr) {
   string hostname;

   in_addr_t hostAddress = inet_addr(pIpAddr.c_str());
   hostent *host = gethostbyaddr(&hostAddress, sizeof(hostAddress), AF_INET);
   if (host == NULL) {
      cerr << __FUNCTION__ << " failed to determine hostname for " << pIpAddr << "\n";
   } else {
      hostname = host->h_name;
   }

   return hostname;
}

string SocketUtils::getAddress(const sockaddr_in &pAddr) {
   ostringstream oss;
   oss << inet_ntoa(pAddr.sin_addr);
   return oss.str();
}

int SocketUtils::getPort(const sockaddr_in &pAddr) {
   return ntohs(pAddr.sin_port);
}

int SocketUtils::canRead(int pSockFd, int pTimeout) {
   if (pSockFd > 0) {
      fd_set readFds;
      FD_ZERO(&readFds);
      FD_SET(pSockFd, &readFds);
      timeval toval;
      toval.tv_sec = pTimeout;
      toval.tv_usec = 0;
      return select(pSockFd+1, &readFds, NULL, NULL, pTimeout < 0 ? NULL : &toval);
   } else {
      return -1;
   }
}

int SocketUtils::setReuse(int pSockFd, bool pReuse) {
   int result = -1;

   int reuse = pReuse ? 1 : 0;
   if (pSockFd >= 0 && (result = setsockopt(pSockFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))) < 0) {
      cerr << __FUNCTION__ << " failed to set socket reuse: " << strerror(errno) << "\n";
   }

   return result;
}

int SocketUtils::bindAll(int pSockFd, int pPort) {
   int result = -1;

   if (pSockFd >= 0) {
      sockaddr_in myaddr;
      bzero((char*)&myaddr, sizeof(myaddr));
      myaddr.sin_family = AF_INET;
      myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
      myaddr.sin_port = htons(pPort);

      if ((result = bind(pSockFd, (struct sockaddr*)&myaddr, sizeof(myaddr))) < 0) {
         cerr << __FUNCTION__ << " failed to bind socket: " << strerror(errno) << "\n";
      }
   }

   return result;
}
