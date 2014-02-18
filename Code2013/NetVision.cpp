#include "NetVision.h"

#include <signal.h>

#define KINECT_IP "10.18.85.11"
#define KINECT_PORT 2221

NetVision::NetVision()
{
	sock= -1;
	signal(SIGPIPE, SIG_IGN);
	m_globals= GlobalData::GetInstance();
}
NetVision::~NetVision()
{
	closeSock();
}
bool NetVision::open()
{
	closeSock();
	if((sock= socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("ERROR: NetVision: socket()\n");
		return true;
	}
	struct sockaddr_in serv_addr;
	bzero(reinterpret_cast<char* const>(&serv_addr), sizeof serv_addr);
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_addr.s_addr= inet_addr(KINECT_IP);
	serv_addr.sin_port= htons(KINECT_PORT);
	if(connect(sock, reinterpret_cast<struct sockaddr* const>(&serv_addr), sizeof serv_addr) < 0)
	{
		perror("ERROR: NetVision: connect()\n");
		switch(errno)
		{
		case EINVAL: perror("struct serv_addr invalid");
		break;

		case EBADF: perror("socket descriptor invalid.");
		break;

		case ENOTSUP: perror("connect operation not supported.");
		break;

		case ENOSYS: perror("component INCLUDE_SC_SOCKLIB not configured into the kernel.");
		break;

		case ECONNREFUSED: perror("connection refused.");
		break;

		case EALREADY:
		case EINPROGRESS: perror("connection attempt pending.");
		break;

		case EISCONN: perror("connection established.");
		break;

		default: perror("Unknown error.");
		}
		return true;
	}
	return false;
}
void NetVision::closeSock()
{
	if(sock >= 0)
		close(sock);
	sock= -1;
}
unsigned char NetVision::recv() const
{
	net::Chunk* chunk;
	unsigned char result;
	if(result= net::chunktransmission::readChunk(sock, &chunk))
	{
		printf("\nERROR: NetVision: readChunk(): %s", net::error::stringFrom(result).c_str());
		return result;
	}
	printf("\nNetVision: Size: %d", chunk->size());

	net::ChunkReader* const cr= net::chunkrw::readerFor(chunk);

	double tilt, angle, speed;
	if(result= cr->nextDouble(&tilt))
	{
		printf("\nERROR: NetVision: tilt: %s", net::error::stringFrom(result).c_str());
		goto cleanup;
	}
	if(result= cr->nextDouble(&angle))
	{
		printf("\nERROR: NetVision: angle: %s", net::error::stringFrom(result).c_str());
		goto cleanup;
	}
	if(result= cr->nextDouble(&speed))
	{
		printf("\nERROR: NetVision: speed: %s", net::error::stringFrom(result).c_str());
		goto cleanup;
	}
	semTake(m_globals->semaphore, WAIT_FOREVER);
	m_globals->i_launcherVisionTilt= tilt;
	m_globals->i_launcherVisionHorizontalAngle= angle;
	m_globals->i_launcherVisionSpeed= speed;
	semGive(m_globals->semaphore);

	if(tilt != 0.0||angle != 0.0||speed != 0.0)
	{
		printf("\nNetVision: tilt: %f",  tilt);
		printf("\nNetVision: angle: %f", angle);
		printf("\nNetVision: speed: %f", speed);
		m_globals->i_requestForVision = false;
	}
	else
		printf("\nNetVision: 0 Blobs.");
	cleanup:
	delete cr;
	delete chunk;
	return result;
}
unsigned char NetVision::send() const
{
	net::ChunkWriter* const cw= net::chunkrw::preferredWriter();

	cw->writeBoolean(true);

	printf("\nNetVision: send(): Size: %d", cw->chunkSize());

	unsigned char result;
	if(result= net::chunktransmission::writeChunk(sock, cw->chunk()))
		printf("\nERROR: NetVision: writeChunk(): %s", net::error::stringFrom(result).c_str());

	delete cw;

	return result;
}
