#ifndef AUTONOMOUS_H
#define AUTONOMOUS_H

#define INCH_CONV 28.5

#include "Timer.h"
#include "GlobalData.h"
#include <Math.h>

//Time constants
#define FIRE_TIME 5.0
#define RESET_TIME 0.0
#define LOWER_HOPPER_TIME 0.5
#define RAISE_INTAKE_TIME 2.0
#define LOWER_INTAKE_TIME 0.5
#define SPIN_UP_TIME 3.0

//Shooter Angle Constants
#define ANGLE_TO_POT_VOLT(x) ((x)*(1-0.330709)/40 + 0.330709)
#define FIRST_SHOOTER_ANGLE ANGLE_TO_POT_VOLT(25.0)
#define FINAL_SHOOTER_ANGLE ANGLE_TO_POT_VOLT(30.0)
#define SIDE_SHOOTER_ANGLE ANGLE_TO_POT_VOLT(26.0)
#define MIDFIELD_SHOOTER_ANGLE ANGLE_TO_POT_VOLT(24.0)

//Shooter Velocity Constants
#define INITIAL_FIRE_VELOCITY 1
#define FINAL_FIRE_VELOCITY 1
#define SIDE_FIRE_VELOCITY 0.75
#define MIDFIELD_FIRE_VELOCITY 1

//Drivetrain Velocity Constants
#define AUTO_VELOCITY -0.35
#define AUTO_SLOW_VELOCITY 0.0
#define MIDFIELD_VELOCITY 0.0
#define REVERSE_VELOCITY 0.35

//Encoder Distance Constants
#define DISTANCE_TO_PYRAMID_DISKS 1000
#define DISTANCE_TO_FAR_DISKS 2000
#define DISTANCE_TO_CENTER 1000
#define REVERSE_DISTANCE 400
#define PICKUP_MIDFIELD 0

//Distance constants in inches
#define PYRAMID_DISCS_DISTANCE 40
#define FAR_DISCS_DISTANCE 138
#define REVERSE_DISTANCE_INCHES 72
#define ENC_PER_INCH 29.5

//Drivetrain Angle Constants
#define ANGLE_1 0.0
#define ANGLE_2 0.0
#define ANGLE_3 0.0
#define SIDE_ANGLE -140.0
#define MIDFIELD_ANGLE 160.0

//EMF Constants
#define INITIAL_EMF 0.0
#define FINAL_EMF 0.0
#define MIDFIELD_EMF 0.0

//Threshold Constants
#define ACTUATIONS_PER_SECOND 1.5
#define LAUNCHER_THRESHOLD 0.1
#define MAX_TURN_ERROR 5.0

class bhs_Autonomous
{

public:
	//Constructor
	bhs_Autonomous();
	//Destructor
	~bhs_Autonomous();

	double inch(int num);
	double foot(int num);
#if 0
	void SevenAutonomous();
	void MidFieldAutonomous(bool rightSide);
	void SevenMidFieldAutonomous();
	void FiveAutonomous();
//	void DriveStraightTest();
	void SideThreeDiscs();
#endif
	void MiddleThreeDiscs();
	void ThreeDiscReverse();
	void ThreeDiscReverse180();
	void EncoderTest(double enc);
	
	//The constants that are indented are only utilized in SevenMidField and MidField
	typedef enum {
		k_spinUp = 0,
		k_fireThree,
			k_midreverse,
			k_midturnToCenter,
			k_midreachCenter,
			k_midturnToPickup,
		k_scoopTwo,	
		k_loadTwo,			
		k_scoopFour,
		k_loadFour,
			k_midturnToFire,
		k_finalReverse,
		k_prepareFour,
		k_fireFour
	} State;
	State m_state;

private:
	inline void fireDiscs();
	inline void setDefaults();
	long inchToEncoder(int inches);

	Timer* m_timer;

	GlobalData* m_globals;
};

#endif
