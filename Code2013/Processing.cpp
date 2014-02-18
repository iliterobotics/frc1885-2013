#include "Processing.h"
#include "Constants.h"

#define DECELERATION_CONST -1
#define JOYSTICK_DEADZONE 0.0692
#define ARCADE_DEADBAND 0.1
#define BEARING_TOLEARNCE 5.0
#define TURN_DEGREES_PID 1

#define WAIT_FOR_PANDA 5000

#define convertJoyValue(val) ((val)/DECELERATION_CONST)

GlobalData* Processing::gd = GlobalData::GetInstance();
#if TURN_DEGREES_PID
PID Processing::m_straightPID;
PID Processing::m_turnPID;
int Processing::m_loadBlinkCount = 0;
#endif
int Processing::m_fireState = K_INIT;
int Processing::m_dataAttempt = 0;
Timer* Processing::m_kickerTimer = new Timer();

void Processing::process() {
#if TURN_DEGREES_PID
	m_straightPID.setConstants(gd->c_straightP, gd->c_straightI,
			gd->c_straightD);
	m_turnPID.setConstants(gd->c_turnP, gd->c_turnI, gd->c_turnD);
#endif

	// Low priority inputs
	//processFinger();
	processHopper();
	//processSpatula();
	processManualKick();
	processDriveTrain();

	// Mid priority inputs
	/* Preset shooter speed and tilt processing moved to processLauncherPosition() in high priority */

	// High priority inputs
	processLauncherPosition();
	processAutoFireSequence();
	processHang();
}

void Processing::processFinger() {
	gd->o_finger = gd->d_fingerUp;
}

void Processing::processHopper() {
	if (gd->d_hopperSwitch)
		gd->s_hopper->putUp();
	else
		gd->s_hopper->putDown();

}

void Processing::processSpatula() {
	if (gd->d_spatulaDown)
		gd->s_spatula->moveDown();
	else if (gd->d_spatulaUp)
		gd->s_spatula->moveUp();
	else if (gd->s_spatula->isUp() && !gd->d_spatulaUp)
		gd->s_spatula->setIntakeOff();
}

void Processing::processManualKick() {
	if (gd->o_matchState==2)
	{
		if (gd->d_manualKick)
		{
			if (m_kickerTimer->Get()==0)
			{
				m_kickerTimer->Start();
				gd->s_launcher->kickerOut();
			}
		}
		else
		{
			if (m_kickerTimer->Get() > gd->i_dsKickerMinTime)
			{
				m_kickerTimer->Stop();
				m_kickerTimer->Reset();
				gd->s_launcher->kickerIn();
			}

		}
	}
	else
	{
		if (gd->d_manualKick)
		{
			gd->s_launcher->kickerOut();
		}
		else
		{
			gd->s_launcher->kickerIn();
		}
	}
}

