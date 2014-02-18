#include "bhs_AutonomousBasicWithBridge.h"

const UINT32 bhs_AutonomousBasicWithBridge::MOVE_TO_BRIDGE_DIGITAL_INPUT = 8;
const UINT32 bhs_AutonomousBasicWithBridge::DELAY_ANALOG_INPUT = 1;
const UINT32 bhs_AutonomousBasicWithBridge::LESS_DISTANCE_ANALOG_INPUT = 4;

const float bhs_AutonomousBasicWithBridge::SHOOTER_POWER = 0.8;
const float bhs_AutonomousBasicWithBridge::DRIVE_TRAIN_POWER = 0.5;

// we should only hold two balls, but autonomous will shoot 3 times in case not all of them shoot
const int bhs_AutonomousBasicWithBridge::NUM_BALLS_TO_SHOOT = 3;

const double bhs_AutonomousBasicWithBridge::MIN_DELAY_DURATION = 4.0;
const double bhs_AutonomousBasicWithBridge::PULSE_SHOOT_DURATION = 0.25;
const double bhs_AutonomousBasicWithBridge::PULSE_STOP_DURATION = 1.75;
const double bhs_AutonomousBasicWithBridge::MAX_MOVE_TO_BRIDGE_DURATION = 4.0; // stop just in case the robot is getting stuck

const double bhs_AutonomousBasicWithBridge::DT_ENCODER_TICKS_PER_INCH = 38.44;
const double bhs_AutonomousBasicWithBridge::MOVE_TO_BRIDGE_DISTANCE_IN_INCHES = 112.0;
const double bhs_AutonomousBasicWithBridge::LESS_DISTANCE_PER_ANALOG_TICK_IN_INCHES = 6.0;

const float bhs_AutonomousBasicWithBridge::DES_ENC_RATE_HARD_BALL = 10500.0;
const float bhs_AutonomousBasicWithBridge::DES_ENC_RATE_SOFT_BALL = 11500.0;
const float bhs_AutonomousBasicWithBridge::DES_ENC_RATE_HARD_BALL_SIDE = 11600.0 + 500;
const float bhs_AutonomousBasicWithBridge::DES_ENC_RATE_SOFT_BALL_SIDE = 12900.0 + 600;

bhs_AutonomousBasicWithBridge::bhs_AutonomousBasicWithBridge(bhs_GlobalData* a_globalData)
: m_globalData(a_globalData)
, m_state(k_startShooter)
, m_timer()
, m_numBallsShot(0)
, m_initEncoder(0)
, m_moveToBridgeSet(false)
, m_lessEncoderCounts(0)
, m_encoderTarget(0)
, m_shootingFromSide(false)
{
	m_softBalls = new bool[NUM_BALLS_TO_SHOOT];
	for (int i=0; i<NUM_BALLS_TO_SHOOT; i++) {
		m_softBalls[i] = false;
	}
}

bhs_AutonomousBasicWithBridge::~bhs_AutonomousBasicWithBridge() {
	m_globalData = 0;
	delete[] m_softBalls;
}

void bhs_AutonomousBasicWithBridge::init() {
	m_state = k_startShooter;
}

void bhs_AutonomousBasicWithBridge::run() {
	switch (m_state) {
	case k_startShooter:
#if DEBUG_AUTONOMOUS_STATES
		DriverStationLCD::GetInstance()->Clear();
		DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "start shooter");
#endif
		m_numBallsShot = 0;

		m_initEncoder = m_globalData->mdd_encoderCounts;
		m_globalData->m_autonomousDelay = DriverStation::GetInstance()->GetAnalogIn(DELAY_ANALOG_INPUT);
		m_moveToBridgeSet = DriverStation::GetInstance()->GetDigitalIn(MOVE_TO_BRIDGE_DIGITAL_INPUT);
		m_lessEncoderCounts = (int) (DriverStation::GetInstance()->GetAnalogIn(LESS_DISTANCE_ANALOG_INPUT)) *
				LESS_DISTANCE_PER_ANALOG_TICK_IN_INCHES * DT_ENCODER_TICKS_PER_INCH;
		m_encoderTarget = (int) (MOVE_TO_BRIDGE_DISTANCE_IN_INCHES * DT_ENCODER_TICKS_PER_INCH - m_lessEncoderCounts);
		if (NUM_BALLS_TO_SHOOT >= 3) {
			if (DriverStation::GetInstance()->GetDigitalIn(4)) {
				m_softBalls[0] = false;
				m_softBalls[1] = false;
				m_softBalls[2] = false;
			} else if (DriverStation::GetInstance()->GetDigitalIn(5)) {
				m_softBalls[0] = true;
				m_softBalls[1] = false;
				m_softBalls[2] = false;
			} else if (DriverStation::GetInstance()->GetDigitalIn(6)) {
				m_softBalls[0] = true;
				m_softBalls[1] = true;
				m_softBalls[2] = true;
			}
		}
		m_shootingFromSide = DriverStation::GetInstance()->GetDigitalIn(3);

		m_globalData->mdd_joystick1X = 0.0;
		m_globalData->mdd_joystick1Y = 0.0;
		m_globalData->mdd_joystick2X = 0.0;
		m_globalData->mdd_joystick2Y = 0.0;
		m_globalData->msd_switchSpeedOverride = true;
		if (NUM_BALLS_TO_SHOOT >= 1) {
			// msd_shooterTarget is equal to msd_potentiometerReading
			if (m_shootingFromSide) {
				if (m_softBalls[0]) {
					m_globalData->msd_potentiometerReading = DES_ENC_RATE_SOFT_BALL_SIDE / 15200;
				} else {
					m_globalData->msd_potentiometerReading = DES_ENC_RATE_HARD_BALL_SIDE / 15200;
				}
			} else {
				if (m_softBalls[0]) {
					m_globalData->msd_potentiometerReading = DES_ENC_RATE_SOFT_BALL / 15200;
				} else {
					m_globalData->msd_potentiometerReading = DES_ENC_RATE_HARD_BALL / 15200;
				}
			}
		}
		m_state = k_waitForShooter;
		m_timer.Reset();
		m_timer.Start();
#if DEBUG_AUTONOMOUS_SHOOTER
		DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line2, "start: %f", m_timer.Get());
