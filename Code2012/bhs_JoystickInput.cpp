#include "bhs_JoystickInput.h"

const UINT32 bhs_JoystickInput::JOYSTICK_DRIVER_1_PORT = 1;
const UINT32 bhs_JoystickInput::JOYSTICK_DRIVER_2_PORT = 2;
const UINT32 bhs_JoystickInput::JOYSTICK_OPERATOR_1_PORT = 3;
const UINT32 bhs_JoystickInput::JOYSTICK_OPERATOR_2_PORT = 4;

const UINT32 bhs_JoystickInput::BUTTON_USE_ARCADE_DRIVE = 6;
const UINT32 bhs_JoystickInput::BUTTON_ARM_FRONT_UP = 3;
const UINT32 bhs_JoystickInput::BUTTON_ARM_FRONT_DOWN = 2;
const UINT32 bhs_JoystickInput::BUTTON_NUDGE_FORWARD = 8;
const UINT32 bhs_JoystickInput::BUTTON_NUDGE_BACKWARD = 9;
const UINT32 bhs_JoystickInput::BUTTON_NUDGE_LEFT = 8;
const UINT32 bhs_JoystickInput::BUTTON_NUDGE_RIGHT = 9;

const UINT32 bhs_JoystickInput::BUTTON_LOCK_DISTANCE_FOR_SHOOTER = 1;
const UINT32 bhs_JoystickInput::BUTTON_FEEDER_FEEDING = 2;
const UINT32 bhs_JoystickInput::BUTTON_HOOP_OVERRIDE_TOP = 3;
const UINT32 bhs_JoystickInput::BUTTON_HOOP_OVERRIDE_BOTTOM = 4;
const UINT32 bhs_JoystickInput::BUTTON_SHOOTER_LAY_UP = 5;
const UINT32 bhs_JoystickInput::SWITCH_SHOOTER_SPEED_OVERRIDE = 6;
const UINT32 bhs_JoystickInput::BUTTON_FEEDER_REVERSE = 7;
const UINT32 bhs_JoystickInput::SWITCH_INTAKE_COLLECT = 8;
const UINT32 bhs_JoystickInput::SWITCH_INTAKE_REVERSE = 9;
const UINT32 bhs_JoystickInput::BUTTON_SHOOTER_POWER_SHOT = 10;

const Joystick::AxisType bhs_JoystickInput::POTENTIOMETER_AXIS_TYPE = Joystick::kXAxis;
const float bhs_JoystickInput::POTENTIOMETER_DEAD_ZONE = 0.1;
const float bhs_JoystickInput::POTENTIOMETER_MIN_VAL = 0.7;

bhs_JoystickInput::bhs_JoystickInput(bhs_GlobalData* a_globalData)
: m_globalData(a_globalData)

#if USE_DRIVER_JOYSTICKS
, m_jsDriver1(JOYSTICK_DRIVER_1_PORT)
#if USE_TWO_DRIVER_JOYSTICKS
, m_jsDriver2(JOYSTICK_DRIVER_2_PORT)
#endif
#endif

#if USE_OPERATOR_JOYSTICKS
, m_jsOperator1(JOYSTICK_OPERATOR_1_PORT)
#endif
{
#if TEST_DRIVE_ANGLE_ANGLES
	m_driveAngleImpulse = new bhs_ButtonImpulse[7];
#endif
}

bhs_JoystickInput::~bhs_JoystickInput() {
	m_globalData = 0;
#if TEST_DRIVE_ANGLE_ANGLES
	delete[] m_driveAngleImpulse;
#endif
}

void bhs_JoystickInput::init() {

}

