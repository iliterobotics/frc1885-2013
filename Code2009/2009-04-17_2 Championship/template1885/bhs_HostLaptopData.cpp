#include "bhs_HostLaptopData.h"
#include "RobotGlobalData.h"
bhs_HostLaptopData::bhs_HostLaptopData()
{
  
}

void bhs_HostLaptopData::setAllData(RobotGlobalData *apData, DriverStation *apDS)
{
  mnBatteryVoltage = apDS->GetBatteryVoltage();
  mnAllianceColor = apDS->GetAlliance();
  if(apDS->IsDisabled())
  {
    mnUserDataMarker1 = 0;
    mnUserDataMarker2 = 0;
  }
  else
  {
    mnUserDataMarker1 = 0xCAFEBEAD;
    mnUserDataMarker2 = 0xDEADFACE;
  }
  apData->lockAllData();
  getAllRoboticValues(apData);
  getAllJoystickValues(apDS);
  apData->releaseAllData();
}

void bhs_HostLaptopData::getJoystickValues(RobotGlobalData *apData)
{
	mnUserXaxis = apData->mnUserXaxis;
	mnUserYaxis = apData->mnUserYaxis;
	mnUserAngle = apData->mnUserAngle;
}

void bhs_HostLaptopData::getMotorValues(RobotGlobalData *apData)
{
	mnMotorAPower = apData->mnMotorAPower;
	mnMotorBPower = apData->mnMotorBPower;
	mnMotorCPower = apData->mnMotorCPower;
	mnMotorDPower = apData->mnMotorDPower;
	  
	mnLeftMotorFront = apData->mnLeftMotorFront;
	mnLeftMotorBack = apData->mnLeftMotorBack;
	mnRightMotorFront = apData->mnRightMotorFront;
	mnRightMotorBack = apData->mnRightMotorBack;
	  
	mbPhysicalMotorAFacing = apData->mbPhysicalMotorAFacing;
	mbPhysicalMotorBFacing = apData->mbPhysicalMotorBFacing;
	mbPhysicalMotorCFacing = apData->mbPhysicalMotorCFacing;
	mbPhysicalMotorDFacing = apData->mbPhysicalMotorDFacing;
	
	mnCellLoaderPower = apData->mnCellLoaderPower;
}

void bhs_HostLaptopData::getSensorValues(RobotGlobalData *apData)
{
	mnCameraTurretTiltDegree = apData->mnCameraTurretTiltDegree;
	mnCameraTurretPanDegree = apData->mnCameraTurretPanDegree;
	
	mnCameraTurretActualTiltDegree = apData->mnCameraTurretActualTiltDegree;
	mnCameraTurretActualPanDegree = apData->mnCameraTurretActualPanDegree;
	
	mnAccelerometerXValue = apData->mnAccelerometerXValue;
	mnAccelerometerYValue = apData->mnAccelerometerYValue;
	mnGyroValue = apData->mnGyroValue;
	
	mnRobotPositionX = apData->mnRobotPositionX;
	mnRobotPositionY = apData->mnRobotPositionY;
	mnRobotSpeed = apData->mnRobotSpeed;
	mnRobotDirection = apData->mnRobotDirection;
}