void Processing::processDriveTrain() {

	static double prevLeft = 0.0;
	static double prevRight = 0.0;

	// We love our motors and treat them kindly.
	if (prevLeft != 0.0 && gd->o_drivetrainLeft != 0.0 && prevLeft
			/ gd->o_drivetrainLeft < 0.0)
		gd->o_drivetrainLeft = 0.0;

	if (prevRight != 0.0 && gd->o_drivetrainRight != 0.0 && prevRight
			/ gd->o_drivetrainRight < 0.0)
		gd->o_drivetrainRight = 0.0;

	if (gd->d_gyroReset)
		gd->g_gyro->Reset();

	// ARCADE
	if (gd->d_joystickArcadeToggle) {
		// DRIVE STRAIGHT
		if (fabs(gd->d_joystick_2_x - JOYSTICK_CENTER) < ARCADE_DEADBAND)
			driveStraight(gd->d_joystick_2_y);

		// NORMAL ARCADE
		else {
			if (gd->d_joystick_2_y > 0) {
				gd->o_drivetrainLeft = -gd->d_joystick_2_x - gd->d_joystick_2_y;
				gd->o_drivetrainRight = gd->d_joystick_2_y - gd->d_joystick_2_x;
			} else {
				gd->o_drivetrainLeft = gd->d_joystick_2_x - gd->d_joystick_2_y;
				gd->o_drivetrainRight = gd->d_joystick_2_x + gd->d_joystick_2_y;
			}
		}
	}
	// TANK
	else {
		gd->o_drivetrainLeft = convertJoyValue(gd->d_joystick_1_y);
		gd->o_drivetrainRight = -convertJoyValue(gd->d_joystick_2_y);
	}

	// TURN DEGREES
	if (gd->d_turnSwitch != GlobalData::NOT_TURNING)
		turnDegrees(gd->d_turn);

	prevLeft = gd->o_drivetrainLeft;
	prevRight = gd->o_drivetrainRight;
}
void Processing::driveStraight(const double speed) {
	const double currentValue = gd->i_gyroAngle;
	const double setPoint = 0.0;
	const double pidOutput = m_straightPID.getPID(currentValue, setPoint, 1);

	if (speed < 0.0) {
		gd->o_drivetrainLeft = (speed + speed * sinf(pidOutput)) / DECELERATION_CONST;
		gd->o_drivetrainRight = -(speed - speed * sinf(pidOutput)) / DECELERATION_CONST;
	} else {
		gd->o_drivetrainLeft = (speed - speed * sinf(pidOutput)) / DECELERATION_CONST;
		gd->o_drivetrainRight = -(speed + speed * sinf(pidOutput)) / DECELERATION_CONST;
	}
}
void Processing::turnDegrees(const double setPoint) {
#if TURN_DEGREES_PID
	const double currentValue = gd->i_gyroAngle;
	const double pidOutput = m_turnPID.getPID(currentValue, setPoint, 1);
	//printf("Gyro value in TurnDegrees: %f\n", gd->i_gyroAngle);

	if (fabs(currentValue - setPoint) < BEARING_TOLEARNCE) {
		gd->o_drivetrainLeft = 0;
		gd->o_drivetrainRight = 0;

		gd->d_turnSwitch = GlobalData::NOT_TURNING;
	} else {
		gd->o_drivetrainLeft = -(sinf(pidOutput)) / DECELERATION_CONST;
		gd->o_drivetrainRight = -(sinf(pidOutput)) / DECELERATION_CONST;
	}
#else
	double bass_power = 0.4; // don't drop the bass
	double prop_power = 1.0 - bass_power;

	double dAngle = angleDifference(gd->i_gyroAngle, setPoint);

	double dtPower = bass_power + prop_power * dAngle/180.0;

	if(dtPower > 1.0)
		dtPower = 1.0;

	if (fabs(currentValue - setPoint) < BEARING_TOLEARNCE)
	{
		gd->o_drivetrainLeft = 0;
		gd->o_drivetrainRight = 0;

		gd->d_turnSwitch = GlobalData::NOT_TURNING;
	}
	else
	{
		gd->o_drivetrainLeft = dtPower;
		gd->o_drivetrainRight = dtPower;
	}
#endif
}
double Processing::angleDifference(double a, double b) {
	double retval = fmod(b - a, 360);
	if (retval < -180)
		retval += 360;
	else if (retval > 180)
		retval -= 360;
	return retval;
}

void Processing::processLauncherPosition() {
	if (gd->d_autoEnabled && !gd->d_launcherFireSequence
			&& gd->d_launcherPresetSwitch != LAUNCHER_MANUAL) // Driver still has manual control when not firing
	{
		gd->o_launcherSpeed = 0;
		gd->o_launcherTilt = 0;
		gd->d_turn = 0;
		return;
	}

	if (gd->d_autoEnabled && gd->d_launcherFireSequence) {
		gd->i_launcherSpeed = PRESET1_WHEEL_SPEED; //gd->i_launcherVisionSpeed; Because kinect always returns 1
		gd->i_launcherDesiredTilt = gd->i_launcherVisionTilt + gd->i_dsTiltOffset;
		printf("vision tilt: %f, actual tilt: %f\t", gd->i_launcherVisionTilt,
				launcherPotToAngle(gd->i_launcherTiltPotentiometer));
	} else {
		switch (gd->d_launcherPresetSwitch) {
		case LAUNCHER_PRESET_1:
			gd->i_launcherSpeed = PRESET1_WHEEL_SPEED;
			gd->i_launcherDesiredTilt = PRESET1_ANGLE + gd->i_dsDefaultTiltOffset;
			break;
		case LAUNCHER_PRESET_2:
			gd->i_launcherSpeed = PRESET2_WHEEL_SPEED;
			gd->i_launcherDesiredTilt = PRESET2_ANGLE;
			break;
		case LAUNCHER_MANUAL:
			gd->i_launcherSpeed = dsSpeedPotScale(gd->d_launcherManualSpeed);
			gd->i_launcherDesiredTilt = dsTiltPotToAngle(
					gd->d_launcherManualTilt);
		}
#if 1
		// Sets tilt to 20 degrees when loading at the feeder station.
		// If already at 20 degrees, blinks lights.
		Relay::Value tempLight = Relay::kForward;
		if (gd->d_launcherPreset3) {
			gd->i_launcherDesiredTilt = PRESET3_ANGLE;
#if 1
			if (m_loadBlinkCount % 6000 == 0) {
				if (tempLight == Relay::kOff)
					tempLight = Relay::kForward;
				else
					tempLight = Relay::kOff;
			}
			m_loadBlinkCount++;
#endif
		} else {
			m_loadBlinkCount = 0;
			tempLight = Relay::kOff;
		}
		gd->o_light1State = tempLight;
		gd->o_light2State = tempLight;
#endif
	}
	gd->s_launcher->moveToSpeed(gd->i_launcherSpeed);
	if (!gd->s_launcher->isAtDesiredAngle()){
		gd->s_launcher->moveToAngle(gd->i_launcherDesiredTilt,
				launcherPotToAngle(gd->i_launcherTiltPotentiometer));
		gd->o_light2State = Relay::kOff;
	}
	else {
		gd->i_launcherDesiredTilt = gd->s_launcher->getAngle();
		gd->o_launcherTilt = 0;
		if (gd->d_autoEnabled || gd->d_launcherPresetSwitch!=LAUNCHER_MANUAL)
			gd->o_light2State = Relay::kForward;
	}

	if (gd->d_autoEnabled && gd->d_launcherFireSequence && gd->d_turnSwitch
			== GlobalData::NOT_TURNING) {
		gd->d_turnSwitch = GlobalData::WANT_TO_TURN;
		gd->d_turn = gd->i_launcherVisionHorizontalAngle;
		// Overrides joystick inputs
		turnDegrees(gd->d_turn);
		printf("%f, %f\n", gd->i_launcherVisionHorizontalAngle,
				gd->g_gyro->GetAngle());
	}
	gd->o_launcherSpeed = - gd->o_launcherSpeed;
}