void bhs_JoystickInput::run() {
#if USE_DRIVER_JOYSTICKS
#if USE_TWO_DRIVER_JOYSTICKS
	m_globalData->mdd_joystick1X = m_jsDriver1.GetAxis(Joystick::kXAxis);
	m_globalData->mdd_joystick1Y = -m_jsDriver1.GetAxis(Joystick::kYAxis);
	m_globalData->mdd_joystick2X = m_jsDriver2.GetAxis(Joystick::kXAxis);
	m_globalData->mdd_joystick2Y = -m_jsDriver2.GetAxis(Joystick::kYAxis);

	m_globalData->mdd_buttonUseArcadeDrive = m_jsDriver1.GetRawButton(BUTTON_USE_ARCADE_DRIVE);
	m_globalData->mbd_buttonFrontUp = m_jsDriver1.GetRawButton(BUTTON_ARM_FRONT_UP);
	m_globalData->mbd_buttonFrontDown = m_jsDriver1.GetRawButton(BUTTON_ARM_FRONT_DOWN);
	m_globalData->mbd_buttonFrontUp2 = m_jsDriver2.GetRawButton(BUTTON_ARM_FRONT_UP);
	m_globalData->mbd_buttonFrontDown2 = m_jsDriver2.GetRawButton(BUTTON_ARM_FRONT_DOWN);

	m_globalData->m_impulseNudgeForward = m_impulseNudgeForward.apply(m_jsDriver1.GetRawButton(BUTTON_NUDGE_FORWARD));
	m_globalData->m_impulseNudgeBackward = m_impulseNudgeBackward.apply(m_jsDriver1.GetRawButton(BUTTON_NUDGE_BACKWARD));
	m_globalData->m_impulseNudgeLeft = m_impulseNudgeLeft.apply(m_jsDriver2.GetRawButton(BUTTON_NUDGE_LEFT));
	m_globalData->m_impulseNudgeRight = m_impulseNudgeRight.apply(m_jsDriver2.GetRawButton(BUTTON_NUDGE_RIGHT));

	m_globalData->m_extraTenthsOfSecToNudge = 0;
	if (DriverStation::GetInstance()->GetDigitalIn(1)) {
		m_globalData->m_extraTenthsOfSecToNudge++;
	}
	if (DriverStation::GetInstance()->GetDigitalIn(2)) {
		m_globalData->m_extraTenthsOfSecToNudge++;
	}
#else
	m_globalData->mdd_buttonUseArcadeDrive = true;
	m_globalData->mdd_joystick2X = m_jsDriver1.GetAxis(Joystick::kXAxis);
	m_globalData->mdd_joystick2Y = -m_jsDriver1.GetAxis(Joystick::kYAxis);
#endif
#endif

#if USE_OPERATOR_JOYSTICKS
	m_globalData->msd_switchSpeedOverride = m_jsOperator1.GetRawButton(SWITCH_SHOOTER_SPEED_OVERRIDE);
	m_globalData->msd_buttonPowerShot = m_jsOperator1.GetRawButton(BUTTON_SHOOTER_POWER_SHOT);
	m_globalData->msd_buttonLayUp = m_jsOperator1.GetRawButton(BUTTON_SHOOTER_LAY_UP);
	m_globalData->mcd_buttonIntakeCollect = m_jsOperator1.GetRawButton(SWITCH_INTAKE_COLLECT);
	m_globalData->mcd_buttonIntakeReverse = m_jsOperator1.GetRawButton(SWITCH_INTAKE_REVERSE);
	m_globalData->mcd_buttonFeederFeeding = m_jsOperator1.GetRawButton(BUTTON_FEEDER_FEEDING);
	m_globalData->mcd_buttonFeederReverse = m_jsOperator1.GetRawButton(BUTTON_FEEDER_REVERSE);
	m_globalData->msd_buttonHoopOverrideTop = m_jsOperator1.GetRawButton(BUTTON_HOOP_OVERRIDE_TOP);
	m_globalData->msd_buttonHoopOverrideBottom = m_jsOperator1.GetRawButton(BUTTON_HOOP_OVERRIDE_BOTTOM);
	m_globalData->msd_impulseLockDistanceForShooter =
			m_lockDistanceForShooterImpulse.apply(m_jsOperator1.GetRawButton(BUTTON_LOCK_DISTANCE_FOR_SHOOTER));

	m_globalData->msd_potentiometerReading = (1.0 - m_jsOperator1.GetAxis(POTENTIOMETER_AXIS_TYPE)) / 0.965;
	if (m_globalData->msd_potentiometerReading > 1.0) {
		m_globalData->msd_potentiometerReading = 1.0;
	} else if (m_globalData->msd_potentiometerReading < 0.0) {
		m_globalData->msd_potentiometerReading = 0.0;
	}

	// scale the potentiometer reading
	if (m_globalData->msd_potentiometerReading < POTENTIOMETER_DEAD_ZONE) {
		// dead zone is 0.0 to 0.1
		m_globalData->msd_potentiometerReading = 0.0;
	} else {
		// 0.1 to 1.0 becomes 0.7 to 1.0
		m_globalData->msd_potentiometerReading = (m_globalData->msd_potentiometerReading - POTENTIOMETER_DEAD_ZONE) /
				(1 - POTENTIOMETER_DEAD_ZONE) * (1 - POTENTIOMETER_MIN_VAL) + POTENTIOMETER_MIN_VAL;
	}
#endif



#if TEST_DRIVE_ANGLE_MIN_SPEEDS
	m_globalData->m_incMinKinetic = m_jsDriver2.GetRawButton(3);
	m_globalData->m_decMinKinetic = m_jsDriver2.GetRawButton(2);
	m_globalData->m_incMinStatic = m_jsDriver2.GetRawButton(5);
	m_globalData->m_decMinStatic = m_jsDriver2.GetRawButton(4);
#endif

#if TEST_DRIVE_ANGLE_PID
	m_globalData->mdd_buttonIncP = m_jsDriver2.GetRawButton(6);
	m_globalData->mdd_buttonDecP = m_jsDriver2.GetRawButton(7);
	m_globalData->mdd_buttonIncD = m_jsDriver2.GetRawButton(11);
	m_globalData->mdd_buttonDecD = m_jsDriver2.GetRawButton(10);
	m_globalData->mdd_resetPIDButton = m_jsDriver2.GetRawButton(1);
#endif

#if TEST_DRIVE_ANGLE_ANGLES
	if (m_driveAngleImpulse[0].apply(m_jsDriver1.GetRawButton(6))) {
		m_globalData->mdd_startDriveAngle = true;
		m_globalData->mdd_desiredAngle = 0;
	} else if (m_driveAngleImpulse[1].apply(m_jsDriver1.GetRawButton(7))) {
		m_globalData->mdd_startDriveAngle = true;
		m_globalData->mdd_desiredAngle = 1;
	} else if (m_driveAngleImpulse[2].apply(m_jsDriver1.GetRawButton(8))) {
		m_globalData->mdd_startDriveAngle = true;
		m_globalData->mdd_desiredAngle = 5;
	} else if (m_driveAngleImpulse[3].apply(m_jsDriver1.GetRawButton(9))) {
		m_globalData->mdd_startDriveAngle = true;
		m_globalData->mdd_desiredAngle = 7;
	} else if (m_driveAngleImpulse[4].apply(m_jsDriver1.GetRawButton(10))) {
		m_globalData->mdd_startDriveAngle = true;
		m_globalData->mdd_desiredAngle = 10;
	} else if (m_driveAngleImpulse[5].apply(m_jsDriver1.GetRawButton(11))) {
		m_globalData->mdd_startDriveAngle = true;
		m_globalData->mdd_desiredAngle = m_globalData->mdd_gyroAngle + 180;
	} else if (m_driveAngleImpulse[6].apply(m_jsDriver1.GetRawButton(1))) {
		m_globalData->mdd_stopDriveAngle = true;
	}
#endif

#if TEST_DRIVE_ANGLE_GYRO_DEADBAND
	if (m_jsDriver2.GetRawButton(8)) {
		m_driveGyro.incDeadband(-10);
	} else if (m_jsDriver2.GetRawButton(9)) {
		m_driveGyro.incDeadband(10);
	}
#endif

#if DISPLAY_DRIVE_ANGLE_DEBUG
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "PD: %f/%f", m_globalData.mdd_gyroP, m_globalData.mdd_gyroD);
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line2, "gy(a/v): %5.4f/%5.4f", m_globalData->mdd_gyroAngle, m_globalData->mdd_gyroBiasVoltage);
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "deadband: %d", m_driveGyro.getDeadband());
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line4, "min(s/k): %4.3f/%4.3f", m_globalData.m_minSpeedStatic, m_globalData.m_minSpeedKinetic);
#endif

