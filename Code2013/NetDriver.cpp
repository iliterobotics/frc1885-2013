#include "NetDriver.h"

#include <signal.h>

#define DRIVER_IP "10.18.85.5"
#define DRIVER_PORT 1180

NetDriver::NetDriver()
{
	sock= -1;
	signal(SIGPIPE, SIG_IGN);
	m_globals= GlobalData::GetInstance();
	tempGlobals= new unsigned char[sizeof(GlobalData)];
}
NetDriver::~NetDriver()
{
	closeSock();
	delete[] tempGlobals;
	delete cw;
}
bool NetDriver::open()
{
	closeSock();
	if((sock= socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("\nERROR: NetDriver: socket(): ");
		switch(errno)
		{
		case ENOTSUP: perror("address family or socket operation not supported.");
		break;

		case ENOSYS: perror("component INCLUDE_SC_SOCKLIB not configured into the kernel.");
		break;

		default: perror("Unknown error.");
		}
		return true;
	}
	struct sockaddr_in serv_addr;
	bzero(reinterpret_cast<char* const>(&serv_addr), sizeof serv_addr);
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_addr.s_addr= inet_addr(DRIVER_IP);
	serv_addr.sin_port= htons(DRIVER_PORT);
	if(connect(sock, reinterpret_cast<struct sockaddr* const>(&serv_addr), sizeof serv_addr) == ERROR)
	{
		perror("\nERROR: NetDriver: connect(): ");
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
void NetDriver::closeSock()
{
	if(sock >= 0)
		close(sock);
	sock= -1;
}
#if 0
unsigned char NetDriver::recv() const
{
	net::Chunk* chunk;
	unsigned char result;
	if(result= net::chunktransmission::readChunk(sock, &chunk))
	{
		printf("\nERROR: NetDriver: readChunk(): %s", net::error::stringFrom(result).c_str());
		return result;
	}
	printf("\nDriver: Size: %d", chunk->size());

	net::ChunkReader* const cr= net::chunkrw::readerFor(chunk);

	net::byte idSubSystem;
	double value;

	if(result= cr->nextByte(&idSubSystem))
	{
		printf("\nERROR: NetDriver: idMessage: %s", net::error::stringFrom(result).c_str());
		goto cleanup;
	}
	else
		printf("\nNetDriver: idMessage: %d", idSubSystem);
	if(result= cr->nextByte(&idSubSystem))
	{
		printf("\nERROR: NetDriver: idSubSystem: %s", net::error::stringFrom(result).c_str());
		goto cleanup;
	}
	else
		printf("\nNetDriver: idSubSystem: %d", idSubSystem);
	if(result= cr->nextDouble(&value))
	{
		printf("\nERROR: NetDriver: value: %s", net::error::stringFrom(result).c_str());
		goto cleanup;
	}
	else
		printf("\nNetDriver: value: %f", value);

	semTake(m_globals->semaphore, WAIT_FOREVER);
	switch(idSubSystem)
	{
		case 0: m_globals->d_autoEnabled= value != 0.0; break;

		case 1: m_globals->c_straightP= value; break;
		case 2: m_globals->c_straightI= value; break;
		case 3: m_globals->c_straightD= value; break;

		case 4: m_globals->c_turnP= value; break;
		case 5: m_globals->c_turnI= value; break;
		case 6: m_globals->c_turnD= value; break;

		case 7: m_globals->c_tiltP= value; break;
		case 8: m_globals->c_tiltI= value; break;
		case 9: m_globals->c_tiltD= value; break;
		default: perror("\nWARNING: NetDriver: Invalid Request");
	}
	cleanup:
	semGive(m_globals->semaphore);
	delete chunk;
	delete cr;
	return result;
}
#endif
unsigned char NetDriver::send()
{
	cw= net::chunkrw::preferredWriter();
	semTake(m_globals->semaphore, WAIT_FOREVER);

	memcpy(tempGlobals, m_globals, sizeof(GlobalData));

	semGive(m_globals->semaphore);
	const GlobalData* const t_m_globals= reinterpret_cast<const GlobalData* const>(tempGlobals);

	cw->writeDouble(t_m_globals->o_drivetrainLeft);
	cw->writeDouble(t_m_globals->o_drivetrainRight);

	cw->writeDouble(t_m_globals->o_launcherTilt);
	cw->writeDouble(t_m_globals->o_launcherSpeed);
	cw->writeDouble(t_m_globals->o_intake);

	cw->writeByte(t_m_globals->o_matchState);

	cw->writeDouble(t_m_globals->d_joystick_1_x);
	cw->writeDouble(t_m_globals->d_joystick_1_y);
	cw->writeDouble(t_m_globals->d_joystick_2_x);
	cw->writeDouble(t_m_globals->d_joystick_2_y);

	cw->writeBoolean(t_m_globals->d_autoEnabled);
	cw->writeByte(t_m_globals->d_launcherPresetSwitch);
	cw->writeBoolean(t_m_globals->d_hopperSwitch);
	cw->writeBoolean(t_m_globals->d_launcherFireSequence);
	cw->writeBoolean(t_m_globals->d_manualKick);
	cw->writeBoolean(t_m_globals->d_hangSafetySwitch);
	cw->writeDouble(t_m_globals->d_launcherManualSpeed);
	cw->writeDouble(t_m_globals->d_launcherManualTilt);

	cw->writeBoolean(t_m_globals->i_comp_isEnabled);
	cw->writeBoolean(t_m_globals->o_hang);
	cw->writeBoolean(t_m_globals->o_hopper);
	cw->writeBoolean(t_m_globals->o_spatula);
	cw->writeBoolean(t_m_globals->o_launcherKicker);

	cw->writeDouble(t_m_globals->i_launcherVisionTilt);
	cw->writeDouble(t_m_globals->i_launcherVisionHorizontalAngle);
	cw->writeDouble(t_m_globals->i_launcherVisionSpeed);

	cw->writeInt(t_m_globals->i_encoderLeft);
	cw->writeInt(t_m_globals->i_encoderRight);
	cw->writeDouble(t_m_globals->i_gyroAngle);
	cw->writeDouble(t_m_globals->i_launcherTiltPotentiometer);
	cw->writeInt(t_m_globals->i_comp);

	//printf("\nNetDriver: send(): Size: %d", cw->chunkSize());

	unsigned char result;
	if(result= net::chunktransmission::writeChunk(sock, cw->chunk()))
		printf("\nERROR: NetDriver: writeChunk(): %s", net::error::stringFrom(result).c_str());

	delete cw;

	return result;
}
