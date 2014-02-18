#ifndef NETVISION_H
#define NETVISION_H

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

class NetVision
{
	int sock;
	GlobalData* m_globals;
	void closeSock();
public:
	NetVision();
	~NetVision();
	
	bool open();
	
	unsigned char recv() const;
	unsigned char send() const;
};
#endif
