#ifndef _TARGETLOCATOR_H_
#define _TARGETLOCATOR_H_

#include "RobotGlobalData.h"
#include "CameraTurret.h"

class TargetLocator
{
	public:
		TargetLocator();
		void Initialize(RobotGlobalData *apData, CameraTurret *apTurret, SEM_ID apSem);
		
		void targetMainLoop();

	private:
		void nextAngle();
		
		static const int PAN_ANGLE_CHANGE = 20;
		static const int TILT_ANGLE_CHANGE = 20;
		static const int MIN_PAN_ANGLE = 0;
		static const int MAX_PAN_ANGLE = 170;
		static const int MIN_TILT_ANGLE = 100;
		static const int MAX_TILT_ANGLE = 170;
		
		CameraTurret *mpTurret;
		RobotGlobalData *mpData;
		SEM_ID mpSem;
		INT32 mnTargetingTaskId;
		
		enum teState{eeSCAN, eeSERVO_WAIT, eeFIND, eeTRACK};
		teState meCurrentState;
		
		int mnTiltAngle;
		int mnPanAngle;
		int mnPanDirection;
		int mnTiltDirection;
};

#endif
