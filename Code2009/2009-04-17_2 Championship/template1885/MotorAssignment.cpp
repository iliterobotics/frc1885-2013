#include "RobotGlobalData.h"
#include "MotorAssignment.h"

/* The first row is the motor directions for forward motor state
 and the second row is for the sideways motor state.  Each row
 is ordered as motors left front, left back, right front,
 right back.
 */
const float MotorAssignment::caMotorPowerDirections[kNumPowerStates][4] =
{
{ 1.0, 1.0, -1.0, -1.0 },   //kForward
{ -1.0, 1.0, 1.0, -1.0 },   //kSideways
{ 1.0, 1.0, -1.0, -1.0 },   //kAutomatic
{ -1.0, 1.0, 1.0, -1.0 },   //kVector
{ 1.0, 1.0, 1.0, -1.0 },    //kHybridForward
{ 1.0, 1.0, -1.0, 1.0 },    //kHybridBackward
{ 1.0, 1.0, -1.0, 1.0 },    //kHybridLeft
{ -1.0, 1.0, 1.0, 1.0 } };  //kHybridRight

void MotorAssignment::assignToPhysicalMotor(RobotGlobalData* apGlobalData)
{
  /*This function ensures that when the drivetrain twitches the motors are
   * correctly reassigned to compensate for the state of the robot.
   */
  switch (apGlobalData->meDriveStatus)
  {
    /*This is the state is when the wheels are forwards.
     */
    case RobotGlobalData::kMotorsForward:
      apGlobalData->mnMotorAPower = apGlobalData->mnLeftMotorBack;
      apGlobalData->mnMotorBPower = apGlobalData->mnLeftMotorFront;
      apGlobalData->mnMotorCPower = apGlobalData->mnRightMotorFront;
      apGlobalData->mnMotorDPower = apGlobalData->mnRightMotorBack;
      break;
      /*This state is when the wheels are all facing sideways.
       */
    case RobotGlobalData::kMotorsSideways:
      apGlobalData->mnMotorAPower = apGlobalData->mnRightMotorBack;
      apGlobalData->mnMotorBPower = apGlobalData->mnLeftMotorBack;
      apGlobalData->mnMotorCPower = apGlobalData->mnLeftMotorFront;
      apGlobalData->mnMotorDPower = apGlobalData->mnRightMotorFront;
      break;
      /*This state is the same as the forwards state.
       */
    case RobotGlobalData::kAutomatic:
      apGlobalData->mnMotorAPower = apGlobalData->mnLeftMotorBack;
      apGlobalData->mnMotorBPower = apGlobalData->mnLeftMotorFront;
      apGlobalData->mnMotorCPower = apGlobalData->mnRightMotorFront;
      apGlobalData->mnMotorDPower = apGlobalData->mnRightMotorBack;
      break;
    case RobotGlobalData::kVector:
      apGlobalData->mnMotorAPower = apGlobalData->mnLeftMotorBack;
      apGlobalData->mnMotorBPower = apGlobalData->mnLeftMotorFront;
      apGlobalData->mnMotorCPower = apGlobalData->mnRightMotorFront;
      apGlobalData->mnMotorDPower = apGlobalData->mnRightMotorBack;
      break;
    case RobotGlobalData::kVectorForwardHybrid:
    case RobotGlobalData::kVectorBackwardHybrid:
    case RobotGlobalData::kVectorLeftHybrid:
    case RobotGlobalData::kVectorRightHybrid:
      apGlobalData->mnMotorAPower = apGlobalData->mnLeftMotorBack;
      apGlobalData->mnMotorBPower = apGlobalData->mnLeftMotorFront;
      apGlobalData->mnMotorCPower = apGlobalData->mnRightMotorFront;
      apGlobalData->mnMotorDPower = apGlobalData->mnRightMotorBack;
      break;
  }
}

void MotorAssignment::setMotorDirections(RobotGlobalData* apGlobalData)
{
  MotorPowerState state;
  
  switch (apGlobalData->meDriveStatus)
  {
    case RobotGlobalData::kMotorsForward:
      state = kForward;
      break;
    case RobotGlobalData::kMotorsSideways:
      state = kSideways;
      break;
    case RobotGlobalData::kAutomatic:
      state = kAutomatic;
      break;
    case RobotGlobalData::kVector:
      state = kVector;
      break;
    case RobotGlobalData::kVectorForwardHybrid:
      state = kHybridForward;
      break;
    case RobotGlobalData::kVectorBackwardHybrid: //Added by Buddy
      state = kHybridBackward;
      break;
    case RobotGlobalData::kVectorLeftHybrid:
      state = kHybridLeft;
      break;
    case RobotGlobalData::kVectorRightHybrid:
      state = kHybridRight;
      break;
    default:
      state = kForward;
  }
  apGlobalData->mnMotorAPower *= caMotorPowerDirections[state][0];
  apGlobalData->mnMotorBPower *= caMotorPowerDirections[state][1];
  apGlobalData->mnMotorCPower *= caMotorPowerDirections[state][2];
  apGlobalData->mnMotorDPower *= caMotorPowerDirections[state][3];
}