double Processing::dsTiltPotToAngle(double pot) {
	double pot_min = 0.330709;
	double angle = (40 / (1 - pot_min)) * (pot - pot_min);
	return angle;
}
double Processing::dsSpeedPotScale(double x) {
	double pot_min = 0.330709;
	double scale_factor = 1 - pot_min;
	return (x - pot_min) / scale_factor;
}
double Processing::launcherPotToAngle(double pot) {
	//double angle = (8.3981 * pot) + 0.7784;
	//double angle = (8.462 * pot) + 0.4521; //Disabled on 4/25
	//double angle = (8.8156 * pot) + -2.2329;//Disabled 4/26
	double angle = (8.4089 * pot) - 2.3154;
	return angle;
}
double Processing::launcherAngleToPot(double angle) {
	//double pot = (0.1189 * angle) - 0.089;
	double pot = (0.1181 * angle) - 0.0515;
	return pot;
}

void Processing::processAutoFireSequence() {
	if (gd->d_launcherFireSequence) {
		printf( "fireState: %d\n", m_fireState);
		switch (m_fireState) {
		case K_INIT:
			// To minimize vibration so the kinect can detect blobs.
			gd->g_comp->Stop();
			gd->s_launcher->moveToSpeed(0);
			gd->d_gyroReset = true;
			m_fireState = K_ACQUIRE_DATA;
			gd->i_requestForVision = true;
			break;

		case K_ACQUIRE_DATA:
			// If Panda board does not connect or does not detect any blobs, stop trying to ask it for data
			if (m_dataAttempt >= WAIT_FOR_PANDA) {
				m_fireState = K_INIT;
				break;
			}
			m_dataAttempt++;
			if (gd->d_autoEnabled)
			{
				gd->i_launcherSpeed = gd->i_launcherVisionSpeed;
				gd->i_launcherDesiredTilt = gd->i_launcherVisionTilt + gd->i_dsTiltOffset;
			}
			else
			{
				gd->i_launcherSpeed = gd->d_launcherManualSpeed;
				gd->i_launcherDesiredTilt = gd->d_launcherManualTilt;
			}
			//gd->d_turn = gd->i_launcherVisionHorizontalAngle;
			if (gd->i_launcherSpeed != 0 && gd->i_launcherDesiredTilt != 0) {
				printf("light1\n");
				gd->o_light1State = Relay::kForward;
				m_fireState = K_ALIGNMENT;
			}
			break;

		case K_ALIGNMENT:
			gd->g_comp->Start();
			gd->s_hopper->putUp();
			gd->s_launcher->moveToAngle(gd->i_launcherDesiredTilt,
					launcherPotToAngle(gd->i_launcherTiltPotentiometer));
			gd->s_launcher->moveToSpeed(gd->i_launcherSpeed);
			//turnDegrees(gd->d_turn);
			m_fireState = K_TRANSIT;
			// Fallthrough. There is no break because... it doesn't need to wait until the next loop.

		case K_TRANSIT:
			//if (isReadyToLaunch()) 
			gd->s_hopper->putUp();
			printf( "k_transit\n");
			if (gd->s_launcher->isAtDesiredSpeed() && gd->s_launcher->isAtDesiredAngle())
			{
				printf( "k_transit, at desired speed and angle\n");
				gd->o_light2State = Relay::kForward;
				m_fireState = K_FIRE;
			}
			break;

		case K_FIRE:
			gd->s_launcher->launch(true);
			// We do not worry here, in the switch, about what happens if the button is released,
			// code does not enter this switch if the button is not pressed.
			break;

		default:
			printf("Illegal state: auto fire sequence.\n");
			break;

		}
	} else if (m_fireState != K_INIT) {
		gd->o_light1State = Relay::kOff;
		gd->o_light2State = Relay::kOff;
		m_fireState = K_INIT;
		gd->g_comp->Start();
		gd->s_launcher->launch(false);
		gd->s_launcher->moveToSpeed(0);
		gd->s_launcher->moveToAngle(gd->i_launcherDesiredTilt,
				launcherPotToAngle(gd->i_launcherTiltPotentiometer));
	}

}
bool Processing::isReadyToLaunch() {
	// bool hopperUp = gd->s_hopper->isUp();
	// No limit switches, no way of checking current status
	gd->s_hopper->putUp();
	bool positionsAdjusted = gd->s_launcher->isAtDesiredSpeed()
									&& gd->s_launcher->isAtDesiredAngle() 
									&& gd->d_turnSwitch	== GlobalData::NOT_TURNING; //FIXME: check logic for whether is at desired bearing

	return positionsAdjusted;
}
void Processing::ensureReadyToLaunch() {
	// if (gd->s_hopper->isUp())
	// 	gd->o_hopper = DoubleSolenoid::kOff;
	// else
	// No limit switches, no way of checking current status
	gd->s_hopper->putUp();

	//FIXME: fallback for vision communication failure?
	if (gd->i_launcherSpeed == 0.0)
		gd->i_launcherSpeed = PRESET1_WHEEL_SPEED;
	if (gd->i_launcherDesiredTilt == 0.0)
		gd->i_launcherDesiredTilt = PRESET1_ANGLE + gd->i_dsDefaultTiltOffset;

	if (!gd->s_launcher->isAtDesiredSpeed())
		gd->s_launcher->moveToSpeed(gd->i_launcherSpeed);
	if (!gd->s_launcher->isAtDesiredAngle())
		gd->s_launcher->moveToAngle(gd->i_launcherDesiredTilt,
				launcherPotToAngle(gd->i_launcherTiltPotentiometer));
	//if (gd->d_turnSwitch != GlobalData::NOT_TURNING)
	//	turnDegrees(gd->d_turn);
}

