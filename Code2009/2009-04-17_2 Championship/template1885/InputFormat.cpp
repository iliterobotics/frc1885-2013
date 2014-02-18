#include <math.h>
#include "InputFormat.h"
#include "bhs_Constants.h"

const float DEAD_ZONE_VAL = 0.1;

InputFormat::InputFormat()
{
  printf("Begin InputFormat constructor\n");
  mnDeadZone = DEAD_ZONE_VAL;
  mpAirPressureSensor = new DigitalInput(bhs_Constants::cnDIGITAL_AIR_PRESSURE_IN);
  //mpTargetPOT = new TargetPotentiometer(bhs_Constants::cnTURRET_POTENTIOMETER_CHANNEL);
  printf("Completed InputFormat constructor\n");
}

void InputFormat::Initialize(Servo* apPanServo, Servo* apTiltServo)
{
  mpPanServo = apPanServo;
  mpTiltServo = apTiltServo;
}

void InputFormat::handleOperatorInputs(RobotGlobalData* apData, DriverStation* apDS,
                                       bhs_SensorPlatform* apSensorPlatform)
{
  driveData(apData, apDS);
  handleFirstJoystickButtons(apData, apDS);
  handleSensorInputs(apData, apSensorPlatform);
  manualShooting(apData, apDS);
  handleSecondJoystickButtons(apData, apDS);
  handleDebugJoystickButtons(apData, apDS);
}

void InputFormat::handleSensorInputs(RobotGlobalData* apData,
                                     bhs_SensorPlatform* apSensorPlatform)
{
  handlePneumaticInputs(apData);
  handleAnalogInputs(apData, apSensorPlatform);
}

float InputFormat::ConfigJoystickDriveDeadZone(float anDeadZone)
{
  if (anDeadZone < .99 || anDeadZone > -.99)
    return mnDeadZone = anDeadZone;
  return 0;
}

/**get input
 * define dead zone
 * rescale line
 * will need to know angle of line
 */
void InputFormat::driveData(RobotGlobalData* apData, DriverStation* apDS)
{
  bhs_CameraTurretData lcCameraTurretData;
  
  apData->getCameraTurretData(lcCameraTurretData);
  
  //Function variables used for calculations
  double lnXAxis, lnYAxis, lnZAxis, lnThrottle, lnVectorAxis, lnLineAngle,
      lnResultant, lnDesiredResultant;
  
  //get data from driver
  apData->mnRawXaxis = apDS->GetStickAxis(1, 1);
  apData->mnRawYaxis = apDS->GetStickAxis(1, 2);
  apData->mnRawZaxis = apDS->GetStickAxis(1, 3);
  lnThrottle = (-1*(apDS->GetStickAxis(1, 4)) + 1)/2;
  lnXAxis = apDS->GetStickAxis(1, 1);
  lnYAxis = apDS->GetStickAxis(1, 2)*-1.0;
  lnZAxis = apDS->GetStickAxis(1, 3);
  //printf("throttle %f\n", lnThrottle);
  
  //check if x and y axis are in the dead zone
  lnLineAngle=atan2(lnYAxis, lnXAxis);//in radians
  lnVectorAxis = sqrt(pow(lnYAxis, 2) + pow(lnXAxis, 2));
  
  //put the data in the global pointers for everyone else to use

  //ignore commands while in dead zone
  if (lnVectorAxis <= DEAD_ZONE_VAL)
  {
    apData->mnUserXaxis = 0;
    apData->mnUserYaxis = 0;
  }
  //should scale x and y for usable input
  //-1 to 1 = -100% (backward) to 100% (forward)
  else
  {
    lnResultant = lnVectorAxis - mnDeadZone;
    lnDesiredResultant = lnResultant / (1.414213562 - mnDeadZone);
    
    apData->mnUserXaxis = lnDesiredResultant * cos(lnLineAngle);
    apData->mnUserYaxis = lnDesiredResultant * sin(lnLineAngle);
    
    if (apData->mnUserXaxis > 1)
      apData->mnUserXaxis = 1;
    else if (apData->mnUserXaxis < -1)
      apData->mnUserXaxis = -1;
    if (apData->mnUserYaxis > 1)
      apData->mnUserYaxis = 1;
    else if (apData->mnUserYaxis < -1)
      apData->mnUserYaxis = -1;
  }
  
  if (apData->meDriveStatus == RobotGlobalData::kMotorsSideways
      || apData->meDriveStatus == RobotGlobalData::kVectorLeftHybrid
      || apData->meDriveStatus == RobotGlobalData::kVectorRightHybrid)
  {
    apData->mnUserXaxis = lnZAxis * lnThrottle;
    apData->mnUserYaxis = lnXAxis * lnThrottle;
  }
  else
  {
    apData->mnUserXaxis = lnXAxis * lnThrottle;
    apData->mnUserYaxis = lnYAxis * lnThrottle;
  }
  
  if (apData->mnRawXaxis == 0 && -1*apData->mnRawYaxis == 0)
    apData->mnUserAngle = 0;
  else if (-1*apData->mnRawYaxis < 0 && apData->mnRawXaxis> 0)
    apData ->mnUserAngle = 360 + atan(-1*apData->mnRawYaxis/apData->mnRawXaxis) * 180 / bhs_Constants::PI;
  else if (apData->mnRawXaxis == 0 && -1*apData->mnRawYaxis < 0)
    apData->mnUserAngle = 270;
  else if (-1*apData->mnRawYaxis == 0 && apData->mnRawXaxis < 0)
    apData->mnUserAngle = 180;
  else if (apData->mnRawXaxis == 0 && -1*apData->mnRawYaxis> 0)
    apData->mnUserAngle = 90;
  else if (-1*apData->mnRawYaxis == 0 && apData->mnRawXaxis> 0)
    apData->mnUserAngle = 0;
  else if (-1*apData->mnRawYaxis < 0 && apData->mnRawXaxis < 0)
    apData->mnUserAngle = 180 + atan(-1*apData->mnRawYaxis/apData->mnRawXaxis) * 180 / bhs_Constants::PI;
  else if (apData->mnRawXaxis < 0 && -1*apData->mnRawYaxis> 0)
    apData->mnUserAngle = 180 + atan(-1*apData->mnRawYaxis/apData->mnRawXaxis) * 180 / bhs_Constants::PI;
  else if (apData->mnRawXaxis> 0 && -1*apData->mnRawYaxis> 0)
    apData->mnUserAngle = atan(-1*apData->mnRawYaxis/apData->mnRawXaxis) * 180 / bhs_Constants::PI;
  //printf("angle %f\n", apData->mnUserAngle);
  
  apData->mnUserThrottle = lnThrottle;

  //Camera servos current position
  lcCameraTurretData.mnCameraTurretActualTiltDegree = mpTiltServo->GetAngle();
  lcCameraTurretData.mnCameraTurretActualPanDegree = mpPanServo->GetAngle();
  //printf("tilt angle %f\n", mpTiltServo->GetAngle());

  apData->setCameraTurretData(lcCameraTurretData);
}

