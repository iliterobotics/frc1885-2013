#ifndef BHS_DRIVE_ANGLE_H
#define BHS_DRIVE_ANGLE_H

#include "Timer.h"

#include "bhs_Constants.h"
#include "bhs_GDDriveData.h"
#include "bhs_PID.h"

class bhs_DriveAngle {
public:
	bhs_DriveAngle(bhs_GDDriveData*);
	~bhs_DriveAngle();

	void run();
	bool goToAngle();

private:
	bhs_GDDriveData* m_driveData;
	bhs_PID m_driveAnglePID;

	Timer m_timer;

	bool m_doingDriveAngle;
	bool m_driveAngleFirstTime;
	float m_lastAngle;

	bool m_isMoving;

	float limit(float);
	float processMinSpeed(float);

};

#endif