void Processing::processHang() {
	if (gd->d_hangSafetySwitch) {
		//ensureReadyToHang();

		if (gd->d_hangRetracted)
			gd->s_hang->retract();
		else if (gd->d_hangExtended)
			gd->s_hang->extend();
		else
			gd->o_hang = DoubleSolenoid::kOff;

	} else {
		//FIXME: emergency protocol for switching off hang in middle of hanging?
		gd->o_hang = DoubleSolenoid::kOff;
	}
}
#if 0
bool Processing::isReadyToHang() {
	gd->s_hopper->putDown();
	//gd->s_spatula->moveUp();

	printf("%d\t%f\t%f\t%d\n", gd->s_hopper->isDown(),
			gd->s_launcher->getAngle(), gd->s_launcher->getSpeed(),
			gd->s_spatula->isUp());

	return gd->s_hopper->isDown() && fabs(
			gd->s_launcher->getAngle() - (PRESET1_ANGLE+gd->i_dsDefaultTiltOffset)) < 2.0
			&& gd->s_launcher->getSpeed() == 0.0;
	//gd->s_spatula->isUp();
	// && gd->s_hang->isExtended();	//FIXME: if isExtended is a part of the hang precondition, it would ignore driver input
}
void Processing::ensureReadyToHang() {
	//	 if(gd->s_hopper->isDown())
	//	 	gd->o_hopper = DoubleSolenoid::kReverse;

	gd->s_hopper->putDown();

	gd->s_launcher->moveToAngle(PRESET1_ANGLE+gd->i_dsDefaultTiltOffset,
			launcherPotToAngle(gd->i_launcherTiltPotentiometer));

	if (gd->s_launcher->getSpeed() != 0.0)
		gd->s_launcher->moveToSpeed(0.0);

	//if (!gd->s_spatula->isUp())
	//gd->s_spatula->moveUp();

	// if (!gd->s_hang->isExtended())	//FIXME: if isExtended is a part of the hang precondition, it would ignore driver input
	// 	gd->s_hang->extend();		
}
#endif
