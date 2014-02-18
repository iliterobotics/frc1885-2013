#ifndef NETDRIVER_H
#define NETDRIVER_H

#include "binarychunkreader.h"
#include "binarychunkwriter.h"
#include "binaryencoding.h"
#include "binaryencoding-inl.h"
#include "chunk.h"
#include "chunkreader.h"
#include "chunkrwfactory.h"
#include "chunktransmission.h"
#include "chunkwriter.h"
#include "chunkwriter-inl.h"
#include "common.h"
#include "haxx.h"
#include "neterror.h"
#include "nettypes.h"
#include "rwcommon.h"
#include "rwcommon-inl.h"
#include "textchunkreader.h"
#include "textchunkwriter.h"

#include "GlobalData.h"

class NetDriver
{
	int sock;
	GlobalData* m_globals;
	unsigned char* tempGlobals;
	net::ChunkWriter* cw;
	void closeSock();
public:
	NetDriver();
	~NetDriver();
	
	bool open();
#if 0
	unsigned char recv() const;
#endif
	unsigned char send();
};
#endif
