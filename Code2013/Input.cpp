#include "vxWorks.h"
#include "semLib.h"
#include "sched.h"

#include "Input.h"
#include "Processing.h"
#include "Constants.h"

GlobalData* Input::m_globals = GlobalData::GetInstance();

namespace {
bool prevFire = false;
}

// We map the battery's voltage to a speed factor linearly. The baseline
//  voltage is the mean of the minimum and maximum voltages. The baseline
//  factor is the mean of the minimum and maximum factors. `BufferedSpeed`
//  doesn't allow the motor's speed to exceed some maximum value. This means
//  that there is a gap where the motor just maxes out:
//
// s|  ----\       |
// p|       \      |
// e|        \     |
// e|         \    |
// d|          \   |
//      voltage
// (assuming a constant inital speed)
namespace {
// Taken from the Processing project.
// Maps a value from one coordinate plane to another.
double planeMap(double value, double istart, double istop, double ostart, double ostop) {
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}
}

void Input :: GetInputs ()
{	
	semTake(m_globals->semaphore, WAIT_FOREVER);

	GetDriverControlInputs();
	GetSensorInputs();

	semGive(m_globals->semaphore);
	sched_yield();
}

void Input :: GetDriverControlInputs ()
{
	// Driver Station
	m_globals->i_dsEncoderDistanceOffset = m_globals->i_ds->GetAnalogIn(AUTONOMOUS_ENCODER_DISTANCE) * ENCODER_DIST_MULTIPLIER + ENCODER_DIST_CONST; // Range: -1000 to 1000
	m_globals->i_dsAutoKickerMinTime = m_globals->i_ds->GetAnalogIn(AUTONOMOUS_KICKER_MIN_TIME) / KICKER_MIN_TIME_DIVISOR; // Range: 0 to 1 second
	m_globals->i_dsKickerMinTime = m_globals->i_ds->GetAnalogIn(KICKER_MIN_TIME) / KICKER_MIN_TIME_DIVISOR; // Range: 0 to 1 second
	m_globals->i_dsTiltOffset = m_globals->i_ds->GetAnalogIn(TILT_OFFSET) * TILT_OFFSET_MULTIPLIER + TILT_OFFSET_CONST; // Range: -5 to 5 degrees
	 
#define DS_D(x) m_globals->i_ds->GetDigitalIn(x)
	
	// Changed this to be actual wait times so hertz is
	// easier to calculate
	if (!DS_D(3) && !DS_D(4) && !DS_D(5))
		m_globals->i_dsDataTransmitRate = 0.33;  //3 hr
	else if (!DS_D(3) && !DS_D(4) && DS_D(5))
		m_globals->i_dsDataTransmitRate = 0.25; // 4hz
	else if (!DS_D(3) && DS_D(4) && !DS_D(5))
		m_globals->i_dsDataTransmitRate = 0.20; // 5hz
	else if (!DS_D(3) && DS_D(4) && DS_D(5))
		m_globals->i_dsDataTransmitRate = 0.15;  //6.6 hz
	else if (DS_D(3) && !DS_D(4) && !DS_D(5))
		m_globals->i_dsDataTransmitRate = 0.10; //10 hz
	else if (DS_D(3) && !DS_D(4) && DS_D(5))
		m_globals->i_dsDataTransmitRate = 0.05; // 20 hz
	else if (DS_D(3) && DS_D(4) && !DS_D(5))
		m_globals->i_dsDataTransmitRate = 0.02; // 50 hz
	else
		m_globals->i_dsDataTransmitRate = 0.9;  // 1hz (well 0.9 hz)
	
	
	
	if (!DS_D(6) && !DS_D(7) && !DS_D(8))
		m_globals->i_dsDefaultTiltOffset = 0;
	else if (!DS_D(6) && !DS_D(7) && DS_D(8))
		m_globals->i_dsDefaultTiltOffset = -1;
	else if (!DS_D(6) && DS_D(7) && !DS_D(8))
		m_globals->i_dsDefaultTiltOffset = -0.75;
	else if (!DS_D(6) && DS_D(7) && DS_D(8))
		m_globals->i_dsDefaultTiltOffset = -0.5;
	else if (DS_D(6) && !DS_D(7) && !DS_D(8))
		m_globals->i_dsDefaultTiltOffset = -0.25;
	else if (DS_D(6) && !DS_D(7) && DS_D(8))
		m_globals->i_dsDefaultTiltOffset = 0.25;
	else if (DS_D(6) && DS_D(7) && !DS_D(8))
		m_globals->i_dsDefaultTiltOffset = 0.5;
	else
		m_globals->i_dsDefaultTiltOffset = 0.75;
	
	// Drive train
	m_globals->d_joystick_1_x = m_globals->g_joystickDriver1->GetX( GenericHID::kLeftHand );
	m_globals->d_joystick_1_y = m_globals->g_joystickDriver1->GetY( GenericHID::kLeftHand );
	m_globals->d_joystick_2_x = m_globals->g_joystickDriver2->GetX( GenericHID::kRightHand);
	m_globals->d_joystick_2_y = m_globals->g_joystickDriver2->GetY( GenericHID::kRightHand);
	m_globals->d_joystickArcadeToggle = m_globals->g_joystickDriver1->GetRawButton( JOYSTICK_ARCADE_TOGGLE );

	m_globals->d_gyroReset = m_globals->g_joystickDriver2->GetRawButton( GYRO_RESET );

	m_globals->d_turn = 0;
	if ( m_globals->g_joystickDriver2->GetRawButton( TURN_SWITCH ) )	//FIXME: turn off at competition?
		m_globals->d_turnSwitch = GlobalData::TURNING;
	else
		m_globals->d_turnSwitch = GlobalData::NOT_TURNING;


	// Finger
	if(m_globals->g_joystickDriver2->GetRawButton( FINGER_UP ))
		m_globals->d_fingerUp = true;
	else if(m_globals->g_joystickDriver2->GetRawButton( FINGER_DOWN ))
		m_globals->d_fingerUp = false;

	// Spatula
	m_globals->d_spatulaUp = m_globals->g_joystickDriver1->GetRawButton( SPATULA_UP );
	m_globals->d_spatulaDown = m_globals->g_joystickDriver1->GetRawButton( SPATULA_DOWN );

	//Hang
	m_globals->d_hangSafetySwitch = !m_globals->g_joystickOperator1->GetRawButton( HANG_SAFETY_SWITCH );
	m_globals->d_hangExtended = m_globals->g_joystickDriver2->GetRawButton( HANG_EXTENDED );
	m_globals->d_hangRetracted = m_globals->g_joystickDriver2->GetRawButton( HANG_RETRACTED );
	m_globals->d_hangRetracted = m_globals->g_joystickDriver2->GetRawButton( HANG_RETRACTED );
	// Hopper
	m_globals->d_hopperSwitch = !m_globals->g_joystickOperator1->GetRawButton( HOPPER_SWITCH );

	// Launcher
	m_globals->d_launcherManualSpeed = m_globals->g_joystickOperator1->GetX();
	m_globals->d_launcherManualTilt = m_globals->g_joystickOperator1->GetY();
	m_globals->d_launcherPreset3 = m_globals->g_joystickDriver1->GetTrigger();
	// LAUNCHER_PRESET_SWITCH_2 corresponds to switch up on button board which will be preset values 1( LAUNCHER_
	// LAUNCHER_PRESET_SWITCH_6 switch corresponds to switch down on button board.
	m_globals->d_launcherPresetSwitch = m_globals->g_joystickOperator1->GetRawButton( LAUNCHER_PRESET_SWITCH_2 )
										- m_globals->g_joystickOperator1->GetRawButton( LAUNCHER_PRESET_SWITCH_6 ) + 1;
	// Kicker
	m_globals->d_manualKick = m_globals->g_joystickOperator1->GetRawButton( MANUAL_KICK );
	m_globals->d_autoEnabled = m_globals->g_joystickOperator1->GetRawButton( AUTO_ENABLED );
	m_globals->d_launcherFireSequence = m_globals->g_joystickOperator1->GetRawButton( LAUNCHER_FIRE_SEQUENCE );
	if ( !prevFire && m_globals->d_launcherFireSequence )
		m_globals->g_gyro->Reset();
	prevFire = m_globals->d_launcherFireSequence;

	//FIXME: remove after Processing.cpp is debugged and bhs_SSD obsolete
	//Processing::processLauncherPosition();

	// Other Stuff
	m_globals->d_batteryVoltage = m_globals->i_ds->GetBatteryVoltage();

#if 0
	printf("D_DT: %f, %f, %f, %f\n", m_globals->d_joystick_1_x, m_globals->d_joystick_1_y, m_globals->d_joystick_2_x, m_globals->d_joystick_2_y );
	printf("D_SPATULA: %s/%s\n", m_globals->d_spatulaDown ? "DOWN" : "...", m_globals->d_spatulaUp ? "UP" : "...");
	printf("D_HANG SAFETY: %s\n", m_globals->d_hangSafetySwitch ? "YES" : "NO");
	printf("D_AUTO ENABLED: %s\n", m_globals->d_autoEnabled ? "YES" : "NO");
	printf("D_PRESET VALUE: %d\n", m_globals->d_launcherPresetSwitch);
	printf("D_MANUAL KICK: %s\n", m_globals->d_manualKick ? "YES" : "NO");
	printf("D_HOPPER SWITCH: %d\n", m_globals->d_hopperSwitch);
	printf("D_FIRE SEQUENCE: %s\n", m_globals->d_launcherFireSequence ? "YES" : "NO");
#endif
}