void InputFormat::handleFirstJoystickButtons(RobotGlobalData* apData, DriverStation* apDS)
{
  short lnButtons = apDS->GetStickButtons(1);
  
  //Handle the buttons to switch drive modes

  //All of the rising edge button functions must be called here just in case
  //somebody presses a button while another button is being pressed. That way,
  //the change in button state will be noted.  The last button on this list that
  //gets the rising edge status will have precedence if two buttons are pressed
  //at exactly the same time.
  if (mcTwitchAutoButton.risingEdge(lnButtons & bhs_Constants::TWITCH_AUTO_BUTTON))
  {
    apData->meDriveStatus = RobotGlobalData::kAutomatic;
  }
  if (mcTwitchVectorButton.risingEdge(lnButtons
      & bhs_Constants::TWITCH_VECTOR_BUTTON))
  {
    apData->meDriveStatus = RobotGlobalData::kVector;
  }
  if (mcTwitchVectorHybridForwardButton.risingEdge(lnButtons
      & bhs_Constants::TWITCH_VECTOR_HYBRID_FRONT_BUTTON))
  {
    apData->meDriveStatus = RobotGlobalData::kVectorForwardHybrid;
  }
  if (mcTwitchVectorHybridBackwardButton.risingEdge(lnButtons
      & bhs_Constants::TWITCH_VECTOR_HYBRID_BACK_BUTTON))
  {
    apData->meDriveStatus = RobotGlobalData::kVectorBackwardHybrid;
  }
  if (mcTwitchVectorHybridLeftButton.risingEdge(lnButtons
      & bhs_Constants::TWITCH_VECTOR_HYBRID_LEFT_BUTTON))
  {
    apData->meDriveStatus = RobotGlobalData::kVectorLeftHybrid;
  }
  if (mcTwitchVectorHybridRightButton.risingEdge(lnButtons
      & bhs_Constants::TWITCH_VECTOR_HYBRID_RIGHT_BUTTON))
  {
    apData->meDriveStatus = RobotGlobalData::kVectorRightHybrid;
  }
  if (mcTwitchSidewaysButton.risingEdge(lnButtons
      & bhs_Constants::TWITCH_SIDEWAYS_BUTTON))
  {
    apData->meDriveStatus = RobotGlobalData::kMotorsSideways;
  }
  if (mcTwitchForwardButton.risingEdge(lnButtons
      & bhs_Constants::TWITCH_FORWARD_BUTTON))
  {
    apData->meDriveStatus = RobotGlobalData::kMotorsForward;
  }
  
  
  if (mcCellFeederForwardButton.risingEdge(lnButtons
        & bhs_Constants::CELL_FEEDER_FORWARD_BUTTON))
  {
    apData->meCellFeederState = bhs_Constants::cnCELL_FEEDER_FORWARD_VALUE;
  }
  if (mcCellFeederReverseButton.risingEdge(lnButtons
      & bhs_Constants::CELL_FEEDER_REVERSE_BUTTON))
  {
    apData->meCellFeederState = bhs_Constants::cnCELL_FEEDER_REVERSE_VALUE;
  }
  if (mcCellFeederOffButton.risingEdge(lnButtons & bhs_Constants::CELL_FEEDER_OFF_BUTTON))
  {
    apData->meCellFeederState = bhs_Constants::cnCELL_FEEDER_OFF_VALUE;
  }
}

