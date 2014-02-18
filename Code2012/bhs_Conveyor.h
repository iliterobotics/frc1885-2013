#ifndef BHS_CONVEYOR_H
#define BHS_CONVEYOR_H

#include "Relay.h"

#include "bhs_Constants.h"
#include "bhs_GDConveyorData.h"

class bhs_Conveyor {
public:
	static const float INTAKE_POWER;
	static const float FEEDER_POWER;

	bhs_Conveyor(bhs_GDConveyorData*);
	~bhs_Conveyor();

	void run();

private:
	bhs_GDConveyorData* m_conveyorData;

};

#endif
