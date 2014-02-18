/*
 * bhs_SensorPlatform.cpp
 *
 *  Created on: Jan 22, 2008
 *      Author: ITRobo
 */

#include "bhs_SensorPlatform.h"
#include "bhs_Constants.h"
#include "bhs_SensorPlatform.h"
#include "bhs_Accelerometer.h"
#include "Gyro.h"
#include "math.h"
#include "sysLib.h"

static int SensorPlatformTiming(int apTimingSem, int anPeriodicDelay)
{
  SEM_ID* lpSem = (SEM_ID*)apTimingSem;
  while (true)
  {
    taskDelay(anPeriodicDelay);
    semGive(*lpSem);
  }
  return 0;
}

static int SensorPlatformTask(int apSensorPlatform, int apTimingSem)
{
  bhs_SensorPlatform* lpSensorPlatform = (bhs_SensorPlatform*)apSensorPlatform;
  SEM_ID* lpSem = (SEM_ID*)apTimingSem;
  while (true)
  {
    semTake(*lpSem, WAIT_FOREVER);
    lpSensorPlatform->update();
  }
}

/*
 * Default constructor
 */
bhs_SensorPlatform::bhs_SensorPlatform()
{
  accelX = new bhs_Accelerometer(bhs_Constants::cnACCELEROMETER_X_CHANNEL);
  accelY = new bhs_Accelerometer(bhs_Constants::cnACCELEROMETER_Y_CHANNEL);
  gyro = new Gyro(bhs_Constants::cnGYRO_CHANNEL);
  accelX->SetSensitivity(bhs_Constants::cnACCELEROMETER_X_SENSITIVITY);
  accelY->SetSensitivity(bhs_Constants::cnACCELEROMETER_Y_SENSITIVITY);
  gyro->SetSensitivity(bhs_Constants::cnGYRO_SENSITIVITY);
  
  mVelocitySpeedX = 0;
  mVelocitySpeedY = 0;
  mAbsoluteDirection = 0;
  mX = 0;
  mY = 0;
  mGyroAngleOffset = 0;
  
  mcTimingSem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
}

void bhs_SensorPlatform::Initialize()
{
  //We want this to run a certain number of times a second
  int lnPeriodicDelay = sysClkRateGet()/bhs_Constants::cnSENSOR_PLATFORM_EXE_RATE;
  
  mnSensorPlatformTimingTask = taskSpawn("Sensor Platform Timing", 99, //task pri.
                                          VX_FP_TASK, // options
                                          64000, // stack size
                                          (FUNCPTR)SensorPlatformTiming, // function to start
                                          (int)&mcTimingSem, // targeting class to start
                                          lnPeriodicDelay, 0, 0, 0, 0, 0, 0, 0, 0);
  
  mnSensorPlatformTask = taskSpawn("Sensor Platform", 101, //task pri.
                                    VX_FP_TASK, // options
                                    64000, // stack size
                                    (FUNCPTR)SensorPlatformTask, // function to start
                                    (int)this, // targeting class to start
                                    (int)&mcTimingSem, 0, 0, 0, 0, 0, 0, 0, 0);
}

bhs_SensorPlatform::~bhs_SensorPlatform()
{
  delete accelX;
  delete accelY;
  delete gyro;
  semFlush(mcTimingSem);
}

void bhs_SensorPlatform::update()
{
  static int lnDelayCount = 0;
  mAccelX = accelX->GetAcceleration()*bhs_Constants::G_FORCE_TO_ACCELERATION;
  mAccelY = accelY->GetAcceleration()*bhs_Constants::G_FORCE_TO_ACCELERATION;
  mGyroAngle = gyro->GetAngle();
  
  calcVelocity(mAccelX, mAccelY);
  mAbsoluteDirection = mGyroAngle + mGyroAngleOffset + mVelocityAngle;
  if (mAbsoluteDirection > 360)
    mAbsoluteDirection = fmod(mAbsoluteDirection, 360);
  else if (mAbsoluteDirection<0)
    mAbsoluteDirection = 360 - fmod((mAbsoluteDirection*-1), 360);
  
  mX += mVelocitySpeedX * cos(mAbsoluteDirection * bhs_Constants::PI / 180)
      * mcPERIOD;
  mY += mVelocitySpeedY * sin(mAbsoluteDirection * bhs_Constants::PI / 180)
      * mcPERIOD;
  
  lnDelayCount++;
  if (lnDelayCount >= 50)
  {
    /*printf("Number 7's version 1");
    printf("accelX %f accelY %f gyro %f\n", mAccelX, mAccelY, mGyroAngle);
    printf("velocityX %f velocityY %f velocityAngle %f\n", mVelocitySpeedX, mVelocitySpeedY, mVelocityAngle);
    printf("absDirection %f posX %f posY %f\n", mAbsoluteDirection, mX, mY);*/
    lnDelayCount = 0;
  }
}

