#include "bhs_GDDriveData.h"

bhs_GDDriveData::bhs_GDDriveData()
	: mdd_buttonUseArcadeDrive(false)
	, mdd_joystick1X(0)
	, mdd_joystick1Y(0)
	, mdd_joystick2X(0)
	, mdd_joystick2Y(0)
	
	, mdd_gyroAngle(0)
	, mdd_gyroBiasVoltage(0)
	, mdd_encoderCounts(0)
	, mdd_encoderDistance(0)
	
	, mdd_driveLeftPower(0)
	, mdd_driveRightPower(0)
	
	, mdd_startDriveAngle(false)
	, mdd_stopDriveAngle(false)
	, mdd_desiredAngle(0.0)

	, mdd_driveAnglePIDAddition(0.0)
	, mdd_driveAngleTimer(0.0)

#if TEST_DRIVE_ANGLE_PID
	, mdd_buttonIncP(false)
	, mdd_buttonDecP(false)
	, mdd_buttonIncD(false)
	, mdd_buttonDecD(false)
	, mdd_resetPIDButton(false)
	
	, mdd_gyroP(bhs_Constants::PID_ANGLE_P)
	, mdd_gyroD(bhs_Constants::PID_ANGLE_D)
#endif

#if TEST_DRIVE_ANGLE_MIN_SPEEDS
	, m_minSpeedKinetic(bhs_Constants::MIN_DRIVE_ANGLE_SPEED_KINETIC)
	, m_minSpeedStatic(bhs_Constants::MIN_DRIVE_ANGLE_SPEED_STATIC)

	, m_incMinKinetic(false)
	, m_decMinKinetic(false)
	, m_incMinStatic(false)
	, m_decMinStatic(false)
#endif
{
	
}
