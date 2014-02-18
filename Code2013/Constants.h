#ifndef CONSTATNS_H
#define CONSTANTS_H

#define MIN_ANGLE_VOLTAGE 0.057 //0.002124
#define MAX_ANGLE_VOLTAGE 1.76 //1.786
#define MIN_ANGLE 0.0
#define MIN_VOLTAGE 0.000817
#define MAX_ANGLE 40.0
#define MAX_VOLTAGE 5

#define LAUNCH_DEFAULT_SPEED 0.35
#define PRESET1_ANGLE 26.5
#define PRESET1_POT_VOLTAGE 2.660676
#define PRESET1_WHEEL_SPEED 1.0
#define PRESET2_ANGLE 16.5
#define PRESET2_POT_VOLTAGE 1.323030
#define PRESET2_WHEEL_SPEED 0.0
#define PRESET3_ANGLE 20

#define LAUNCH_POT_TOLERANCE 0.4 // IN VOLTAGE
#define LAUNCHER_TILT_SPEED 0.4

#define KICK_DELAY 0.75
#define SHOOTER_SPEED_TOLERANCE 0.15
#define TILT_ANGLE_TOLERANCE 0.5


// MOTOR CONSTANTS
#define LAUNCHER_TILT_MAX_SPEED 0.30

// Driver Station Constants
#define ENCODER_DIST_MULTIPLIER 400
#define ENCODER_DIST_CONST -1000
#define KICKER_MIN_TIME_DIVISOR 5
#define TILT_OFFSET_MULTIPLIER 2
#define TILT_OFFSET_CONST -5


#endif