#if DISPLAY_BUTTON_BOARD_POTENTIOMETER
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line4, "pot: %f", m_globalData->msd_potentiometerReading);
#endif

#if TEST_SHOOTER_PID
	if (m_jsDriver2.GetRawButton(6)) {
		m_globalData->msd_p += 0.000001;
	} else if (m_jsDriver2.GetRawButton(7)) {
		m_globalData->msd_p -= 0.000001;
	}

	if (m_jsDriver2.GetRawButton(8)) {
		m_globalData->msd_i += 0.000003;
	} else if (m_jsDriver2.GetRawButton(9)) {
		m_globalData->msd_i -= 0.000003;
	}

	if (m_jsDriver2.GetRawButton(10)) {
		m_globalData->msd_d += 0.000001;
	} else if (m_jsDriver2.GetRawButton(11)) {
		m_globalData->msd_d -= 0.000001;
	}

	if (m_jsDriver2.GetRawButton(2)) {
		m_globalData->msd_p = 0.0;
		m_globalData->msd_i = 0.0001;
		m_globalData->msd_d = 0.0;
	}
#endif

#if TEST_SHOOTER_POWERS
	if (m_jsDriver1.GetRawButton(6)) {
		m_globalData->msd_potentiometerReading = 0.0;
	} else if (m_jsDriver1.GetRawButton(7)) {
		m_globalData->msd_potentiometerReading = 0.1;
	} else if (m_jsDriver1.GetRawButton(8)) {
		m_globalData->msd_potentiometerReading = 0.2;
	} else if (m_jsDriver1.GetRawButton(9)) {
		m_globalData->msd_potentiometerReading = 0.3;
	} else if (m_jsDriver1.GetRawButton(10)) {
		m_globalData->msd_potentiometerReading = 0.4;
	} else if (m_jsDriver1.GetRawButton(11)) {
		m_globalData->msd_potentiometerReading = 0.5;
	} else if (m_jsDriver2.GetRawButton(6)) {
		m_globalData->msd_potentiometerReading = 0.6;
	} else if (m_jsDriver2.GetRawButton(7)) {
		m_globalData->msd_potentiometerReading = 0.7;
	} else if (m_jsDriver2.GetRawButton(8)) {
		m_globalData->msd_potentiometerReading = 0.8;
	} else if (m_jsDriver2.GetRawButton(9)) {
		m_globalData->msd_potentiometerReading = 0.9;
	} else if (m_jsDriver2.GetRawButton(10)) {
		m_globalData->msd_potentiometerReading = 0.95;
	} else if (m_jsDriver2.GetRawButton(11)) {
		m_globalData->msd_potentiometerReading = 1.0;
	}
