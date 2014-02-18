#ifndef BHS_PC_DATA_SERVER_H
#define BHS_PC_DATA_SERVER_H

#include <vxWorks.h>

#include <errnoLib.h>
#include <hostLib.h>
#include <inetLib.h>
#include <sockLib.h>
#include <semLib.h>

#include "Task.h"
#include "Timer.h"
#include "Utility.h"

#define VIDEO_TO_PC_PORT 1180

class bhs_PCDataServer 
{
public:
	bhs_PCDataServer(void);
	~bhs_PCDataServer(void);
	unsigned int Release(void);

	bool ready(void);
	bool xmitData(char*, int);
	bool xmitImage(char*, int);
	bool xmitImage2(char*, int);

	static int ServerSocketTask(bhs_PCDataServer* apThis);

private:
	int ServerTask();

	void xmitRawData(int header,int length,char* apData);

	bool mbServerReady;
	SEM_ID mExternalUserSem;
	SEM_ID mServerReadySem;
	SEM_ID mCurrentConnectionDone;

	int mConnectedSocket;

};

#endif