void bhs_SensorPlatform::calcVelocity(float anAccelerationX, float anAccelerationY)
{
  mVelocitySpeedX += anAccelerationX * mcPERIOD;
  mVelocitySpeedY += anAccelerationY * mcPERIOD;
   
  if (mVelocitySpeedX == 0 && mVelocitySpeedY == 0)
    mVelocityAngle = 0;
  else if (mVelocitySpeedY < 0 && mVelocitySpeedX > 0)
    mVelocityAngle = 360 +  atan(mVelocitySpeedY/mVelocitySpeedX) * 180 / bhs_Constants::PI;
  else if (mVelocitySpeedX == 0 && mVelocitySpeedY < 0)
    mVelocityAngle = 270;
  else if (mVelocitySpeedY == 0 && mVelocitySpeedX < 0)
    mVelocityAngle = 180;
  else if (mVelocitySpeedX == 0 && mVelocitySpeedY > 0)
    mVelocityAngle = 90;
  else if (mVelocitySpeedY == 0 && mVelocitySpeedX > 0)
    mVelocityAngle = 0;
  else if (mVelocitySpeedY < 0 && mVelocitySpeedX < 0)
    mVelocityAngle = 180 +  atan(mVelocitySpeedY/mVelocitySpeedX) * 180 / bhs_Constants::PI;
  else if (mVelocitySpeedX < 0 && mVelocitySpeedY > 0)
    mVelocityAngle = 180 +  atan(mVelocitySpeedY/mVelocitySpeedX) * 180 / bhs_Constants::PI;
  else if (mVelocitySpeedX > 0 && mVelocitySpeedY > 0)
    mVelocityAngle =  atan(mVelocitySpeedY/mVelocitySpeedX) * 180 / bhs_Constants::PI;
}

void bhs_SensorPlatform::angleOrientation(Orientation startAngle)
{
  switch (startAngle)
  {
    case NORTH:
      mGyroAngleOffset = 90;
      break;
    case EAST:
      mGyroAngleOffset = 0;
      break;
    case SOUTH:
      mGyroAngleOffset = 270;
      break;
    case WEST:
      mGyroAngleOffset = 180;
      break;
    default:
      mGyroAngleOffset = 0;
      break;
  }
}
void bhs_SensorPlatform::resetVelocity()
{
  mVelocitySpeedX = 0;
  mVelocitySpeedY = 0;
  mAbsoluteDirection = 0;
}

void bhs_SensorPlatform::reset()
{
  mVelocitySpeedX = 0;
  mVelocitySpeedY = 0;
  mAbsoluteDirection = 0;
  mY = 0;
  mX = 0;
}

float bhs_SensorPlatform::getX()
{
  return mX;
}

float bhs_SensorPlatform::getY()
{
  return mY;
}

float bhs_SensorPlatform::getSpeedX()
{
  return mVelocitySpeedX;
}

float bhs_SensorPlatform::getSpeedY()
{
  return mVelocitySpeedY;
}

float bhs_SensorPlatform::getAccelX()
{
  return mAccelX;
}

float bhs_SensorPlatform::getAccelY()
{
  return mAccelY;
}

float bhs_SensorPlatform::getGyroAngle()
{
  return mGyroAngle;
}

float bhs_SensorPlatform::getDirection()
{
  //return mAbsoluteDirection - mGyroAngleOffset;
  return mVelocityAngle;
}