void bhs_HostLaptopData::getProcessedTargets(RobotGlobalData *apData)
{
  //Getting User Desired Shooting Distance and Angle
  mnUserDesiredShootingDistance = apData->mnUserDesiredShootingDistance;
  mnUserDesiredShootingAngle = apData->mnUserDesiredShootingAngle;
  
  //store process target data
  std::set<bhs_ProcessedTarget>::const_iterator pos;
  int i = 0;
  for(pos = apData->mmTargets.begin(); pos != apData->mmTargets.end(); pos++)
  {
    maTargets[i].mnAngle = pos->mnAngle;
    maTargets[i].mnDistance = pos->mnDistance;
    maTargets[i].mnTeam = (long)pos->meTeam;
    maTargets[i].mnValidity = (long)pos->mbValid;
    i++;
    if(i >= bhs_Constants::cnMAX_TARGET_REPORTS)
    {
      break;
    }
  }
  for(; i<bhs_Constants::cnMAX_TARGET_REPORTS; i++)
  {
    maTargets[i].mnValidity = false;
  }
  
  mnActualShootingDistance = apData->mnActualShootingDistance;
  mnActualShootingAngle = apData->mnActualShootingAngle;
  
  mnDesiredShootingDistance = apData->mnDesiredShootingDistance;
  mnDesiredShootingAngle = apData->mnDesiredShootingAngle;
  
  mnCurrentTargetDistance = apData->mnCurrentTargetDistance;
  mnCurrentTargetAngle = apData->mnCurrentTargetAngle;
  
  mnCameraZone1Active = (long)apData->mbCameraZone1Active;
  mnCameraZone2Active = (long)apData->mbCameraZone2Active;
  mnCameraZone3Active = (long)apData->mbCameraZone3Active;
}

void bhs_HostLaptopData::getAllRoboticValues(RobotGlobalData *apData)
{
	mnRobotPositionX = apData->mnRobotPositionX;
	mnRobotPositionY = apData->mnRobotPositionY;
	mnRobotSpeed = apData->mnRobotSpeed;
	mnRobotDirection = apData->mnRobotDirection;
	
	meDriveStatus = apData->meDriveStatus;
	
	mnAirPressure = apData->mnAirPressure;
	mnRunCompressor = apData->mbRunCompressor;
	
	
	if(apData->mnCellLoaderPower == bhs_Constants::cnCELL_LOADER_PUSH_OUT_POWER)
	{
	  mnCellLoaderState = (long)Relay::kForward;
	}
	else if(apData->mnCellLoaderPower == bhs_Constants::cnCELL_LOADER_TAKE_IN_POWER)
  {
    mnCellLoaderState = (long)Relay::kReverse;
  }
	else
  {
    mnCellLoaderState = (long)Relay::kOff;
  }
	//mnFeederState = (long)apData->meFeederState;
	
	//mnTurretLockOn = (long)apData->mbTurretLockOn;
	
	mbAutonomousSwitchState = (long)apData->mbAutonomousSwitchState;
	
	getJoystickValues(apData);
	getMotorValues(apData);
	getSensorValues(apData);
	getProcessedTargets(apData);
}

void bhs_HostLaptopData::getAllJoystickValues(DriverStation *apData)
{
  //joystick 1-4
  mnJoystick1X = apData->GetStickAxis(1, 1);
  mnJoystick1Y = apData->GetStickAxis(1, 2);
  mnJoystick1Buttons = apData->GetStickButtons(1);
  mnJoystick2X = apData->GetStickAxis(2, 1);
  mnJoystick2Y = apData->GetStickAxis(2, 2);
  mnJoystick2Buttons = apData->GetStickButtons(2);
  mnJoystick3X = apData->GetStickAxis(3, 1);
  mnJoystick3Y = apData->GetStickAxis(3, 2);
  mnJoystick3Buttons = apData->GetStickButtons(3);
  mnJoystick4X = apData->GetStickAxis(4, 1);
  mnJoystick4Y = apData->GetStickAxis(4, 2);
  mnJoystick4Buttons = apData->GetStickButtons(4);
  
  //Driverstation inputs
  mnAnalogIn1 = apData->GetAnalogIn(1);
  mnAnalogIn2 = apData->GetAnalogIn(2);
  mnAnalogIn3 = apData->GetAnalogIn(3);
  mnAnalogIn4 = apData->GetAnalogIn(4);
  mnDigitalIn1 = apData->GetDigitalIn(1);
  mnDigitalIn2 = apData->GetDigitalIn(2);
  mnDigitalIn3 = apData->GetDigitalIn(3);
  mnDigitalIn4 = apData->GetDigitalIn(4);
  mnDigitalIn5 = apData->GetDigitalIn(5);
  mnDigitalIn6 = apData->GetDigitalIn(6);
  mnDigitalIn7 = apData->GetDigitalIn(7);
  mnDigitalIn8 = apData->GetDigitalIn(8);
  
}
