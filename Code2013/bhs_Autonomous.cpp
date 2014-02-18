#include "bhs_Autonomous.h"

bhs_Autonomous::bhs_Autonomous()
{
	m_state = k_spinUp;
	m_globals = GlobalData::GetInstance();
	m_timer = new Timer();
}
long bhs_Autonomous::inchToEncoder(int inches){
	return (long)(ENC_PER_INCH * inches);
}
double bhs_Autonomous::inch(int num)
{
	return((INCH_CONV) * num);
}

double bhs_Autonomous::foot(int num)
{
	return(inch(num * 12));
}
/*
void bhs_Autonomous::DriveStraightTest(){
	m_globals->d_joystickArcadeToggle = true;
	m_globals->d_joystick_2_y = AUTO_VELOCITY;
}*/
double desiredAngle = 0;
bool turnInit = false;
void bhs_Autonomous::EncoderTest(double encCount){
	/*m_globals->d_joystickArcadeToggle = true;
	m_globals->d_joystick_2_y = -0.25;
	if(fabs(m_globals->i_encoderLeft)>inchToEncoder(40))
		m_globals->d_joystick_2_y = 0;
	printf("Left Encoder: %d\n",m_globals->i_encoderLeft);	//18.625, 503
															//1078, 37.25
															 * 
															 */
	//Turn test
	
	
	if(!turnInit) {
		m_globals->g_gyro->Reset();
		Wait(0.5);
		m_globals->i_gyroAngle = m_globals->g_gyro->GetAngle();
		desiredAngle = encCount + m_globals->i_gyroAngle;
		printf("Init Gyro value: %f/%f\n", m_globals->i_gyroAngle,desiredAngle);
		turnInit=TRUE;
	}
	m_globals->d_turn = (desiredAngle - m_globals->i_gyroAngle);
	printf("Remaining value: %f/%f\n", m_globals->d_turn,desiredAngle);

	
	if (fabs(m_globals->d_turn) > 1.0)
	   m_globals->d_turnSwitch = GlobalData::WANT_TO_TURN;
	else
	   m_globals->d_turnSwitch = GlobalData::NOT_TURNING;


}
inline void bhs_Autonomous::fireDiscs(){
	//The DriverStation Manual input is being used for testing

	if(m_globals->i_dsAutoKickerMinTime != 0){
		m_globals->d_manualKick = ((int)(m_timer->Get()*(1/m_globals->i_dsAutoKickerMinTime)))%2 == 0 ;
		printf("d_manualKick: %d\n",m_globals->d_manualKick);
	}
}
inline void bhs_Autonomous::setDefaults(){
	m_globals->d_spatulaDown = false;
	m_globals->d_spatulaUp = true;	//Defaults, just in case one of the double solenoids is not set
	m_globals->d_hopperSwitch = false;
	m_globals->d_autoEnabled = false;
	m_globals->d_launcherPresetSwitch = LAUNCHER_MANUAL;
	//m_globals->d_launcherManualTilt = FIRST_SHOOTER_ANGLE;
	m_globals->d_launcherManualSpeed = INITIAL_FIRE_VELOCITY;
	m_globals->d_joystickArcadeToggle = true;
	m_globals->d_joystick_2_y = 0;
	m_globals->d_manualKick = false;
	m_globals->d_fingerUp = false;
	m_timer->Start();
}
void bhs_Autonomous::ThreeDiscReverse(){
	MiddleThreeDiscs();
	if(m_state == k_midreverse){
		if(fabs(m_globals->i_encoderLeft) < (inchToEncoder(REVERSE_DISTANCE_INCHES) + m_globals->i_dsEncoderDistanceOffset))
			m_globals->d_joystick_2_y = REVERSE_VELOCITY;
		else{
			m_globals->d_joystick_2_y = 0;
			m_state = k_midturnToCenter;
			m_globals->g_gyro->Reset();
		}
	}
}
void bhs_Autonomous::ThreeDiscReverse180(){
	ThreeDiscReverse();
	if(m_state == k_midturnToCenter){
		m_globals->d_turn = 180;
		m_globals->d_turnSwitch = GlobalData::WANT_TO_TURN;
	}
}
void bhs_Autonomous::MiddleThreeDiscs(){
	setDefaults();
	switch(m_state){
	//Get the shooter up to speed and the hopper in place
	case k_spinUp:
		m_globals->d_hopperSwitch = true;
		if(m_timer->Get()>3.0){//INITIAL_EMF - m_globals->i_launcherSpeed < LAUNCHER_THRESHOLD){
			m_state = k_fireThree;
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Fire the three starting frisbees
	case k_fireThree:
		m_globals->d_hopperSwitch = true;
		if(m_timer->Get() < FIRE_TIME)
			fireDiscs();
		else{
			m_state = k_midreverse;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;
	default:
		m_globals->d_launcherManualSpeed = 0;
		break;
	}
}
#if 0
void bhs_Autonomous::SideThreeDiscs(){
	setDefaults();
	switch(m_state){
	//Get the shooter up to speed and the hopper in place
	case k_spinUp:
		m_globals->d_hopperSwitch = true;
		if(m_timer->Get()>3.0){//INITIAL_EMF - m_globals->i_launcherSpeed < LAUNCHER_THRESHOLD){
			m_state = k_fireThree;
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Fire the three starting frisbees
	case k_fireThree:
		m_globals->d_hopperSwitch = true;
		if(m_timer->Get() < FIRE_TIME)
			fireDiscs();
		else{
			m_state = k_midreverse;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;
	default:
		printf("/nError in Autonomous: Code has entered unreachable state./n");
		break;
	}
}
void bhs_Autonomous::SevenAutonomous(){

	//#if MANIPUATORS
	printf("Entering SevenAutonomous\n");
	setDefaults();
	if(m_state > k_midreverse && m_state < k_prepareFour)
	{
		m_globals->d_launcherManualTilt = MIN_ANGLE;
		m_globals->d_launcherPresetSwitch = LAUNCHER_MANUAL;
		m_globals->d_launcherManualSpeed = (m_state == k_finalReverse)? FINAL_FIRE_VELOCITY : 0.0;
	}
	switch(m_state){
	//Get the shooter up to speed and the hopper in place
	case k_spinUp:
		m_globals->d_hopperSwitch = true;
		if(m_timer->Get()>3.0){//INITIAL_EMF - m_globals->i_launcherSpeed < LAUNCHER_THRESHOLD){
			printf("At speed\n");
			m_state = k_fireThree;
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Fire the three starting frisbees
	case k_fireThree:
		m_globals->d_hopperSwitch = true;
		if(m_timer->Get() < FIRE_TIME){
			fireDiscs();
			printf("Fire Timer: %f\n", m_timer->Get());
		}
		else{
			m_state = k_midreverse;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;

	case k_midreverse:
		m_globals->d_joystick_2_y = REVERSE_VELOCITY;
		printf("i_encoderLeft:%d\n",m_globals->i_encoderLeft);
		if(fabs(m_globals->i_encoderLeft) > REVERSE_DISTANCE){
			m_state = k_scoopTwo;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Lower the hopper and spatula while accelerating to scoop up the frisbees below the pyramid
	case k_scoopTwo:
		if(m_timer->Get() > LOWER_HOPPER_TIME)
		{
			m_globals->d_spatulaDown = true;
			m_globals->d_spatulaUp = false;
		}
		m_globals->d_launcherFireSequence = false;
		m_globals->d_hopperSwitch = false;
		m_globals->d_joystickArcadeToggle = true;
		m_globals->d_joystick_2_y = AUTO_VELOCITY;
		if(fabs(m_globals->i_encoderLeft) > inchToEncoder(PYRAMID_DISCS_DISTANCE))
		{
			m_state = k_loadTwo;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Decelerate and load the frisbees into the hopper
	case k_loadTwo:
		m_globals->d_hopperSwitch = false;
		if(m_timer->Get() < RAISE_INTAKE_TIME){
			m_globals->d_spatulaDown = false;
			m_globals->d_spatulaUp = true;
		}
		else if(m_timer->Get() < RAISE_INTAKE_TIME + LOWER_INTAKE_TIME){
			m_globals->d_spatulaDown = true;
			m_globals->d_spatulaUp = false;
		}
		else{
			m_state = k_scoopFour;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		m_globals->d_joystick_1_y = AUTO_SLOW_VELOCITY;
		break;

		//Accelerate to pick up the last two disks
	case k_scoopFour:
		m_globals->d_spatulaDown = true;
		m_globals->d_spatulaUp = false;
		m_globals->d_hopperSwitch = false;
		m_globals->d_joystickArcadeToggle = true;
		m_globals->d_joystick_2_y = AUTO_VELOCITY;
		if(fabs(m_globals->i_encoderLeft) > inchToEncoder(FAR_DISCS_DISTANCE))
		{
			m_state = k_loadFour;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Stop and load the final two disks
	case k_loadFour:
		m_globals->d_hopperSwitch = false;
		//m_globals->d_launcherManualTilt = 1.0;
		//m_globals->d_launcherPresetSwitch = LAUNCHER_MANUAL;
		//m_globals->d_launcherManualSpeed = 0.75;
		if(m_timer->Get() < RAISE_INTAKE_TIME){
			m_globals->d_spatulaDown = false;
			m_globals->d_spatulaUp = true;
		}
		else if(m_timer->Get() < RAISE_INTAKE_TIME + LOWER_INTAKE_TIME){
			m_globals->d_spatulaDown = true;
			m_globals->d_spatulaUp = false;
		}
		else{
			m_state = k_finalReverse;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		m_globals->d_joystick_1_y = 0;
		break;
	case k_finalReverse:
		m_globals->d_joystick_2_y = REVERSE_VELOCITY;
		if(m_globals->i_encoderLeft > -inchToEncoder(PYRAMID_DISCS_DISTANCE)){
			m_state = k_prepareFour;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;
		//Raise hopper and get shooter up to speed and angle
	case k_prepareFour: 
		m_globals->d_launcherManualTilt = FINAL_SHOOTER_ANGLE;
		m_globals->d_launcherPresetSwitch = LAUNCHER_MANUAL;
		m_globals->d_launcherManualSpeed = FINAL_FIRE_VELOCITY;
		m_globals->d_hopperSwitch = true;
		//	m_globals->i_launcherDesiredTilt = FINAL_SHOOTER_ANGLE;
		if(m_timer->Get()>2.0){//FINAL_EMF - m_globals->i_launcherSpeed < LAUNCHER_THRESHOLD){
			m_state = k_fireFour;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Fire the final four disks
	case k_fireFour:
		m_globals->d_launcherManualTilt = FINAL_SHOOTER_ANGLE;
		m_globals->d_launcherPresetSwitch = LAUNCHER_MANUAL;
		m_globals->d_launcherManualSpeed = FINAL_FIRE_VELOCITY;
		m_globals->d_hopperSwitch = true;
		//	m_globals->i_launcherDesiredTilt = FINAL_SHOOTER_ANGLE;
		fireDiscs();
		break;

	default:
		printf("/nError in Autonomous: Code has entered unreachable state./n");
		break;
	}
	printf("Hopper: %d\nSpatula: %d\nLauncher Speed: %f\nAutonomous State: %d\nLeftEncoder%d\n",
			(m_globals->d_hopperSwitch), m_globals->d_spatulaDown,
			m_globals->d_launcherManualSpeed, m_state, m_globals->i_encoderLeft);
}
void bhs_Autonomous::FiveAutonomous(){
	setDefaults();
	if(m_state > k_midreverse && m_state < k_prepareFour)
	{
		m_globals->d_launcherManualSpeed = 0.0;
	}
	switch(m_state){
	//Get the shooter up to speed and the hopper in place
	case k_spinUp:
		m_globals->d_hopperSwitch = true;
		m_globals->d_spatulaDown = true;
		m_globals->d_spatulaUp = false;
		if(m_timer->Get()>SPIN_UP_TIME){//INITIAL_EMF - m_globals->i_launcherSpeed < LAUNCHER_THRESHOLD){
			m_state = k_fireThree;
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Fire the three starting frisbees
	case k_fireThree:
		m_globals->d_hopperSwitch = true;
		m_globals->d_spatulaDown = true;
		m_globals->d_spatulaUp = false;
		if(m_timer->Get() < FIRE_TIME)
			fireDiscs();
		else{
			m_state = k_scoopTwo;//k_midreverse;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		/*	case k_midreverse:
		m_globals->d_joystick_2_y = REVERSE_VELOCITY;
		if(fabs(m_globals->i_encoderLeft) > REVERSE_DISTANCE){
			m_state = k_scoopTwo;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;*/

		//Lower the hopper and spatula while accelerating to scoop up the frisbees below the pyramid
	case k_scoopTwo:
		m_globals->d_spatulaDown = true;
		m_globals->d_spatulaUp = false;
		if(m_timer->Get() > LOWER_HOPPER_TIME)
		{
			m_globals->d_spatulaDown = true;
			m_globals->d_spatulaUp = false;
		}
		m_globals->d_launcherFireSequence = false;
		m_globals->d_hopperSwitch = false;
		m_globals->d_joystickArcadeToggle = true;
		m_globals->d_joystick_2_y = AUTO_VELOCITY;
		if(fabs(m_globals->i_encoderLeft) > inchToEncoder(PYRAMID_DISCS_DISTANCE))
		{
			m_state = k_loadTwo;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Decelerate and load the frisbees into the hopper
	case k_loadTwo:
		m_globals->d_hopperSwitch = false;
		if(m_timer->Get() < RAISE_INTAKE_TIME){
			m_globals->d_spatulaDown = false;
			m_globals->d_spatulaUp = true;
		}
		else if(m_timer->Get() < RAISE_INTAKE_TIME + LOWER_INTAKE_TIME){
			m_globals->d_spatulaDown = true;
			m_globals->d_spatulaUp = false;
		}
		else{
			m_state = k_prepareFour;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		m_globals->d_joystick_1_y = AUTO_SLOW_VELOCITY;
		break;

		//Raise hopper and get shooter up to speed and angle
	case k_prepareFour: 
		m_globals->d_launcherManualTilt = FINAL_SHOOTER_ANGLE;
		m_globals->d_launcherPresetSwitch = LAUNCHER_MANUAL;
		m_globals->d_launcherManualSpeed = FINAL_FIRE_VELOCITY;
		m_globals->d_hopperSwitch = true;
		if(m_timer->Get()>2.0){//FINAL_EMF - m_globals->i_launcherSpeed < LAUNCHER_THRESHOLD){
			m_state = k_fireFour;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Fire the final four disks
	case k_fireFour:
		m_globals->d_launcherManualTilt = FINAL_SHOOTER_ANGLE;
		m_globals->d_launcherPresetSwitch = LAUNCHER_MANUAL;
		m_globals->d_launcherManualSpeed = FINAL_FIRE_VELOCITY;
		m_globals->d_hopperSwitch = true;

		fireDiscs();
		break;

	default:
		printf("/nError in Autonomous: Code has entered unreachable state./n");
		break;
	}
	printf("Hopper: %d\nSpatula: %d\nLauncher Speed: %f\nAutonomous State: %d\nLeftEncoder%d\n",
			(m_globals->d_hopperSwitch), m_globals->d_spatulaDown,
			m_globals->d_launcherManualSpeed, m_state, m_globals->i_encoderLeft);
}
void bhs_Autonomous::MidFieldAutonomous(bool rightSide){
	setDefaults();
	switch(m_state){
	//Get the shooter up to speed and the hopper in place
	case k_spinUp:
		m_globals->d_hopperSwitch = true;
		if(m_timer->Get()>3.0){//INITIAL_EMF - m_globals->i_launcherSpeed < LAUNCHER_THRESHOLD){
			m_state = k_fireThree;
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Fire the three starting frisbees
	case k_fireThree:
		m_globals->d_hopperSwitch = true;
		if(m_timer->Get() < FIRE_TIME)
			fireDiscs();
		else{
			m_state = k_midreverse;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;
	case k_midreverse:
		m_globals->d_joystick_2_y = REVERSE_VELOCITY;
		if(fabs(m_globals->i_encoderLeft) > REVERSE_DISTANCE){
			m_state = k_midturnToCenter;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;
	case k_midturnToCenter:
		m_globals->d_turn = (rightSide? SIDE_ANGLE : -SIDE_ANGLE);
		m_globals->d_turnSwitch = GlobalData::WANT_TO_TURN;
		if(fabs(m_globals->i_gyroAngle - (rightSide? SIDE_ANGLE : -SIDE_ANGLE)) < MAX_TURN_ERROR){
			m_state = k_scoopTwo;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;
		//Lower the hopper and spatula while accelerating to scoop up the frisbees below the pyramid
	case k_scoopTwo:
		m_globals->d_spatulaDown = true;
		m_globals->d_spatulaUp = false;
		m_globals->d_launcherFireSequence = false;
		m_globals->d_hopperSwitch = false;
		m_globals->d_joystickArcadeToggle = true;
		m_globals->d_joystick_2_y = AUTO_VELOCITY;
		if(fabs(m_globals->i_encoderLeft) > fabs(DISTANCE_TO_CENTER))
		{
			m_state = k_loadTwo;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Decelerate and load the frisbees into the hopper
	case k_loadTwo:
		m_globals->d_hopperSwitch = false;
		if(m_timer->Get() < RAISE_INTAKE_TIME){
			m_globals->d_spatulaDown = false;
			m_globals->d_spatulaUp = true;
		}
		else if(m_timer->Get() < RAISE_INTAKE_TIME + LOWER_INTAKE_TIME){
			m_globals->d_spatulaDown = true;
			m_globals->d_spatulaUp = false;
		}
		else{
			m_state = k_midturnToFire;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;
	case k_midturnToFire:
		m_globals->d_turn = (rightSide? MIDFIELD_ANGLE : -MIDFIELD_ANGLE);
		m_globals->d_turnSwitch = GlobalData::WANT_TO_TURN;
		if(fabs((m_globals->i_gyroAngle) - (rightSide? MIDFIELD_ANGLE : -MIDFIELD_ANGLE)) < MAX_TURN_ERROR){
			m_state = k_prepareFour;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;
	case k_prepareFour: 
		m_globals->i_launcherSpeed = MIDFIELD_FIRE_VELOCITY;
		m_globals->d_hopperSwitch = true;
		//	m_globals->i_launcherDesiredTilt = MIDFIELD_SHOOTER_ANGLE;
		if(m_timer->Get()>2.0){//FINAL_EMF - m_globals->i_launcherSpeed < LAUNCHER_THRESHOLD){
			m_state = k_fireFour;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Fire the final four disks
	case k_fireFour:
		m_globals->d_launcherManualSpeed = MIDFIELD_FIRE_VELOCITY;
		m_globals->d_hopperSwitch = true;
		//	m_globals->i_launcherDesiredTilt = MIDFIELD_SHOOTER_ANGLE;
		fireDiscs();
		break;

	default:
		printf("/nError in Autonomous: Code has entered unreachable state./n");
		break;
	}
	printf("Hopper: %d\nSpatula: %d\nLauncher Speed: %f\nAutonomous State: %d\nLeftEncoder%d\n",
			(m_globals->d_hopperSwitch), m_globals->d_spatulaDown,
			m_globals->d_launcherManualSpeed, m_state, m_globals->i_encoderLeft);

}
void bhs_Autonomous::SevenMidFieldAutonomous(){
#if 0
	m_globals->d_spatulaDown = false;
	m_globals->d_spatulaUp = true;	//Defaults, just in case one of the double solenoids is not set
	m_globals->d_hopperSwitch = false;
	switch(m_state){
	//Get the shooter up to speed and the hopper in place
	case k_spinUp:
		m_globals->d_launcherManualSpeed = INITIAL_FIRE_VELOCITY;
		m_globals->d_hopperSwitch = true;
		m_globals->d_launcherManualTilt = FIRST_SHOOTER_ANGLE;
		if(INITIAL_EMF - m_globals->i_launcherSpeed < LAUNCHER_THRESHOLD){
			m_state = k_fireThree;
			m_timer->Start();
		}
		break;

		//Fire the three starting frisbees
	case k_fireThree:
		m_globals->d_hopperSwitch = true;
		if(m_timer->Get() < FIRE_TIME)
			m_globals->d_launcherFireSequence = true;
		else{
			m_state = k_scoopTwo;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;
	case k_midturnToCenter:
		m_globals->d_turn = ANGLE_1;
		m_globals->d_turnSwitch = GlobalData::WANT_TO_TURN;
		if(fabs(m_globals->i_gyroAngle - ANGLE_1) < MAX_TURN_ERROR){
			m_state = k_midreachCenter;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;
	case k_midreachCenter:
		m_globals->d_joystickArcadeToggle = true;
		m_globals->d_joystick_1_y = AUTO_VELOCITY;
		if(m_globals->i_encoderLeft > DISTANCE_TO_CENTER){
			m_state = k_midturnToPickup;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;
	case k_midturnToPickup:
		m_globals->d_turn = ANGLE_2;
		m_globals->d_turnSwitch = GlobalData::WANT_TO_TURN;
		if(fabs(fabs(m_globals->i_gyroAngle) - fabs(ANGLE_2)) < MAX_TURN_ERROR){
			m_state = k_scoopTwo;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;
	case k_scoopTwo:
		if(m_timer->Get() > LOWER_HOPPER_TIME)
		{
			m_globals->d_spatulaDown = true;
			m_globals->d_spatulaUp = false;
		}
		m_globals->d_launcherFireSequence = false;
		m_globals->d_hopperSwitch = false;
		m_globals->d_joystickArcadeToggle = true;
		m_globals->d_joystick_1_y = MIDFIELD_VELOCITY;
		if(m_globals->i_encoderLeft > PICKUP_MIDFIELD)
		{
			m_state = k_loadTwo;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Decelerate and load the frisbees into the hopper
	case k_loadTwo:
		m_globals->d_hopperSwitch = false;
		if(m_timer->Get() < RAISE_INTAKE_TIME){
			m_globals->d_spatulaDown = false;
			m_globals->d_spatulaUp = true;
		}
		else if(m_timer->Get() < RAISE_INTAKE_TIME + LOWER_INTAKE_TIME){
			m_globals->d_spatulaDown = true;
			m_globals->d_spatulaUp = false;
		}
		else{
			m_state = k_scoopFour;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		m_globals->d_joystick_1_y = 0;
		break;

		//Accelerate to pick up the last two disks
	case k_scoopFour:
		m_globals->d_spatulaDown = true;
		m_globals->d_spatulaUp = false;
		m_globals->d_hopperSwitch = false;
		m_globals->d_joystickArcadeToggle = true;
		m_globals->d_joystick_1_y = MIDFIELD_VELOCITY;
		if(m_globals->i_encoderLeft > PICKUP_MIDFIELD)
		{
			m_state = k_loadFour;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Stop and load the final two disks
	case k_loadFour:
		m_globals->d_hopperSwitch = false;
		if(m_timer->Get() < RAISE_INTAKE_TIME){
			m_globals->d_spatulaDown = false;
			m_globals->d_spatulaUp = true;
		}
		else if(m_timer->Get() < RAISE_INTAKE_TIME + LOWER_INTAKE_TIME){
			m_globals->d_spatulaDown = true;
			m_globals->d_spatulaUp = false;
		}
		else{
			m_state = k_prepareFour;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		m_globals->d_joystick_1_y = 0;
		break;
	case k_midturnToFire:
		m_globals->d_turn = ANGLE_3;
		m_globals->d_turnSwitch = GlobalData::WANT_TO_TURN;
		if(fabs(fabs(m_globals->i_gyroAngle) - fabs(ANGLE_3)) < MAX_TURN_ERROR){
			m_state = k_scoopTwo;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;
		//Raise hopper and get shooter up to speed and angle
	case k_prepareFour:
		m_globals->d_launcherManualSpeed = MIDFIELD_FIRE_VELOCITY;
		m_globals->d_hopperSwitch = true;
		m_globals->d_launcherManualTilt = MIDFIELD_SHOOTER_ANGLE;
		if(MIDFIELD_EMF - m_globals->i_launcherSpeed < LAUNCHER_THRESHOLD){
			m_state = k_fireFour;
			m_timer->Stop();
			m_timer->Reset();
			m_timer->Start();
		}
		break;

		//Fire the final four disks
	case k_fireFour:
		m_globals->d_hopperSwitch = true;
		m_globals->d_autoEnabled = true;
		m_globals->d_launcherFireSequence = true;
		break;
	}
#endif
}
#endif
