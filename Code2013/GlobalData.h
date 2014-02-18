#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include "WPILib.h"
#include "DriverStation.h"
#include "Constants.h"

#include "semLib.h"

#include "bhs_Hang.h"
#include "bhs_Hopper.h"
#include "bhs_Launcher.h"
#include "bhs_Spatula.h"
#include "SpeedBuffer.h"

class bhs_Hang;
class bhs_Hopper;
class bhs_Spatula;
class bhs_Launcher;

// FRC Driver Station Analog Channels
#define AUTONOMOUS_ENCODER_DISTANCE 1
#define KICKER_MIN_TIME 3
#define TILT_OFFSET 4
#define AUTONOMOUS_KICKER_MIN_TIME 2



// **********************************
// ELECTRONICS CHANNEL SPECIFICATIONS
	// Driver Control
#define JOYSTICK_CENTER 0
#define JOYSTICK_DRIVER_1_PORT 1
#define JOYSTICK_DRIVER_2_PORT 2
#define JOYSTICK_OPERATOR_1_PORT 3
#define JOYSTICK_ARCADE_TOGGLE 4
#define FINGER_UP 2
#define FINGER_DOWN 3
#define GYRO_RESET 8
#define SPATULA_UP 3
#define SPATULA_DOWN 2
#define HANG_EXTENDED 11
#define HANG_RETRACTED 10
#define TURN_SWITCH 9
#define HANG_SAFETY_SWITCH 1
#define HOPPER_SWITCH 8
#define LAUNCHER_FIRE_SEQUENCE 10
#define LAUNCHER_PRESET_SWITCH_2 2
#define LAUNCHER_PRESET_SWITCH_6 6
#define AUTO_ENABLED 5
#define MANUAL_KICK 7

//These logical values are based on computation of two button in input.cpp
// FIXME: I don't see any computation.
#define LAUNCHER_PRESET_1 	0 // 2
#define LAUNCHER_PRESET_2 	2 // 0
#define LAUNCHER_MANUAL   	1

	// Digital Inputs
#define LEFT_ENCODER_A_DI 1
#define LEFT_ENCODER_B_DI 2
#define RIGHT_ENCODER_A_DI 3
#define RIGHT_ENCODER_B_DI 4
#define HOPPER_LIMIT_SWITCH_DOWN 5
#define HOPPER_LIMIT_SWITCH_UP 6

	// Analog Inputs
#define GYRO_AI 1
#define TILT_POTENTIOMETER 3
#define LIGHT_SENSOR_AI 4

	// PWMs
#define DRIVETRAIN_L_1_PWM 1
#define DRIVETRAIN_L_2_PWM 2
#define DRIVETRAIN_R_1_PWM 3
#define DRIVETRAIN_R_2_PWM 4
#define LAUNCHER_TILT_PWM 7
#define LAUNCHER_SPEED_PWM 5
#define INTAKE_PWM 6

	// Solenoids
#define SPATULA_SOLENOID 5
#define HANG_A_SOLENOID 1
#define HANG_B_SOLENOID 2
#define HOPPER_SOLENOID_A 3
#define HOPPER_SOLENOID_B 4
#define FINGER_SOLENOID 6
#define LAUNCHER_KICKER_SOLENOID 7		// FIXME: Disc loader was 6 and kicker was 7.
										//		  7's wire appeared to be broken
	// Relays
#define CAM_RELAY 1
#define LIGHT_1_RELAY 2
#define LIGHT_2_RELAY 3
#define COMPRESSOR_SENSOR 7

class GlobalData
{
private:
	GlobalData();
	GlobalData(const GlobalData&); // Disabling copy ctor

	static GlobalData* staticGlobals;

	void init();

public:
	~GlobalData();
	static GlobalData* GetInstance();

	void resetOutputs();
	void resetDS();
	
	SpeedBuffer* m_speedBuffer;

	// *********
	// CONSTANTS
	double c_straightP;
	double c_straightI;
	double c_straightD;
	double c_turnP;
	double c_turnI;
	double c_turnD;
	double c_tiltP;
	double c_tiltI;
	double c_tiltD;
	double c_batteryVoltageMin;
	double c_batteryVoltageMax;
	double c_launcherBatteryFactorMin;
	double c_launcherBatteryFactorMax;

