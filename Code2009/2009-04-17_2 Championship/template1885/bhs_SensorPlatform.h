/*
 * bhs_SensorPlatform.h
 *
 *  Created on: Jan 22, 2008
 *      Author: ITRobo
 */

#ifndef __BHS_SENSORPLATFORM_H__
#define __BHS_SENSORPLATFORM_H__

#include "RobotGlobalData.h"
#include "bhs_Accelerometer.h"
#include "bhs_Constants.h"
#include "Gyro.h"

class bhs_SensorPlatform
{
  private:
    enum Orientation{NORTH, SOUTH, EAST, WEST};
    
	public:
		bhs_SensorPlatform();
		void Initialize();
		~bhs_SensorPlatform();
		
	  // Sets the way angles are inputted, with startAngle being the user's 0 degree
	  // mark
	  void angleOrientation(Orientation startAngle);
	  // Resets x, y, speed, and direction to default
	  void reset();
	  // Resets speed and direction to 0
	  void resetVelocity();
	  
	  // Updates the x and y coordinates
	  void update();
	  // Returns x coordinate
	  float getX();
	  // Returns y coordinate
	  float getY();
	  // Returns acceleration
	  float getAccelX();
	  float getAccelY();
	  // Returns speed
	  float getSpeedX();
	  float getSpeedY();
	  // Returns direction
	  float getGyroAngle();
	  float getDirection();
		
	private:
	  // A portion of a second
	  static const double mcPERIOD = 1.0 / bhs_Constants::cnSENSOR_PLATFORM_EXE_RATE;
	  
	  int mnSensorPlatformTimingTask;
	  int mnSensorPlatformTask;
	  
	  int mnPrintDelay;
	  
		bhs_Accelerometer *accelX;
		bhs_Accelerometer *accelY;
		Gyro *gyro;
		
		SEM_ID mcTimingSem;
		
		// In meters/second^2
		float mAccelX;
		float mAccelY;
		
		// In meters/second
	  float mVelocitySpeedX;
	  float mVelocitySpeedY;
	  // Actual gyro angle
	  float mGyroAngle;
	  // Stored with east being 0 degrees
	  float mAbsoluteDirection;
	  // The current x coordinate
	  float mX;
	  // The current y coordinate
	  float mY;
	  // The difference between inputted angle and 0 as east
	  float mGyroAngleOffset;
	  // The angle of movement relative to the robot
	  float mVelocityAngle;
	  
	  // Sets velocity given acceleration
	  void calcVelocity(float anAccelerationX, float anAccelerationY);
};

#endif
