#ifndef BHS_DRIVE_TRAIN_H
#define BHS_DRIVE_TRAIN_H

#include "bhs_Constants.h"
#include "bhs_GlobalData.h"
#include "bhs_PID.h"

class bhs_DriveTrain {
public:
	bhs_DriveTrain(bhs_GDDriveData*);
	~bhs_DriveTrain();

	void run();

private:
	bhs_GDDriveData* m_driveData;
	bhs_PID m_driveStraightPID;
	bool m_driveStraight;
	float m_desiredAngle;

	void arcadeDrive();
	void tankDrive();

	float limit(float);
	float deadzone(float);

};

#endif // BHS_DRIVE_TRAIN_H
