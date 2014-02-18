/*#include "bhs_RotateTurret.h"

bhs_RotateTurret::bhs_RotateTurret() :
  mcNumCountsForRotationFinish(50)
{
  
}
bhs_RotateTurret::~bhs_RotateTurret()
{
  
}

void bhs_RotateTurret::configCount(int anCnt)
{
  mcNumCountsForRotationFinish = anCnt;
}

void bhs_RotateTurret::setGains(double anProportional, double anIntegral,
                                double anDerivative)
{
  mcRotatePid.setGains(anProportional, anIntegral, anDerivative);
}

bhs_RotateTurretData bhs_RotateTurret::rotateToAngle(double anCurrentAngle,
                                                     double anDesireAngle)
{
  bhs_RotateTurretData lsReturn;
  lsReturn.mbDone = false;
  
  lsReturn.mnPower = mcRotatePid.computeOutput(anDesireAngle, anCurrentAngle);
  
  if (lsReturn.mnPower > bhs_Constants::cnTURRET_MOTOR_MAXIMUM_POWER)
  {
    lsReturn.mnPower = bhs_Constants::cnTURRET_MOTOR_MAXIMUM_POWER;
  }
  else if (lsReturn.mnPower < -1 * bhs_Constants::cnTURRET_MOTOR_MAXIMUM_POWER)
  {
    lsReturn.mnPower = -1 * bhs_Constants::cnTURRET_MOTOR_MAXIMUM_POWER;
  }
  else if(lsReturn.mnPower < bhs_Constants::cnTURRET_MOTOR_MINIMUM_POWER &&
      lsReturn.mnPower > -1 * bhs_Constants::cnTURRET_MOTOR_MINIMUM_POWER)
  {
    lsReturn.mnPower = 0;
  }
  
  if (anCurrentAngle >= (anDesireAngle - 1) && anCurrentAngle <= (anDesireAngle + 1))
  {
    mcCountAtDegree++;
  }
  else
  {
    mcCountAtDegree = 0;
  }
  
  if (mcCountAtDegree >= mcNumCountsForRotationFinish)
  {
    lsReturn.mbDone = true;
  }
  
  return lsReturn;
}*/
