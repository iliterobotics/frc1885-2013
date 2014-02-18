#include "bhs_DriverInput.h"

bhs_DriverInput::bhs_DriverInput(bhs_GlobalData* a_gd) 
	: m_joystick1(JOYSTICK_DRIVER_1_PORT)
	, m_joystick2(JOYSTICK_DRIVER_2_PORT)
	, m_buttonboard(JOYSTICK_OPERATOR_1_PORT)
{
	m_gd = a_gd;

#if TEST_DRIVE_ANGLE_ANGLES
	m_driveAngleImpulse = new bhs_ButtonImpulse[7];
#endif
}

bhs_DriverInput::~bhs_DriverInput() {
	m_gd = 0;
#if TEST_DRIVE_ANGLE_ANGLES
	delete[] m_driveAngleImpulse;
#endif
}

void bhs_DriverInput::init() {
}

void bhs_DriverInput::run() {
#if USE_DRIVER_JOYSTICKS
	m_gd->mdd_joystick1X = m_joystick1.GetAxis(Joystick::kXAxis);
	m_gd->mdd_joystick1Y = -m_joystick1.GetAxis(Joystick::kYAxis);
	m_gd->mdd_joystick2X = m_joystick2.GetAxis(Joystick::kXAxis);
	m_gd->mdd_joystick2Y = -m_joystick2.GetAxis(Joystick::kYAxis);

	m_gd->mdd_buttonUseArcadeDrive = m_joystick1.GetRawButton(BUTTON_USE_ARCADE_DRIVE);

	m_gd->mdd_impulseNudgeForward = m_impulseNudgeForward.apply(m_joystick1.GetRawButton(BUTTON_NUDGE_FORWARD));
	m_gd->mdd_impulseNudgeBackward = m_impulseNudgeBackward.apply(m_joystick1.GetRawButton(BUTTON_NUDGE_BACKWARD));
	m_gd->mdd_impulseNudgeLeft = m_impulseNudgeLeft.apply(m_joystick2.GetRawButton(BUTTON_NUDGE_LEFT));
	m_gd->mdd_impulseNudgeRight = m_impulseNudgeRight.apply(m_joystick2.GetRawButton(BUTTON_NUDGE_RIGHT));

	m_gd->mdd_extraTenthsOfSecToNudge = 0;
	if (DriverStation::GetInstance()->GetDigitalIn(1))
		m_gd->mdd_extraTenthsOfSecToNudge++;
	if (DriverStation::GetInstance()->GetDigitalIn(2))
		m_gd->mdd_extraTenthsOfSecToNudge++;
#endif


#if USE_OPERATOR_JOYSTICKS
#endif


#if TEST_DRIVE_ANGLE_MIN_SPEEDS
	m_gd->mdd_incMinKinetic = m_joystick2.GetRawButton(3);
	m_gd->mdd_decMinKinetic = m_joystick2.GetRawButton(2);
	m_gd->mdd_incMinStatic = m_joystick2.GetRawButton(5);
	m_gd->mdd_decMinStatic = m_joystick2.GetRawButton(4);
#endif


#if TEST_DRIVE_ANGLE_PID
	m_gd->mdd_buttonIncP = m_joystick2.GetRawButton(6);
	m_gd->mdd_buttonDecP = m_joystick2.GetRawButton(7);
	m_gd->mdd_buttonIncD = m_joystick2.GetRawButton(11);
	m_gd->mdd_buttonDecD = m_joystick2.GetRawButton(10);
	m_gd->mdd_resetPIDButton = m_joystick2.GetRawButton(1);
#endif


#if TEST_DRIVE_ANGLE_ANGLES
	if (m_driveAngleImpulse[0].apply(m_joystick1.GetRawButton(6))) {
		m_gd->mdd_startDriveAngle = true;
		m_gd->mdd_desiredAngle = 0;
	}  else if (m_driveAngleImpulse[1].apply(m_joystick1.GetRawButton(7))) {
		m_gd->mdd_startDriveAngle = true;
		m_gd->mdd_desiredAngle = 1;
	} else if (m_driveAngleImpulse[2].apply(m_joystick1.GetRawButton(8))) {
		m_gd->mdd_startDriveAngle = true;
		m_gd->mdd_desiredAngle = 5;
	} else if (m_driveAngleImpulse[3].apply(m_joystick1.GetRawButton(9))) {
		m_gd->mdd_startDriveAngle = true;
		m_gd->mdd_desiredAngle = 7;
	} else if (m_driveAngleImpulse[4].apply(m_joystick1.GetRawButton(10))) {
		m_gd->mdd_startDriveAngle = true;
		m_gd->mdd_desiredAngle = 10;
	} else if (m_driveAngleImpulse[5].apply(m_joystick1.GetRawButton(11))) {
		m_gd->mdd_startDriveAngle = true;
		m_gd->mdd_desiredAngle = m_gd->mdd_gyroAngle + 180;
	} else if (m_driveAngleImpulse[6].apply(m_joystick1.GetRawButton(1))) {
		m_gd->mdd_stopDriveAngle = true;
	}
#endif


#if TEST_DRIVE_ANGLE_GYRO_DEADBAND
	if (m_joystick2.GetRawButton(8)) {
		m_driveGyro.incDeadband(-10);
	} else if (m_joystick2.GetRawButton(9)) {
		m_driveGyro.incDeadband(10);
	}
#endif

}
