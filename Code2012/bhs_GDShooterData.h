#ifndef BHS_GD_SHOOTER_DATA_H
#define BHS_GD_SHOOTER_DATA_H

#include "bhs_Constants.h"

class bhs_GDShooterData {
public:
	bhs_GDShooterData();

	float msd_initBatteryVoltage;
	
	bool msd_switchSpeedOverride;
	
	float msd_potentiometerReading;
	bool msd_buttonPowerShot;
	bool msd_buttonLayUp;
	bool msd_buttonHoopOverrideTop;
	bool msd_buttonHoopOverrideBottom;
	bool msd_impulseLockDistanceForShooter;

	double msd_encoderRate;
	double msd_flavAvgEncRate;
	double msd_flavMinEncRate;
	double msd_flavMaxEncRate;
	double msd_desiredRate;
//	double msd_desiredVelocity;
	float msd_pidAddition;
	double msd_distanceToHoopFromCamera;
	
	float msd_shooterPower;
	float msd_shooterTarget;
	
#if TEST_SHOOTER_PID
	double msd_p;
	double msd_i;
	double msd_d;
#endif

};

#endif // BHS_GD_SHOOTER_DATA_H
