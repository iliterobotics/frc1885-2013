#include "bhs_Constants.h"

const float bhs_Constants::JOYSTICK_DEAD_ZONE = 0.16;
const double bhs_Constants::PID_STRAIGHT_P = 0.1; // default 0.5
const double bhs_Constants::PID_STRAIGHT_I = 0.0; // default 0.0
const double bhs_Constants::PID_STRAIGHT_D = -0.0002; // default -0.001
const double bhs_Constants::PID_ANGLE_P = 0.03;
const double bhs_Constants::PID_ANGLE_I = 0.0;
const double bhs_Constants::PID_ANGLE_D = -0.00015;
// if the current angle is within DRIVE_ANGLE_THRESHOLD of the desired angle, then stop drive angle
const float bhs_Constants::DRIVE_ANGLE_THRESHOLD = 0.18;
// if the rate of change of the current angle is within DRIVE_ANGLE_DIFF_THRESHOLD, then stop drive angle
const float bhs_Constants::DRIVE_ANGLE_DIFF_THRESHOLD = 0.01;
// the minimum power required to rotate the robot when it is not moving
const float bhs_Constants::MIN_DRIVE_ANGLE_SPEED_STATIC = 0.55;
// the minimum power required to rotate the robot when it is moving
const float bhs_Constants::MIN_DRIVE_ANGLE_SPEED_KINETIC = 0.30;

const float bhs_Constants::SHOOTER_ANGLE = 60.0;
const double bhs_Constants::PID_SHOOTER_P = 0.05;
const double bhs_Constants::PID_SHOOTER_I = 0.00;
const double bhs_Constants::PID_SHOOTER_D = 0.00;
const float bhs_Constants::SHOOTER_PID_POWER_THRESHOLD = 0.02;
