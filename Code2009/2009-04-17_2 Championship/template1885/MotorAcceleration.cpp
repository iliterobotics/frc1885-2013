/*
 * MotorAcceleration.cpp
 *
 *  Created on: Jan 21, 2009
 *      Author: ITLab
 */

#include "MotorAcceleration.h"
#include <math.h>

/*
 * Default constructor
 */
MotorAcceleration::MotorAcceleration()
{
  mnPreviousPower = cnZERO_POWER;
}

/*
 * Default destructor
 */
MotorAcceleration::~MotorAcceleration()
{
  
}

float MotorAcceleration::accelerateMotor(float anMotorPower)
{
  float lnActualMotorPower;
  float lnAllowableAcceleration;
  
  anMotorPower += cnZERO_POWER;
  
  //Calculate the maximum acceleration allowed this time.  Acceleration is based
  //on an exponential curve, modified by MAX_ACCEL_MULTIPLIER.
    
  //If the exponential curve gives a maximum acceleration that is too low, use the
  //minimum set acceleration instead.
  if (fabs(mnPreviousPower-cnZERO_POWER) * cnMAX_ACCEL_MULTIPLIER < cnMIN_ACCEL)
  {
    lnAllowableAcceleration = cnMIN_ACCEL;
  }
  //Otherwise, use math to determine the maximum acceleration for this iteration.
  else
  {
    lnAllowableAcceleration = fabs(mnPreviousPower-cnZERO_POWER) * cnMAX_ACCEL_MULTIPLIER;
  }
  
  
  //Set the final power to the motor
  //Limit the acceleration to an exponential curve determined earlier
  if (fabs(anMotorPower-mnPreviousPower) > lnAllowableAcceleration)
  {
    //If acceleration is positive, then add the maximum acceleration
    if (anMotorPower > mnPreviousPower)
    {
      lnActualMotorPower = mnPreviousPower + lnAllowableAcceleration;
    }
    //If the acceleration is negative, then subtract the maximum acceleration
    else
    {
      lnActualMotorPower = mnPreviousPower - lnAllowableAcceleration;
    }
  }
  //Since the wanted acceleration is less than the calculated maximum acceleration,
  //set the power directly to the motor
  else
  {
    lnActualMotorPower = anMotorPower;
  }
  
  mnPreviousPower = lnActualMotorPower;
  
  //Due to float imprecision, the value of the motor power may be close to 1.0
  //(no power) when it should be exactly 1.0
  /*if(lnActualMotorPower > 0.995 && lnActualMotorPower < 1.005)
  {
    //mnPreviousPower is not changed here just in case the first few acceleration
    //cases fall in this dead zone; we don't want to sit still...
    return 0;
  }*/
  
  //Limit the output values to -1.0 to 1.0.  Hopefully this should not happen...
  if(lnActualMotorPower > 2.0)
  {
    //printf("Bad acceleration, attempted to assign motor power to %f. Setting to 1.0 instead.\n", lnActualMotorPower-1.0);
    mnPreviousPower = cnFULL_FORWARD;
    return 1.0;
  }
  if(lnActualMotorPower < 0)
  {
    //printf("Bad acceleration, attempted to assign motor power to %f. Setting to -1.0 instead.\n", lnActualMotorPower-1.0);
    mnPreviousPower = cnFULL_REVERSE;
    return -1.0;
  }
  
  return lnActualMotorPower - cnZERO_POWER;
}

void MotorAcceleration::reset()
{
  mnPreviousPower = cnZERO_POWER;
}
