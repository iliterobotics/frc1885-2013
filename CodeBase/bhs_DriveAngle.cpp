#include "bhs_DriveAngle.h"

#define abs(x) ((x) >= 0 ? (x) : -(x))

bhs_DriveAngle::bhs_DriveAngle(bhs_GDDrivetrain* a_dd)
	: m_driveAnglePID()
	, m_timer()
{
	m_dd = a_dd;

	m_doingDriveAngle = false;
	m_driveAngleFirstTime = true;
	m_lastAngle = 0.0;

	m_isMoving = false;


	m_driveAnglePID.init(bhs_Constants::PID_ANGLE_P, bhs_Constants::PID_ANGLE_I, bhs_Constants::PID_ANGLE_D);
}

bhs_DriveAngle::~bhs_DriveAngle() {
	delete m_dd;
}

void bhs_DriveAngle::run() {
#if TEST_DRIVE_ANGLE_PID
	if (m_dd->mdd_resetPIDButton) {
		m_dd->mdd_gyroP = bhs_Constants::PID_ANGLE_P;
		m_dd->mdd_gyroD = bhs_Constants::PID_ANGLE_D;
	}

	if (m_dd->mdd_buttonIncP) {
		m_dd->mdd_gyroP += 0.00005;
	} else if (m_dd->mdd_buttonDecP) {
		m_dd->mdd_gyroP -= 0.00005;
	}

	if (m_dd->mdd_buttonIncD) {
		m_dd->mdd_gyroD += 0.000001;
	} else if (m_dd->mdd_buttonDecD) {
		m_dd->mdd_gyroD -= 0.000001;
	}

	m_driveAnglePID.init(m_dd->mdd_gyroP, 0.0, m_dd->mdd_gyroD);
#endif
	
#if TEST_DRIVE_ANGLE_MIN_SPEEDS
	if (m_dd->m_incMinKinetic) {
		m_dd->m_minSpeedKinetic += 0.001;
	} else if (m_dd->m_decMinKinetic) {
		m_dd->m_minSpeedKinetic -= 0.001;
	}

	if (m_dd->m_incMinStatic) {
		m_dd->m_minSpeedStatic += 0.001;
	} else if (m_dd->m_decMinStatic) {
		m_dd->m_minSpeedStatic -= 0.001;
	}
#endif

	if (m_dd->mdd_stopDriveAngle) {
		m_dd->mdd_stopDriveAngle = false;
		m_doingDriveAngle = false;
		
		m_timer.Stop();
	} else if (m_dd->mdd_startDriveAngle) {
		m_dd->mdd_startDriveAngle = false;
		m_doingDriveAngle = true;
		m_driveAngleFirstTime = true;
		
		m_timer.Reset();
		m_timer.Start();

		goToAngle();
	} else if (m_doingDriveAngle) {
		if (abs(m_dd->mdd_joystick1X) > bhs_Constants::JOYSTICK_DEAD_ZONE ||
				abs(m_dd->mdd_joystick1Y) > bhs_Constants::JOYSTICK_DEAD_ZONE) {
			m_doingDriveAngle = false;
		} else {
			goToAngle();
		}
	}
}

bool bhs_DriveAngle::goToAngle() {
	m_dd->mdd_driveAngleTimer = m_timer.Get();
	
	if (!m_driveAngleFirstTime &&
			abs(m_dd->mdd_gyroAngle - m_lastAngle) < bhs_Constants::DRIVE_ANGLE_DIFF_THRESHOLD &&
			abs(m_dd->mdd_gyroAngle - m_dd->mdd_desiredAngle) < bhs_Constants::DRIVE_ANGLE_THRESHOLD) {
		m_doingDriveAngle = false; // the robot is at the angle, no longer need to make adjustments

		m_dd->mdd_driveAnglePIDAddition = 0.0;

		return true;
	} else {
		m_driveAngleFirstTime = false;

		if (abs(m_dd->mdd_gyroAngle - m_lastAngle) > bhs_Constants::DRIVE_ANGLE_DIFF_THRESHOLD) {
			m_isMoving = true;
		} else {
			m_isMoving = false;
		}
		

		m_dd->mdd_driveAnglePIDAddition = -m_driveAnglePID.apply(m_dd->mdd_gyroAngle - m_dd->mdd_desiredAngle);
		float l_pidAddition = processMinSpeed(m_dd->mdd_driveAnglePIDAddition);
		m_dd->mdd_driveLeftPower = limit(l_pidAddition);
		m_dd->mdd_driveRightPower = limit(-l_pidAddition);

		m_lastAngle = m_dd->mdd_gyroAngle;
		return false;
	}
}

float bhs_DriveAngle::limit(float a_motorSpeed) {
	if (a_motorSpeed > 1.0) {
		return 1.0;
	} else if (a_motorSpeed < -1.0) {
		return -1.0;
	} else {
		return a_motorSpeed;
	}
}

float bhs_DriveAngle::processMinSpeed(float a_motorSpeed) {
#if TEST_DRIVE_ANGLE_MIN_SPEEDS
	if (abs(a_motorSpeed) > 0.005) {
		if (m_isMoving) {
			if (a_motorSpeed < 0.0) {
				return a_motorSpeed - m_dd->m_minSpeedKinetic;
			} else {
				return a_motorSpeed + m_dd->m_minSpeedKinetic;
			}
		} else {
			if (a_motorSpeed < 0.0) {
				return a_motorSpeed - m_dd->m_minSpeedStatic;
			} else {
				return a_motorSpeed + m_dd->m_minSpeedStatic;
			}
		}
	}
#else
	if (abs(a_motorSpeed) > 0.005) {
		if (m_isMoving) {
			if (a_motorSpeed < 0.0) {
				return a_motorSpeed - bhs_Constants::MIN_DRIVE_ANGLE_SPEED_KINETIC;
			} else {
				return a_motorSpeed + bhs_Constants::MIN_DRIVE_ANGLE_SPEED_KINETIC;
			}
		} else
			if (a_motorSpeed < 0.0) {
				return a_motorSpeed - bhs_Constants::MIN_DRIVE_ANGLE_SPEED_STATIC;
			} else {
				return a_motorSpeed + bhs_Constants::MIN_DRIVE_ANGLE_SPEED_STATIC;
			}
		}
#endif
	return 0.0;
}