void InputFormat::handlePneumaticInputs(RobotGlobalData* apData/*, DriverStation* apDS*/)
{
  apData->mnAirPressure = mpAirPressureSensor->Get();
  
  /*if(mcCompressorButton.risingEdge(apDS->GetStickButtons(1) & bhs_Constants::COMPRESSOR_BUTTON))
   {
   apData->mbRunCompressor = !apData->mbRunCompressor;
   }*/
}

void InputFormat::handleAnalogInputs(RobotGlobalData* apData,
                                     bhs_SensorPlatform* apSensors)
{
  apData->mnAccelerometerXValue = apSensors->getAccelX();
  apData->mnAccelerometerYValue = apSensors->getAccelY();
  apData->mnGyroValue = apSensors->getGyroAngle();
  apData->mnRobotPositionX = apSensors->getX();
  apData->mnRobotPositionY = apSensors->getY();
  apData->mnRobotSpeed = sqrt(apSensors->getSpeedX()*apSensors->getSpeedX()
      +apSensors->getSpeedY()*apSensors->getSpeedY());
  apData->mnRobotDirection = apSensors->getDirection();
  
  /*apData->mnActualShootingAngle = mpTargetPOT->GetAngle();
  apData->mnPOTRawValue = mpTargetPOT->GetRaw();*/
}

void InputFormat::manualShooting(RobotGlobalData* apData, DriverStation* apDS)
{
  //get data from driver
  //float lnXAxis = apDS->GetStickAxis(2, 1)*-1.0;
  /*float lnYAxis = apDS->GetStickAxis(2, 2)*-1.0;
  
  apData->mnUserDesiredShootingDistance += (lnYAxis
      *bhs_Constants::cnDUMPER_DIST_MULTIPLIER);*/
  /*apData->mnUserDesiredShootingAngle += (lnXAxis
      *bhs_Constants::cnDUMPER_ANGLE_MULTIPLIER);*/
  /*if (apData->mnUserDesiredShootingDistance > bhs_Constants::cnDUMPER_DIST_MAX)
    apData->mnUserDesiredShootingDistance = bhs_Constants::cnDUMPER_DIST_MAX;
  else if (apData->mnUserDesiredShootingDistance < bhs_Constants::cnDUMPER_DIST_MIN)
    apData->mnUserDesiredShootingDistance = bhs_Constants::cnDUMPER_DIST_MIN;*/
  
  /*if (apData->mnUserDesiredShootingAngle > bhs_Constants::cnDUMPER_ANGLE_MAX)
    apData->mnUserDesiredShootingAngle = bhs_Constants::cnDUMPER_ANGLE_MAX;
  else if (apData->mnUserDesiredShootingAngle < bhs_Constants::cnDUMPER_ANGLE_MIN)
    apData->mnUserDesiredShootingAngle = bhs_Constants::cnDUMPER_ANGLE_MIN;*/
}

