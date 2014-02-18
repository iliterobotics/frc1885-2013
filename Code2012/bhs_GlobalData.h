#ifndef BHS_GLOBAL_DATA_H
#define BHS_GLOBAL_DATA_H

#include "bhs_Constants.h"
#include "bhs_CameraData.h"
#include "bhs_GDDriveData.h"
#include "bhs_GDConveyorData.h"
#include "bhs_GDShooterData.h"
#include "bhs_GDBridgeData.h"
#include "bhs_GDTransmitData.h"

class bhs_GlobalData :
	public bhs_CameraData,
	public bhs_GDDriveData,
	public bhs_GDConveyorData,
	public bhs_GDShooterData,
	public bhs_GDBridgeData,
	public bhs_GDTransmitData {
public:
	bhs_GlobalData();

	int m_autonomousNumber;
	float m_autonomousDelay;
	
	bool m_impulseNudgeLeft;
	bool m_impulseNudgeRight;
	bool m_impulseNudgeForward;
	bool m_impulseNudgeBackward;
	int m_extraTenthsOfSecToNudge;
		
};

#endif // BHS_GLOBAL_DATA_H
