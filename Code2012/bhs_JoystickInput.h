#ifndef BHS_JOYSTICK_INPUT_H
#define BHS_JOYSTICK_INPUT_H

#include "DriverStation.h"
#include "Joystick.h"

#include "bhs_Constants.h"
#include "bhs_GlobalData.h"
#include "bhs_ButtonImpulse.h"

class bhs_JoystickInput {
public:
	// joysticks
	static const UINT32 JOYSTICK_DRIVER_1_PORT;
	static const UINT32 JOYSTICK_DRIVER_2_PORT;
	static const UINT32 JOYSTICK_OPERATOR_1_PORT;
	static const UINT32 JOYSTICK_OPERATOR_2_PORT;
	
	// driver buttons
	static const UINT32 BUTTON_USE_ARCADE_DRIVE;
	static const UINT32 BUTTON_ARM_FRONT_UP;
	static const UINT32 BUTTON_ARM_FRONT_DOWN;
	static const UINT32 BUTTON_NUDGE_FORWARD;
	static const UINT32 BUTTON_NUDGE_BACKWARD;
	static const UINT32 BUTTON_NUDGE_LEFT;
	static const UINT32 BUTTON_NUDGE_RIGHT;
	
	// operator buttons
	static const UINT32 SWITCH_SHOOTER_SPEED_OVERRIDE;
	static const UINT32 BUTTON_SHOOTER_POWER_SHOT;
	static const UINT32 BUTTON_SHOOTER_LAY_UP;
	static const UINT32 SWITCH_INTAKE_COLLECT;
	static const UINT32 SWITCH_INTAKE_REVERSE;
	static const UINT32 BUTTON_FEEDER_FEEDING;
	static const UINT32 BUTTON_FEEDER_REVERSE;
	static const UINT32 BUTTON_HOOP_OVERRIDE_TOP;
	static const UINT32 BUTTON_HOOP_OVERRIDE_BOTTOM;
	static const UINT32 BUTTON_LOCK_DISTANCE_FOR_SHOOTER;
	
	static const Joystick::AxisType POTENTIOMETER_AXIS_TYPE;
	static const float POTENTIOMETER_DEAD_ZONE;
	static const float POTENTIOMETER_MIN_VAL;
	
	bhs_JoystickInput(bhs_GlobalData*);
	~bhs_JoystickInput();

	void init();
	void run();

private:
	bhs_GlobalData* m_globalData;

#if USE_DRIVER_JOYSTICKS
	Joystick m_jsDriver1;
#if USE_TWO_DRIVER_JOYSTICKS
	Joystick m_jsDriver2;
#endif
	
	bhs_ButtonImpulse m_impulseNudgeLeft;
	bhs_ButtonImpulse m_impulseNudgeRight;
	bhs_ButtonImpulse m_impulseNudgeForward;
	bhs_ButtonImpulse m_impulseNudgeBackward;
#endif
#if USE_OPERATOR_JOYSTICKS
	Joystick m_jsOperator1;

	bhs_ButtonImpulse m_lockDistanceForShooterImpulse;
#endif

#if TEST_DRIVE_ANGLE_ANGLES
	bhs_ButtonImpulse* m_driveAngleImpulse;
#endif

};

#endif // BHS_INPUT_MANAGER_H