#endif
		break;

	case k_waitForShooter:
#if DEBUG_AUTONOMOUS_STATES
		DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "wait for shooter");
#endif
		if (m_timer.Get() >= MIN_DELAY_DURATION + m_globalData->m_autonomousDelay) {
#if DEBUG_AUTONOMOUS_SHOOTER
			DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "end: %f", m_timer.Get());
#endif
			m_state = k_moveConveyor;
			m_timer.Reset();
			m_timer.Start();
		}

		break;

	case k_moveConveyor:
#if DEBUG_AUTONOMOUS_STATES
		DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "move conveyor");
#endif
		m_globalData->mcd_buttonFeederFeeding = true;
		m_globalData->mcd_buttonIntakeCollect = true;
		if (m_timer.Get() >= PULSE_SHOOT_DURATION) {
			m_numBallsShot++;
			if (m_numBallsShot >= NUM_BALLS_TO_SHOOT) {
				if (m_moveToBridgeSet) {
					m_state = k_moveToBridge;
					m_timer.Reset();
					m_timer.Start();
				} else {
					m_state = k_halt;
					m_timer.Stop();
				}
			} else {
				m_state = k_stopConveyor;
				m_timer.Reset();
				m_timer.Start();
			}
		}
		break;

	case k_stopConveyor:
#if DEBUG_AUTONOMOUS_STATES
		DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "stop conveyor");
#endif
		m_globalData->mcd_buttonFeederFeeding = false;
		m_globalData->mcd_buttonIntakeCollect = false;

		if (m_shootingFromSide) {
			if (m_softBalls[m_numBallsShot]) {
				m_globalData->msd_potentiometerReading = DES_ENC_RATE_SOFT_BALL_SIDE / 15200;
			} else {
				m_globalData->msd_potentiometerReading = DES_ENC_RATE_HARD_BALL_SIDE / 15200;
			}
		} else {
			if (m_softBalls[m_numBallsShot]) {
				m_globalData->msd_potentiometerReading = DES_ENC_RATE_SOFT_BALL / 15200;
			} else {
				m_globalData->msd_potentiometerReading = DES_ENC_RATE_HARD_BALL / 15200;
			}
		}

		if (m_timer.Get() >= PULSE_STOP_DURATION) {
			m_state = k_moveConveyor;
			m_timer.Reset();
			m_timer.Start();
		}
		break;

	case k_moveToBridge:
#if DEBUG_AUTONOMOUS_STATES
		DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "move to bridge");
#endif
		m_globalData->mcd_buttonFeederFeeding = false;
		m_globalData->mcd_buttonIntakeCollect = false;
		if (abs(m_globalData->mdd_encoderCounts - m_initEncoder) > m_encoderTarget ||
				m_timer.Get() >= MAX_MOVE_TO_BRIDGE_DURATION) {
			m_state = k_lowerArm;
			m_timer.Stop();
		} else {
			m_globalData->mdd_joystick1Y = -DRIVE_TRAIN_POWER;
			m_globalData->mdd_joystick2Y = -DRIVE_TRAIN_POWER;
		}
		break;

	case k_lowerArm:
#if DEBUG_AUTONOMOUS_STATES
		DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "lowering the arm");
#endif
		m_globalData->mdd_joystick1Y = 0.0;
		m_globalData->mdd_joystick2Y = 0.0;
		if (m_globalData->mbd_backDownLimitSwitch) {
			m_state = k_halt;
		} else {
			//			m_globalData->mbd_buttonBackDown = true;
		}
		break;

	case k_halt:
#if DEBUG_AUTONOMOUS_STATES
		DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "halt");
#endif
		m_globalData->mdd_joystick1X = 0;
		m_globalData->mdd_joystick1Y = 0;
		m_globalData->mdd_joystick2X = 0;
		m_globalData->mdd_joystick2Y = 0;
		m_globalData->msd_potentiometerReading = 0.0;
		m_globalData->mcd_buttonFeederFeeding = false;
		m_globalData->mcd_buttonIntakeCollect = false;
		//		m_globalData->mbd_buttonBackDown = false;
		break;
	}
}
