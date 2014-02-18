#ifndef BHS_CONSTANTS_H
#define BHS_CONSTANTS_H

#include "types/vxTypesOld.h"
#include "Joystick.h"

#define USE_TEST_ROBOT 0

// joystick/button board related defines
#if USE_TEST_ROBOT
#define USE_DRIVER_JOYSTICKS 1
#define USE_TWO_DRIVER_JOYSTICKS 0
#define USE_OPERATOR_JOYSTICKS 0
#define USE_TWO_OPERATOR_JOYSTICKS 0
#else
#define USE_DRIVER_JOYSTICKS 1
#define USE_TWO_DRIVER_JOYSTICKS 1
#define USE_OPERATOR_JOYSTICKS 1
#define USE_TWO_OPERATOR_JOYSTICKS 1
#endif

#define DISPLAY_BRIDGE_LOWERING_BUTTON_DEBUG 0
#define DISPLAY_BUTTON_BOARD_POTENTIOMETER 0

// sensor related defines
#if USE_TEST_ROBOT
#define USE_HOOP_CAMERA 1
#define USE_GROUND_CAMERA 0
#define USE_DRIVE_TRAIN_GYRO 1
#define USE_BRIDGE_GYRO 0
#define USE_DRIVE_TRAIN_ENCODERS 1
#define USE_SHOOTER_ENCODER 1
#define USE_BRIDGE_LIMIT_SWITCHES 0
#define USE_AUTONOMOUS_SWITCHES 0
#else
#define USE_HOOP_CAMERA 1
#define USE_GROUND_CAMERA 1
#define USE_DRIVE_TRAIN_GYRO 1
#define USE_BRIDGE_GYRO 1
#define USE_DRIVE_TRAIN_ENCODERS 1
#define USE_SHOOTER_ENCODER 1
#define USE_BRIDGE_LIMIT_SWITCHES 1
#define USE_AUTONOMOUS_SWITCHES 1
#endif

#define DISPLAY_BRIDGE_LIMIT_SWITCHES 0
#define DISPLAY_AUTONOMOUS_SWITCHES 0

// processing related defines
#define USE_TANK_DRIVE 1
#define USE_SHOOTER_ABS_POWER 1

#define DISPLAY_BRIDGE_LOWERING_DEBUG 0

// motor output related defines
#if USE_TEST_ROBOT
#define USE_DRIVE_TRAIN_MOTORS 1
#define USE_FOUR_DRIVE_TRAIN_MOTORS 0
#define USE_CONVEYOR_MOTORS 0
#define USE_SHOOTER_MOTORS 1
#define USE_TWO_SHOOTER_MOTORS 0
#define USE_BRIDGE_LOWERING_MOTORS 0
#else
#define USE_DRIVE_TRAIN_MOTORS 1
#define USE_FOUR_DRIVE_TRAIN_MOTORS 1
#define USE_CONVEYOR_MOTORS 1
#define USE_SHOOTER_MOTORS 1
#define USE_TWO_SHOOTER_MOTORS 1
#define USE_BRIDGE_LOWERING_MOTORS 1
#endif

// drive angle testing defines
#define TEST_DRIVE_ANGLE_PID 0
#define TEST_DRIVE_ANGLE_MIN_SPEEDS 0
#define TEST_DRIVE_ANGLE_ANGLES 0
#define TEST_DRIVE_ANGLE_GYRO_DEADBAND 0
#define DISPLAY_DRIVE_ANGLE_DEBUG 0

// shooter PID testing related defines
#define TEST_SHOOTER_PID 0
#define TEST_SHOOTER_POWERS 0
#define TEST_SHOOTER_DISTANCES 0
#define TEST_SHOOTER_MIN_MAX_RATES 0
#define TEST_SHOOTER_CONST_POWER 0
#define TEST_SHOOTER_BATTERY_VOLTAGE 0
#define DISPLAY_SHOOTER_DEBUG 0

class bhs_Constants {
public:
	// drive train constants
	static const float JOYSTICK_DEAD_ZONE;
	static const double PID_STRAIGHT_P;
	static const double PID_STRAIGHT_I;
	static const double PID_STRAIGHT_D;
	static const double PID_ANGLE_P;
	static const double PID_ANGLE_I;
	static const double PID_ANGLE_D;
	static const float DRIVE_ANGLE_THRESHOLD;
	static const float DRIVE_ANGLE_DIFF_THRESHOLD;
	static const float MIN_DRIVE_ANGLE_SPEED_STATIC;
	static const float MIN_DRIVE_ANGLE_SPEED_KINETIC;
	
	// shooter constants
	static const float SHOOTER_ANGLE;
	static const double PID_SHOOTER_P;
	static const double PID_SHOOTER_I;
	static const double PID_SHOOTER_D;
	static const float SHOOTER_PID_POWER_THRESHOLD;
};

#endif // BHS_CONSTANTS_H
