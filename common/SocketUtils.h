#ifndef _SOCKET_UTILS_H_
#define _SOCKET_UTILS_H_

#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

class SocketUtils {
public:

	static const int SOCKET_CLOSED = -3;

   // TCP Client
	static int createTcpClient(const char* pHost, int pPort);

   // TCP Server
	static int createTcpServer(int pPort);
	static int acceptTcpClient(int pServerFd, struct sockaddr *pClientAddr);

   // TCP I/O

   // If pTimeout < 0, block until something is read or error
   // or pTimeout == 0, return immediately
   // or pTimeout > 0, block until something is read, error, or timeout
   static int tcpRead(int pSockFd, char* pData, size_t pLen, int pFlags=0, int pTimeout=0);
   static int tcpWrite(int pSockFd, const char* pData, size_t pLen, int pFlags=0);

   // UDP Client
   static int createUdpClient();

   // UDP Server
   static int createUdpServer(int pPort);

   // Utility Functions
   static string getHostname(const string &pIpAddr);
   static string getAddress(const sockaddr_in &pAddr);
   static int getPort(const sockaddr_in &pAddr);

private:

   static int canRead(int pSockFd, int pTimeout=0);
   static int setReuse(int pSockFd, bool pReuse);
   static int bindAll(int pSockFd, int pPort);
};

#endif


