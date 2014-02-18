#include "bhs_RotateRobot.h"

bhs_RotateRobot::bhs_RotateRobot()
:mcNumCountsForRotationFinish(50)
{
  
}
bhs_RotateRobot::~bhs_RotateRobot()
{
  
}

void bhs_RotateRobot::configCount(int anCnt)
{
  mcNumCountsForRotationFinish = anCnt;
}

void bhs_RotateRobot::setGains(double anProportional, 
                               double anIntegral,
                               double anDerivative)
{
  mcRotatePid.setGains(anProportional,anIntegral,anDerivative);
}

bhs_RotateData bhs_RotateRobot::rotateToAngle(double anCurrentAngle,
                                              double anDesireAngle)
{
  bhs_RotateData lsReturn;
  lsReturn.mbDone = false;
  
  lsReturn.mnXaxis = mcRotatePid.computeOutput(anDesireAngle, anCurrentAngle);
  if((anCurrentAngle >= (anDesireAngle - 1)) && 
      (anCurrentAngle <= (anDesireAngle + 1)))
  {
    mcCountAtDegree++;
  }
  else
  {
    mcCountAtDegree = 0;
  }
  
  if(mcCountAtDegree >= mcNumCountsForRotationFinish)
  {
    lsReturn.mbDone = true;
  }
  
  return lsReturn;
}