	bhs_Launcher* s_launcher;
	bhs_Spatula* s_spatula;
	bhs_Hang* s_hang;
	bhs_Hopper* s_hopper;

	// *********************
	// SENSORS AND ACTUATORS
		// Joysticks
	Joystick* g_joystickDriver1;
	Joystick* g_joystickDriver2;
	Joystick* g_joystickOperator1;
		// Sensors
	Encoder* g_encoderLeft;
	Encoder* g_encoderRight;
	Gyro* g_gyro;
	DigitalInput* g_hopperLimitSwitchDown;
	DigitalInput* g_hopperLimitSwitchUp;
	AnalogChannel* g_launcherEMF;
	AnalogChannel* g_tiltPot;
		// Drive train
	Talon* g_DTLeftA;
	Talon* g_DTLeftB;
	Talon* g_DTRightA;
	Talon* g_DTRightB;
		// Spatula
	Solenoid* g_spatula;
	Talon* g_intake;
		// Hang
	DoubleSolenoid* g_hang;
		// Hopper
	DoubleSolenoid* g_hopper;
		// Finger
	Solenoid* g_finger;
		// Launcher
	Talon* g_launcherFire;
	Talon* g_launcherTilt;
	Solenoid* g_launcherKicker;
		// Compressor
	Compressor* g_comp;
		// Light
	Relay* g_light1;
	Relay* g_light2;

	// ******
	// INPUTS
		// Driver Station
	double i_dsEncoderDistanceOffset;
	double i_dsAutoKickerMinTime;
	double i_dsKickerMinTime;
	double i_dsTiltOffset;
	
	double i_dsDataTransmitRate;
	double i_dsDefaultTiltOffset;
	
	
		// Sensors
	int i_encoderLeft;
	int i_encoderRight;
	double i_gyroAngle;
	bool i_hopperLimitSwitchUp;
	bool i_hopperLimitSwitchDown;
	double i_launcherSpeed;
	double i_launcherCurrentTilt;
	double i_launcherDesiredTilt;
	bool i_launcherKickerState;
	double i_launcherTiltPotentiometer;
	bool i_requestForVision;
	double i_launcherVisionTilt;
	double i_launcherVisionHorizontalAngle;
	double i_launcherVisionSpeed;
	double i_powerFactor;
	DriverStation* i_ds;
		// Driver Control
	double d_joystick_1_x;
	double d_joystick_1_y;
	double d_joystick_2_x;
	double d_joystick_2_y;
	bool d_joystickArcadeToggle;
	enum TurnStates
	{
		WANT_TO_TURN,
		TURNING,
		NOT_TURNING
	};
	TurnStates i_turnState;
	bool d_gyroReset;
	double d_turn;
	TurnStates d_turnSwitch;
	bool d_intake;
	bool d_spatulaUp;
	bool d_spatulaDown;
	bool d_hangExtended;
	bool d_hangRetracted;
	bool d_hangSafetySwitch;
	bool d_hopperSwitch;
	bool d_fingerUp;
	bool d_launcherFireSequence;
	int d_launcherPresetSwitch;
	double d_launcherManualSpeed;
	double d_launcherManualTilt;
	bool d_launcherPreset3;
	bool d_autoEnabled;
	bool d_manualKick;
	double d_batteryVoltage;

	unsigned int i_comp;
	bool i_comp_isEnabled;

	// *******
	// OUTPUTS
		// Drive train
	double o_drivetrainLeft;
	double o_drivetrainRight;
		// Spatula
	bool o_spatula;
	double o_intake;
		// Hang
	DoubleSolenoid::Value o_hang;
		// Hopper
	DoubleSolenoid::Value o_hopper;
		// Finger
	bool o_finger;
		// Launcher
	double o_launcherSpeed;
	double o_launcherTilt;
		// Kicker
	bool o_launcherKicker;
		// Light
	Relay::Value o_light1State;
	Relay::Value o_light2State;

	unsigned char o_matchState;
	SEM_ID semaphore;
};

#endif // GLOBALDATA_H
