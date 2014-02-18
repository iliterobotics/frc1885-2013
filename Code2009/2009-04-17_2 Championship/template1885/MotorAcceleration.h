/*
 * MotorAcceleration.h
 *
 *  Created on: Jan 21, 2009
 *      Author: ITLab
 */

/*
 * Description:
 */
#ifndef __MOTORACCELERATION_H__
#define __MOTORACCELERATION_H__

#import "RobotGlobalData.h"

class MotorAcceleration
{
  public:
    MotorAcceleration();
    ~MotorAcceleration();
    float accelerateMotor(float anMotorPower);
    void reset();
    
  private:
    static const float cnMAX_ACCEL_MULTIPLIER = 0.05;
    static const float cnMIN_ACCEL = 0.01;
    static const float cnZERO_POWER = 1.0;
    static const float cnFULL_FORWARD = 2.0;
    static const float cnFULL_REVERSE = 0;
    
    float mnPreviousPower;
};

#endif
