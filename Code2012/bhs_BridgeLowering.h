#ifndef BHS_BRIDGE_LOWERING_H
#define BHS_BRIDGE_LOWERING_H

#include "DriverStationLCD.h"
#include "Relay.h"

#include "bhs_Constants.h"
#include "bhs_GDBridgeData.h"

class bhs_BridgeLowering {
public:
	bhs_BridgeLowering(bhs_GDBridgeData*);
	~bhs_BridgeLowering();

	void run();

private:
	bhs_GDBridgeData* m_bridgeData;

};

#endif
