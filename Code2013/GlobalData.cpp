#include "GlobalData.h"

GlobalData* GlobalData :: staticGlobals = NULL;

GlobalData :: GlobalData ()
{
	static bool hasInstantiated = false;

	if(hasInstantiated)
	{
		throw std::runtime_error("Double-instantiation attempted. U F.");
	}

	hasInstantiated = true;

	// *********
	// CONSTANTS
	c_straightP = 0.1;
	c_straightI = 0.0;
	c_straightD = 0.0000001;
	c_turnP = 0.3;
	c_turnI = 0.0;
	c_turnD = 0.000;
	c_tiltP = 0.25;
	c_tiltI = 0;
	c_tiltD = 0.000001;
	c_batteryVoltageMin = 12.0;
	c_batteryVoltageMax = 14.0;
	c_launcherBatteryFactorMin = 0.8;
	c_launcherBatteryFactorMax = 1.2;

	// ******
	// INPUTS
		// Driver Station
	i_dsEncoderDistanceOffset = 0.0;
	i_dsAutoKickerMinTime = 0.0;
	i_dsKickerMinTime = 0.0;
	i_dsTiltOffset = 0.0;
	i_dsDataTransmitRate = 0.33; //Default to 3 hz
	i_dsDefaultTiltOffset = 0;
	
		// Sensors
	i_encoderLeft = 0;
	i_encoderRight = 0;
	i_gyroAngle = 0.0;
	i_launcherSpeed = 0.0;
	i_launcherCurrentTilt = 0.0;
	i_launcherDesiredTilt = 0.0;
	i_launcherKickerState = false;
	i_hopperLimitSwitchUp = false;
	i_hopperLimitSwitchDown = false;
	i_launcherTiltPotentiometer = 0.0;
	i_requestForVision = false;
	i_launcherVisionTilt = 0.0;
	i_launcherVisionHorizontalAngle = 0.0;
	i_launcherVisionSpeed = 0.0;
	i_powerFactor = 0.0;
		// Driver Control
	d_joystick_1_x = 0.0;
	d_joystick_1_y = 0.0;
	d_joystick_2_x = 0.0;
	d_joystick_2_y = 0.0;
	d_joystickArcadeToggle = false;
	i_turnState = NOT_TURNING;
	d_gyroReset = false;
	d_turn = 0.0;
	d_turnSwitch = NOT_TURNING;
	d_spatulaUp = false;
	d_spatulaDown = false;
	d_hangExtended = false;
	d_hangRetracted = false;
	d_hangSafetySwitch = false;
	d_hopperSwitch = true;
	d_fingerUp = true;
	d_launcherFireSequence = false;
	d_launcherPresetSwitch = LAUNCHER_PRESET_1;
	d_launcherManualSpeed = 0.0;
	d_launcherManualTilt = 0.0;
	d_launcherPreset3 = false;
	d_autoEnabled = false;
	d_manualKick = false;
	d_batteryVoltage = 0.0;

	i_comp= 0;
	i_comp_isEnabled= false;
	
	// *******
	// OUTPUTS
		// Drive train
	o_drivetrainLeft = 0.0;
	o_drivetrainRight = 0.0;
		// Spatula
	o_spatula = false;
	o_intake = 0.0;
		// Hang
	o_hang = DoubleSolenoid::kReverse;
		// Hopper
	o_hopper = DoubleSolenoid::kForward;
		// Finger
	o_finger = false;
		// Launcher
	o_launcherKicker = false;
	o_launcherSpeed = 0.0;
	o_launcherTilt = 0.0;
		// Light
	o_light1State = Relay::kOff;
	o_light2State = Relay::kOff;

	o_matchState= 0;
	semaphore= semMCreate(SEM_Q_FIFO);
}

GlobalData :: ~GlobalData ()
{

}