void InputFormat::handleSecondJoystickButtons(RobotGlobalData* apData,
                                              DriverStation* apDS)
{
  short lnButtons = apDS->GetStickButtons(2);
  
  /*if (mcTurretAutomaticButton.risingEdge(lnButtons
      & bhs_Constants::TURRET_AUTO_AIM_BUTTON))
  {
    apData->meShooterState = RobotGlobalData::kShooterAutomatic;
  }
  
  if (apData->meShooterState == RobotGlobalData::kShooterManualActive)
  {
    if (mcTurretManualButton.fallingEdge(lnButtons
        & bhs_Constants::TURRET_MANUAL_AIM_BUTTON))
    {
      apData->meShooterState = RobotGlobalData::kShooterManualIdle;
    }
  }
  else //if automatic or manual idle
  {
    if (mcTurretManualButton.risingEdge(lnButtons
        & bhs_Constants::TURRET_MANUAL_AIM_BUTTON))
    {
      apData->meShooterState = RobotGlobalData::kShooterManualActive;
      apData->mbDumperSpinBackwards = false;
    }
  }*/
  
  /*if (mcDumperBackwardsButton.risingEdge(lnButtons
      & bhs_Constants::DUMPER_BACKWARDS_BUTTON))
  {
    apData->mbDumperSpinBackwards = !apData->mbDumperSpinBackwards;
  }*/
  
  if (mcCellLoaderForwardButton.risingEdge(lnButtons & bhs_Constants::CELL_LOADER_TAKE_IN_BUTTON))
  {
    apData->mnCellLoaderPower = bhs_Constants::cnCELL_LOADER_TAKE_IN_POWER;
  }
  if (mcCellLoaderReverseButton.risingEdge(lnButtons & bhs_Constants::CELL_LOADER_PUSH_OUT_BUTTON))
  {
    apData->mnCellLoaderPower = bhs_Constants::cnCELL_LOADER_PUSH_OUT_POWER;
  }
  if (mcCellLoaderOffButton.risingEdge(lnButtons & bhs_Constants::CELL_LOADER_OFF_BUTTON))
  {
    apData->mnCellLoaderPower = bhs_Constants::cnCELL_LOADER_OFF_POWER;
  }
  
  if (mcCellHolderOnButton.risingEdge(lnButtons & bhs_Constants::CELL_HOLDER_ON_BUTTON))
  {
    apData->meCellHolderState = bhs_Constants::cnCELL_HOLDER_ON_VALUE;
  }
  if (mcCellHolderOffButton.risingEdge(lnButtons & bhs_Constants::CELL_HOLDER_OFF_BUTTON))
  {
    apData->meCellHolderState = bhs_Constants::cnCELL_HOLDER_OFF_VALUE;
  }
  
  /*if (mcTurretLockButton.risingEdge(lnButtons & bhs_Constants::TURRET_LOCK_BUTTON))
  {
    apData->mbTurretLockOn = !apData->mbTurretLockOn;
  }*/
  
  
  /*bhs_TargetingData lcTargetingData;
  apData->getTargetingData(lcTargetingData);
  
  if (mcCameraZone1Button.risingEdge(lnButtons & bhs_Constants::CAMERA_ZONE_1_BUTTON))
  {
    if (lcTargetingData.mbCameraZone1Active && (lcTargetingData.mbCameraZone2Active
        || lcTargetingData.mbCameraZone3Active))
    {
      lcTargetingData.mbCameraZone1Active = false;
    }
    else if(!lcTargetingData.mbCameraZone1Active)
    {
      lcTargetingData.mbCameraZone1Active = true;
    }
  }
  
  if (mcCameraZone2Button.risingEdge(lnButtons & bhs_Constants::CAMERA_ZONE_2_BUTTON))
  {
    if (lcTargetingData.mbCameraZone2Active && (lcTargetingData.mbCameraZone1Active
            || lcTargetingData.mbCameraZone3Active))
    {
      lcTargetingData.mbCameraZone2Active = false;
    }
    else if(!lcTargetingData.mbCameraZone2Active)
    {
      lcTargetingData.mbCameraZone2Active = true;
    }
  }
  
  if (mcCameraZone3Button.risingEdge(lnButtons & bhs_Constants::CAMERA_ZONE_3_BUTTON))
  {
    if (lcTargetingData.mbCameraZone3Active && (lcTargetingData.mbCameraZone1Active
            || lcTargetingData.mbCameraZone2Active))
    {
      lcTargetingData.mbCameraZone3Active = false;
    }
    else if(!lcTargetingData.mbCameraZone3Active)
    {
      lcTargetingData.mbCameraZone3Active = true;
    }
  }
  
  apData->setTargetingData(lcTargetingData);*/
}

void InputFormat::handleDebugJoystickButtons(RobotGlobalData* apData,
                                             DriverStation* apDS)
{
  short lnButtons = apDS->GetStickButtons(4);
  
  /*if (mcDebugTurretButtons.risingEdge((lnButtons
      & bhs_Constants::DEBUG_TURRET_BUTTONS) == bhs_Constants::DEBUG_TURRET_BUTTONS))
  {
    apData->mbDebugTurret = !apData->mbDebugTurret;
  }*/
  if (mcDebugDumperButtons.risingEdge((lnButtons
      & bhs_Constants::DEBUG_DUMPER_BUTTONS)
      == bhs_Constants::DEBUG_DUMPER_BUTTONS))
  {
    apData->mbDebugDumper = !apData->mbDebugDumper;
  }
}
