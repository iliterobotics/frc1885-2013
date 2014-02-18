#include "bhs_GDDrivetrain.h"

bhs_GDDrivetrain::bhs_GDDrivetrain() {
	mdd_buttonUseArcadeDrive = false;
	mdd_joystick1X = 0;
	mdd_joystick1Y = 0;
	mdd_joystick2X = 0;
	mdd_joystick2Y = 0;

	mdd_gyroAngle = 0;
	mdd_gyroBiasVoltage = 0;
	mdd_encoderCounts = 0;
	mdd_encoderDistance = 0;

	mdd_impulseNudgeLeft = false;
	mdd_impulseNudgeRight = false;
	mdd_impulseNudgeForward = false;
	mdd_impulseNudgeBackward = false;
	mdd_extraTenthsOfSecToNudge = 0;

	mdd_startDriveAngle = false;
	mdd_stopDriveAngle = false;
	mdd_desiredAngle = 0.0;
	mdd_driveAnglePIDAddition = 0.0;
	mdd_driveAngleTimer = 0.0;

	mdd_driveLeftPower = 0.0;
	mdd_driveRightPower = 0.0;

#if TEST_DRIVE_ANGLE_PID
	mdd_buttonIncP = false;
	mdd_buttonDecP = false;
	mdd_buttonIncD = false;
	mdd_buttonDecD = false;
	mdd_resetPIDButton = false;

	mdd_gyroP = 0.0;
	mdd_gyroD = 0.0;
#endif

#if TEST_DRIVE_ANGLE_MIN_SPEEDS
	mdd_minSpeedKinetic = 0.0;
	mdd_minSpeedStatic = 0.0;

	mdd_incMinKinetic = false;
	mdd_decMinKinetic = false;
	mdd_incMinStatic = false;
	mdd_decMinStatic = false;
#endif
}
