#include "bhs_PID.h"

bhs_PID::bhs_PID() :
  mnProportionalGain(0.0), mnIntegralGain(0.0), mnDerivativeGain(0.0),
      mbEnforcePIDOutputLimit(false), mnEnforceIntegralLimit(false),
      mnPIDOutputUpperLimit(0.0), mnPIDOutputLowerLimit(0.0),
      mnIntegralErrorUpperLimit(0.0), mnIntegralErrorLowerLimit(0.0),
      mnIntergralError(0.0), mbCanComputDerivativeGain(false)
{
  
}

bhs_PID::~bhs_PID()
{
}

void bhs_PID::setGains(double anProportional, double anIntegral, double anDerivative)
{
  mnProportionalGain = anProportional;
  mnIntegralGain = anIntegral;
  mnDerivativeGain = anDerivative;
}

void bhs_PID::setIntegralLimit(double anUpperLimit, double anLowerLimit)
{
  mnEnforceIntegralLimit = true;
  mnIntegralErrorUpperLimit = anUpperLimit;
  mnIntegralErrorLowerLimit = anLowerLimit;
}

void bhs_PID::reset()
{
  
  mnIntergralError = 0.0;
  mbCanComputDerivativeGain = false;
}

double bhs_PID::computeOutput(double anDesireOuput, double anCurrent)
{
  double lnPGain = 0.0;
  double lnIGain = 0.0;
  double lnDGain = 0.0;
  
  //computing current error
  double lnError = anDesireOuput -anCurrent;
  
  //computing Proportional gain
  lnPGain = mnProportionalGain*lnError;
  
  //computing Integral gain
  mnIntergralError += lnError; //updating Integral error
  if (mnEnforceIntegralLimit)//limit Intergal error if configure to do so
  {
    if (mnIntergralError > mnIntegralErrorUpperLimit)
    {
      mnIntergralError = mnIntegralErrorUpperLimit;
    }
    else if (mnIntergralError < mnIntegralErrorLowerLimit)
    {
      mnIntegralErrorUpperLimit = mnIntegralErrorLowerLimit;
    }
  }
  lnIGain = mnIntegralGain * mnIntergralError;
  
  //computing derivative gain
  if (mbCanComputDerivativeGain)
  {
    lnDGain = mnDerivativeGain * (mnPreviousDevicePosition - anCurrent);
  }
  mnPreviousDevicePosition = anCurrent;//save value for next time
  //With a Previous value, derivative computation can be done on subsequent computeOutput calls
  mbCanComputDerivativeGain = true;
  
  //error - previous_error
  //lnError = anDesireOuput -anCurrent;
  //-anCurrentPosition + anPreviousPoistion
  return (lnPGain + lnIGain + lnDGain);
}
