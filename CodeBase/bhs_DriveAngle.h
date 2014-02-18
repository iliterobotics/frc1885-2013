#ifndef BHS_DRIVE_ANGLE_H_
#define BHS_DRIVE_ANGLE_H_

#include "Timer.h"

#include "bhs_Constants.h"
#include "bhs_GDDrivetrain.h"
#include "bhs_PID.h"

class bhs_DriveAngle {
	public:
		bhs_DriveAngle(bhs_GDDrivetrain*);
		~bhs_DriveAngle();

		void run();
		bool goToAngle();

	private:
		bhs_GDDrivetrain* m_dd;
		bhs_PID m_driveAnglePID;

		Timer m_timer;

		bool m_doingDriveAngle;
		bool m_driveAngleFirstTime;
		float m_lastAngle;

		bool m_isMoving;

		float limit(float);
		float processMinSpeed(float);

};

#endif //BHS_DRIVE_ANGLE_H_