void GlobalData :: init ()
{
	m_speedBuffer = new SpeedBuffer();

	// *********************
		// SENSORS AND ACTUATORS
		i_ds = DriverStation::GetInstance();
			// Joysticks
		g_joystickDriver1 = new Joystick( JOYSTICK_DRIVER_1_PORT );
		g_joystickDriver2 = new Joystick( JOYSTICK_DRIVER_2_PORT );
		g_joystickOperator1 = new Joystick( JOYSTICK_OPERATOR_1_PORT );
			// Sensors
		g_encoderLeft = new Encoder( LEFT_ENCODER_A_DI, LEFT_ENCODER_B_DI );
		g_encoderRight = new Encoder( RIGHT_ENCODER_A_DI, RIGHT_ENCODER_B_DI );
		g_encoderLeft->Start();
		g_encoderRight->Start();
		g_gyro = new Gyro( GYRO_AI );
		g_hopperLimitSwitchDown = new DigitalInput( HOPPER_LIMIT_SWITCH_DOWN );
		g_hopperLimitSwitchUp = new DigitalInput( HOPPER_LIMIT_SWITCH_UP );
		g_tiltPot = new AnalogChannel(TILT_POTENTIOMETER);
			// Drive train
		g_DTLeftA = new Talon( DRIVETRAIN_L_1_PWM );
		g_DTLeftB = new Talon( DRIVETRAIN_L_2_PWM );
		g_DTRightA = new Talon( DRIVETRAIN_R_1_PWM );
		g_DTRightB = new Talon( DRIVETRAIN_R_2_PWM );
			// Spatula
		g_spatula = new Solenoid( SPATULA_SOLENOID );
		s_spatula = new bhs_Spatula();
		g_intake = new Talon( INTAKE_PWM );
			// Hang
		g_hang = new DoubleSolenoid( HANG_A_SOLENOID, HANG_B_SOLENOID );
		s_hang = new bhs_Hang();
			// Hopper
		g_hopper = new DoubleSolenoid( HOPPER_SOLENOID_A, HOPPER_SOLENOID_B );
		s_hopper = new bhs_Hopper();
			// Finger
		g_finger = new Solenoid( FINGER_SOLENOID );
			// Launcher		

		g_launcherFire = new Talon( LAUNCHER_SPEED_PWM );
		//printf( "***1\n" );
		g_launcherTilt = new Talon( LAUNCHER_TILT_PWM );
		m_speedBuffer->SetMaxSpeed( g_launcherTilt, LAUNCHER_TILT_MAX_SPEED );
		s_launcher = new bhs_Launcher();
		g_launcherKicker = new Solenoid( LAUNCHER_KICKER_SOLENOID );
			//Compressor
	//FIXME switch over to const's/Defines
		g_comp = new Compressor( 7, 6 );
			// Light
		g_light1 = new Relay(LIGHT_1_RELAY);
		g_light2 = new Relay(LIGHT_2_RELAY);
}

GlobalData* GlobalData :: GetInstance ()
{
	if(staticGlobals == NULL)
	{
		staticGlobals = new GlobalData();
		staticGlobals->init();
	}

	return staticGlobals;
}

void GlobalData :: resetOutputs()
{
	// OUTPUTS
			// Drive train
		o_drivetrainLeft = 0.0;
		o_drivetrainRight = 0.0;
			// Spatula
		o_spatula = false;
		o_intake = 0.0;
			// Hang
		o_hang = DoubleSolenoid::kReverse;
			// Hopper
		o_hopper = DoubleSolenoid::kForward;
			// Finger
		o_finger = false;
			// Launcher
		o_launcherKicker = false;
		o_launcherSpeed = 0.0;
		o_launcherTilt = 0.0;
			// Light
		o_light1State = Relay::kOff;
		o_light2State = Relay::kOff;
}

void GlobalData :: resetDS()
{
	// Driver Control
		d_joystick_1_x = 0.0;
		d_joystick_1_y = 0.0;
		d_joystick_2_x = 0.0;
		d_joystick_2_y = 0.0;
		d_joystickArcadeToggle = false;
		i_turnState = NOT_TURNING; // FIXME: is this right?
		d_gyroReset = false;
		d_turn = 0.0;
		d_turnSwitch = NOT_TURNING;
		d_spatulaUp = false;
		d_spatulaDown = false;
		d_hangExtended = false;
		d_hangRetracted = false;
		d_hangSafetySwitch = false;
		d_hopperSwitch = true;
		d_launcherFireSequence = false;
		d_launcherPresetSwitch = LAUNCHER_PRESET_1;
		d_launcherManualSpeed = 0.0;
		d_launcherManualTilt = 0.0;
		d_autoEnabled = false;
		d_manualKick = false;
		d_batteryVoltage = 0.0;
}
