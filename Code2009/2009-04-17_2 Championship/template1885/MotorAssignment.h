/*
 * MotorAssignment.h
 *
 *  Created on: Jan 15, 2009
 *      Author: ITRobo
 */

/*
 * Description:
 */

#ifndef __MOTORASSIGNMENT_H__
#define __MOTORASSIGNMENT_H__

#include "RobotGlobalData.h"

class MotorAssignment
{
  public:
    void assignToPhysicalMotor(RobotGlobalData* apGlobalData);
    void setMotorDirections(RobotGlobalData* apGlobalData);

  private:
    enum MotorPowerState
    { kForward, kSideways, kAutomatic, kVector, kHybridForward, kHybridBackward, kHybridLeft, kHybridRight, kNumPowerStates};

    /* Each row is ordered as motors left front, left back, right front,
     right back.
     */
    static const float caMotorPowerDirections[kNumPowerStates][4];
};

#endif