void Input :: GetSensorInputs ()
{
	// Drive train
	m_globals->i_encoderLeft = m_globals->g_encoderLeft->Get();
	m_globals->i_encoderRight = m_globals->g_encoderRight->Get();
	m_globals->i_gyroAngle = m_globals->g_gyro->GetAngle();

	// Hopper
	m_globals->i_hopperLimitSwitchDown = m_globals->g_hopperLimitSwitchDown->Get();
	m_globals->i_hopperLimitSwitchUp = m_globals->g_hopperLimitSwitchUp->Get();

	// Launcher
	m_globals->i_launcherTiltPotentiometer = m_globals->g_tiltPot->GetAverageVoltage();
	//printf( "launcherpot: %f\n", m_globals->i_launcherTiltPotentiometer );

	// Kicker
	m_globals->i_launcherKickerState = m_globals->g_launcherKicker->Get();

	// Other stuff
	m_globals->i_comp= m_globals->g_comp->GetPressureSwitchValue();
	m_globals->i_comp_isEnabled= m_globals->g_comp->Enabled();

	m_globals->i_powerFactor = planeMap(m_globals->d_batteryVoltage, m_globals->c_batteryVoltageMin,
			m_globals->c_batteryVoltageMax, m_globals->c_launcherBatteryFactorMax,
			m_globals->c_launcherBatteryFactorMin);
}