#endif

#if TEST_SHOOTER_DISTANCES
	if (m_jsDriver1.GetRawButton(1)) {
		m_globalData->msd_distanceToHoopFromCamera = 0;
	} else if (m_jsDriver1.GetRawButton(6)) {
		m_globalData->msd_distanceToHoopFromCamera = 140;
	} else if (m_jsDriver1.GetRawButton(7)) {
		m_globalData->msd_distanceToHoopFromCamera = 160;
	} else if (m_jsDriver1.GetRawButton(8)) {
		m_globalData->msd_distanceToHoopFromCamera = 180;
	} else if (m_jsDriver1.GetRawButton(9)) {
		m_globalData->msd_distanceToHoopFromCamera = 200;
	} else if (m_jsDriver1.GetRawButton(10)) {
		m_globalData->msd_distanceToHoopFromCamera = 220;
	} else if (m_jsDriver1.GetRawButton(11)) {
		m_globalData->msd_distanceToHoopFromCamera = 240;
	}
#endif

#if TEST_SHOOTER_MIN_MAX_RATES
	static double maxSoFar = -50000;
	static double minSoFar = 50000;

	if (m_jsDriver2.GetRawButton(1)) {
		maxSoFar = -50000;
		minSoFar = 50000;
	}

	if (m_globalData->msd_encoderRate > maxSoFar) {
		maxSoFar = m_globalData->msd_encoderRate;
	} else if (m_globalData->msd_encoderRate < minSoFar) {
		minSoFar = m_globalData->msd_encoderRate;
	}
#endif

#if TEST_SHOOTER_BATTERY_VOLTAGE
	m_globalData->m_initBatteryVoltage = -m_jsDriver2.GetAxis(Joystick::kYAxis) + 12.0;
#endif

#if DISPLAY_SHOOTER_DEBUG
#if TEST_SHOOTER_PID
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "PID: %f/%f/%f", m_globalData->msd_p, m_globalData->msd_i, m_globalData->msd_d);
	printf("PID:%lf/%lf/%lf ", m_globalData->msd_p, m_globalData->msd_i, m_globalData->msd_d);
#endif
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line2, "des rate: %f", m_globalData->msd_desiredRate);
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "act rate: %f", m_globalData->msd_encoderRate);
	printf("dr:%4.3f ar:%4.3f ", m_globalData->msd_desiredRate, m_globalData->msd_encoderRate);
#if TEST_SHOOTER_MIN_MAX_RATES
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line4, "min: %f", minSoFar);
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line5, "max: %f", maxSoFar);
#endif
	printf("p:%4.3f ", m_globalData->msd_shooterPower);
	printf("\n");
#endif

#if DISPLAY_BRIDGE_LOWERING_BUTTON_DEBUG
	printf("arm buttons: %d%d%d%d%d\n", m_globalData->mbd_buttonStop ? 1 : 0,
			m_globalData->mbd_buttonFrontUp ? 1 : 0,
					m_globalData->mbd_buttonFrontDown ? 1 : 0,
							m_globalData->mbd_buttonBackUp ? 1 : 0,
									m_globalData->mbd_buttonBackDown ? 1 : 0);
#endif

}
