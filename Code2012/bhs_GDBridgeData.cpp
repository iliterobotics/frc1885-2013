#include "bhs_GDBridgeData.h"

bhs_GDBridgeData::bhs_GDBridgeData()
	: mbd_bridgeGyroAngle(0.0)

	, mbd_backDownLimitSwitch(false)
	, mbd_backUpLimitSwitch(false)
	, mbd_frontDownLimitSwitch(false)
	, mbd_frontUpLimitSwitch(false)

	, mbd_buttonFrontUp(false)
	, mbd_buttonFrontDown(false)
	, mbd_buttonFrontUp2(false)
	, mbd_buttonFrontDown2(false)

	, mbd_bridgeFrontArmValue(Relay::kOff)
{
	
}
