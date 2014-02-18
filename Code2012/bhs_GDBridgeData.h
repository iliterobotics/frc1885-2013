#ifndef BHS_GD_BRIDGE_DATA_H
#define BHS_GD_BRIDGE_DATA_H

#include "Relay.h"

class bhs_GDBridgeData {
public:
	bhs_GDBridgeData();
	
	float mbd_bridgeGyroAngle;

	bool mbd_backDownLimitSwitch;
	bool mbd_backUpLimitSwitch;
	bool mbd_frontDownLimitSwitch;
	bool mbd_frontUpLimitSwitch;

	bool mbd_buttonFrontUp;
	bool mbd_buttonFrontDown;
	bool mbd_buttonFrontUp2;
	bool mbd_buttonFrontDown2;

	Relay::Value mbd_bridgeFrontArmValue;

};

#endif
