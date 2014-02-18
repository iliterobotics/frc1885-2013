#ifndef BHS_GD_CONVEYOR_DATA_H
#define BHS_GD_CONVEYOR_DATA_H

#include "Relay.h"

class bhs_GDConveyorData {
public:
	bhs_GDConveyorData();

	bool mcd_buttonIntakeCollect;
	bool mcd_buttonIntakeReverse;

	bool mcd_buttonFeederFeeding;
	bool mcd_buttonFeederReverse;

	float mcd_leftConveyorPower;
	Relay::Value mcd_vertConveyorValue;
	float mcd_rightConveyorPower;
	float mcd_backConveyorPower;

};

#endif
