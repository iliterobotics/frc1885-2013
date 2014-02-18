/*
 * RobotGlobalData.cpp
 *
 *  Created on: Feb 12, 2009
 *      Author: ITRobo
 */

#include "RobotGlobalData.h"

bool operator==(const bhs_ProcessedTarget& acFirst,
                const bhs_ProcessedTarget& acSecond)
{
  bool lbReturnVal = false;
  
  if (acFirst.mnAngle == acSecond.mnAngle && acFirst.mnDistance
      == acSecond.mnDistance)
  {
    lbReturnVal = true;
  }
  return lbReturnVal;
}

bool operator<(const bhs_ProcessedTarget &acFirst,
               const bhs_ProcessedTarget &acSecond)
{
  double lnFirstHash = acFirst.mnAngle + acFirst.mnDistance * (1000);
  
  double lnSecondHash = acSecond.mnAngle + acSecond.mnDistance * (1000);
  
  bool lbReturnVal = false;
  
  if (lnFirstHash < lnSecondHash)
  {
    lbReturnVal = true;
  }
  return lbReturnVal;
}

bool operator>(const bhs_ProcessedTarget& acFirst, const bhs_ProcessedTarget& acSecond)
{
  double lnFirstHash = acFirst.mnAngle + acFirst.mnDistance * (1000);
  
  double lnSecondHash = acSecond.mnAngle + acSecond.mnDistance * (1000);
  
  bool lbReturnVal = false;
  
  if (lnFirstHash > lnSecondHash)
  {
    lbReturnVal = true;
  }
  return lbReturnVal;
}

/*
 * Default constructor
 */
RobotGlobalData::RobotGlobalData()
{
  printf("Begin RobotGlobalData constructor\n");
  mcTurretSem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
  mcTargetingSem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
  
  mnUserXaxis = 0;
  mnUserYaxis = 0;
  mnUserAngle = 0;
  meDriveStatus = kMotorsForward;
  mnMotorAPower = 0;
  mnMotorBPower = 0;
  mnMotorCPower = 0;
  mnMotorDPower = 0;
  
  mnLeftMotorFront = 0;
  mnLeftMotorBack = 0;
  mnRightMotorFront = 0;
  mnRightMotorBack = 0;
  
  mbPhysicalMotorAFacing = gbMotorFacingForward;
  mbPhysicalMotorBFacing = gbMotorFacingForward;
  mbPhysicalMotorCFacing = gbMotorFacingForward;
  mbPhysicalMotorDFacing = gbMotorFacingForward;
  
  mnCameraTurretTiltDegree = 0;
  mnCameraTurretPanDegree = 0;
  
  mnCameraTurretActualTiltDegree = bhs_Constants::cnFIXED_TILT_ANGLE;
  mnCameraTurretActualPanDegree = 0;
  
  mnAccelerometerXValue = 0;
  mnAccelerometerYValue = 0;
  mnGyroValue = 0;
  
  mnAirPressure = 0;
  mbRunCompressor = true;
  
  mnActualShootingDistance = 0;
  mnActualShootingAngle = 0;
  mnPOTRawValue = 0;
  mnCellLoaderPower = 0;
  mbDumperSpinBackwards = false;
  meShooterState = kShooterManualIdle;
  
  mnDesiredShootingAngle = 90;
  mnDesiredShootingDistance = 0;
  
  mnUserDesiredShootingAngle = 90;
  mnUserDesiredShootingDistance = 0;
  
  mnCurrentTargetDistance = 0;
  mnCurrentTargetAngle = 0;
  
  meConveyorState = Relay::kOff;
  meCellFeederState = Relay::kOff;
  meCellHolderState = bhs_Constants::cnCELL_HOLDER_OFF_VALUE;
  
  mbCameraZone1Active = true;
  mbCameraZone2Active = true;
  mbCameraZone3Active = true;
  
  //mbDebugTurret = false;
  mbDebugDumper = false;
  
  //mbTurretLockOn = false;
  
  printf("Completed RobotGlobalData constructor\n");
}

/*
 * Default destructor
 */
RobotGlobalData::~RobotGlobalData()
{
  semFlush(mcTurretSem);
  semFlush(mcTargetingSem);
}

void RobotGlobalData::getCameraTurretData(bhs_CameraTurretData &acCameraTurretData)
{
  semTake(mcTurretSem, WAIT_FOREVER);
  acCameraTurretData.mnCameraTurretTiltDegree = mnCameraTurretTiltDegree;
  acCameraTurretData.mnCameraTurretPanDegree = mnCameraTurretPanDegree;
  acCameraTurretData.mnCameraTurretActualTiltDegree = mnCameraTurretActualTiltDegree;
  acCameraTurretData.mnCameraTurretActualPanDegree = mnCameraTurretActualPanDegree;
  semGive(mcTurretSem);
}

void RobotGlobalData::setCameraTurretData(bhs_CameraTurretData acCameraTurretData)
{
  semTake(mcTurretSem, WAIT_FOREVER);
  mnCameraTurretTiltDegree = acCameraTurretData.mnCameraTurretTiltDegree;
  mnCameraTurretPanDegree = acCameraTurretData.mnCameraTurretPanDegree;
  mnCameraTurretActualTiltDegree = acCameraTurretData.mnCameraTurretActualTiltDegree;
  mnCameraTurretActualPanDegree = acCameraTurretData.mnCameraTurretActualPanDegree;
  semGive(mcTurretSem);
}

void RobotGlobalData::getTargetingData(bhs_TargetingData &acTargetingData)
{
  semTake(mcTargetingSem, WAIT_FOREVER);
  acTargetingData.mmTargets = mmTargets;
  acTargetingData.mnCurrentTargetDistance = mnCurrentTargetDistance;
  acTargetingData.mnCurrentTargetAngle = mnCurrentTargetAngle;
  acTargetingData.mbCameraZone1Active = mbCameraZone1Active;
  acTargetingData.mbCameraZone2Active = mbCameraZone2Active;
  acTargetingData.mbCameraZone3Active = mbCameraZone3Active;
  semGive(mcTargetingSem);
}

void RobotGlobalData::setTargetingData(bhs_TargetingData acTargetingData)
{
  semTake(mcTargetingSem, WAIT_FOREVER);
  mmTargets = acTargetingData.mmTargets;
  mnCurrentTargetDistance = acTargetingData.mnCurrentTargetDistance;
  mnCurrentTargetAngle = acTargetingData.mnCurrentTargetAngle;
  mbCameraZone1Active = acTargetingData.mbCameraZone1Active;
  mbCameraZone2Active = acTargetingData.mbCameraZone2Active;
  mbCameraZone3Active = acTargetingData.mbCameraZone3Active;
  semGive(mcTargetingSem);
}

void RobotGlobalData::lockAllData()
{
  semTake(mcTurretSem, WAIT_FOREVER);
  semTake(mcTargetingSem, WAIT_FOREVER);
}

void RobotGlobalData::releaseAllData()
{
  semGive(mcTurretSem);
  semGive(mcTargetingSem);
}
