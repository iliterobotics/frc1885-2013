#include "bhs_DriveTrain.h"

#define abs(x) ((x) >= 0 ? (x) : -(x))

#define TEST_PID 0

bhs_DriveTrain::bhs_DriveTrain(bhs_GDDriveData* a_driveData)
	: m_driveData(a_driveData)
	, m_driveStraightPID()
	, m_driveStraight(false)
	, m_desiredAngle(0.0)
{
	m_driveStraightPID.init(bhs_Constants::PID_STRAIGHT_P, bhs_Constants::PID_STRAIGHT_I, bhs_Constants::PID_STRAIGHT_D);
}

bhs_DriveTrain::~bhs_DriveTrain() {
	m_driveData = 0;
}

void bhs_DriveTrain::run() {
#if USE_TANK_DRIVE
	if (m_driveData->mdd_buttonUseArcadeDrive) {
		arcadeDrive();
	} else {
		tankDrive();
	}
#else
	arcadeDrive();
#endif
}

void bhs_DriveTrain::arcadeDrive() {
#if TEST_PID
	// TODO: remove all of this code when PID has been calibrated properly

	if (m_driveData->mdd_resetPIDButton) {
		m_driveData->mdd_gyroP = bhs_Constants::PID_ANGLE_P_SMALL_DIFF;
		//driveData->mdd_gyroI = bhs_Constants::GYRO_I;
		m_driveData->mdd_gyroD = bhs_Constants::PID_ANGLE_D;
	}

	if (m_driveData->mdd_buttonIncP) {
		m_driveData->mdd_gyroP += 0.001;
	} else if (m_driveData->mdd_buttonDecP) {
		m_driveData->mdd_gyroP -= 0.001;
	}

	if (m_driveData->mdd_buttonIncD) {
		m_driveData->mdd_gyroD += 0.00008; // set to 0.00008 because the D value is much smaller than the P value
	} else if (m_driveData->mdd_buttonDecD) {
		m_driveData->mdd_gyroD -= 0.00008;
	}

	m_driveStraightPID.init(m_driveData->mdd_gyroP, 0.0, m_driveData->mdd_gyroD);
#endif

	//*
	float l_forwardBackSpeed = deadzone(m_driveData->mdd_joystick2Y);
	float l_leftRightSpeed;

	// the robot can only be in drive straight if the joysticks are in the proper orientation
	if (abs(m_driveData->mdd_joystick2X) < bhs_Constants::JOYSTICK_DEAD_ZONE &&
			abs(m_driveData->mdd_joystick2Y) > bhs_Constants::JOYSTICK_DEAD_ZONE) {
		// if we are not already in drive straight, set drive straight on and
		// then set the desired angle to the current gyro reading so that the
		// robot can lock on to this angle as it moves
		// if the robot is already in drive straight, then the desired angle
		// has already been set! only set it for the first iteration
		if (!m_driveStraight) {
			m_driveStraightPID.reset();
			m_desiredAngle = m_driveData->mdd_gyroAngle;
			m_driveStraight = true;
		}
	} else {
		m_driveStraight = false;
	}

	if (m_driveStraight) {
		m_driveData->mdd_driveAnglePIDAddition = -m_driveStraightPID.apply(m_driveData->mdd_gyroAngle - m_desiredAngle);
		l_leftRightSpeed = m_driveData->mdd_driveAnglePIDAddition;
	} else {
		l_leftRightSpeed = deadzone(m_driveData->mdd_joystick2X);
	}
	//*/

	m_driveData->mdd_driveLeftPower = limit(l_forwardBackSpeed + l_leftRightSpeed);
	m_driveData->mdd_driveRightPower = limit(l_forwardBackSpeed - l_leftRightSpeed);
}

void bhs_DriveTrain::tankDrive() {
	m_driveData->mdd_driveLeftPower = deadzone(m_driveData->mdd_joystick1Y);
	m_driveData->mdd_driveRightPower = deadzone(m_driveData->mdd_joystick2Y);
}

float bhs_DriveTrain::limit(float a_motorSpeed) {
	if (a_motorSpeed > 1.0) {
		return 1.0;
	} else if (a_motorSpeed < -1.0) {
		return -1.0;
	} else {
		return a_motorSpeed;
	}
}

float bhs_DriveTrain::deadzone(float a_joystickValue) {
	if (abs(a_joystickValue) < bhs_Constants::JOYSTICK_DEAD_ZONE) {
		return 0.0;
	} else if (a_joystickValue > 1.0) {
		return 1.0;
	} else if (a_joystickValue < -1.0) {
		return -1.0;
	} else {
		if (a_joystickValue > 0.0) {
			return (a_joystickValue - bhs_Constants::JOYSTICK_DEAD_ZONE) / (1.0 - bhs_Constants::JOYSTICK_DEAD_ZONE);
		} else {
			return (a_joystickValue + bhs_Constants::JOYSTICK_DEAD_ZONE) / (1.0 - bhs_Constants::JOYSTICK_DEAD_ZONE);
		}
	}
}
