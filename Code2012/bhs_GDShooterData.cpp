#include "bhs_GDShooterData.h"

bhs_GDShooterData::bhs_GDShooterData()
	: msd_initBatteryVoltage(0)

	, msd_switchSpeedOverride(false)

	, msd_potentiometerReading(0.0)
	, msd_buttonPowerShot(false)
	, msd_buttonLayUp(false)
	, msd_buttonHoopOverrideTop(false)
	, msd_buttonHoopOverrideBottom(false)
	, msd_impulseLockDistanceForShooter(false)

	, msd_encoderRate(0.0)
	, msd_flavAvgEncRate(0.0)
	, msd_flavMinEncRate(0.0)
	, msd_flavMaxEncRate(0.0)
	, msd_desiredRate(0.0)
//	, msd_desiredVelocity(0)
	, msd_pidAddition(0)
	, msd_distanceToHoopFromCamera(0.0)

	, msd_shooterPower(0.0)
	, msd_shooterTarget(0)

#if TEST_SHOOTER_PID
	, msd_p(0.0001666)
	, msd_i(0.0)
	, msd_d(0.0)
#endif
{

}
