#ifndef _INPUTFORMAT_H_
#define _INPUTFORMAT_H_

#include "DriverStation.h"
#include "RobotGlobalData.h"
#include "DigitalInput.h"
#include "bhs_SensorPlatform.h"
#include "bhs_EdgeDetector.h"
#include "TargetPotentiometer.h"

class InputFormat
{
  public:
    InputFormat();
    void Initialize(Servo* apPanServo, Servo* apTiltServo);
    void handleOperatorInputs(RobotGlobalData* apData, DriverStation* apDS, bhs_SensorPlatform* apSensorPlatform);
    void handleSensorInputs(RobotGlobalData* apData, bhs_SensorPlatform* apSensorPlatform);

  private:
    float ConfigJoystickDriveDeadZone(float anDeadZone);
    void driveData(RobotGlobalData* apData, DriverStation* apDS);
    void handleFirstJoystickButtons(RobotGlobalData* apData, DriverStation* apDS);
    void handlePneumaticInputs(RobotGlobalData* apData/*, DriverStation* apDS*/);
    void handleAnalogInputs(RobotGlobalData* apData, bhs_SensorPlatform* apSensors);
    void manualShooting(RobotGlobalData* apData, DriverStation* apDS);
    void handleSecondJoystickButtons(RobotGlobalData* apData, DriverStation* apDS);
    void handleDebugJoystickButtons(RobotGlobalData* apData, DriverStation* apDS);

    float mnDeadZone;
    float mnUserXaxis;
    float mnUserYaxis;

    bhs_EdgeDetector mcTwitchForwardButton;
    bhs_EdgeDetector mcTwitchSidewaysButton;
    bhs_EdgeDetector mcTwitchAutoButton;
    bhs_EdgeDetector mcTwitchVectorButton;
    bhs_EdgeDetector mcTwitchVectorHybridForwardButton;
    bhs_EdgeDetector mcTwitchVectorHybridBackwardButton;
    bhs_EdgeDetector mcTwitchVectorHybridLeftButton;
    bhs_EdgeDetector mcTwitchVectorHybridRightButton;
    //bhs_EdgeDetector mcCompressorButton;
    bhs_EdgeDetector mcTurretManualButton;
    bhs_EdgeDetector mcTurretAutomaticButton;
    //bhs_EdgeDetector mcDumperBackwardsButton;
    bhs_EdgeDetector mcCellLoaderForwardButton;
    bhs_EdgeDetector mcCellLoaderReverseButton;
    bhs_EdgeDetector mcCellLoaderOffButton;
    bhs_EdgeDetector mcCellFeederForwardButton;
    bhs_EdgeDetector mcCellFeederReverseButton;
    bhs_EdgeDetector mcCellFeederOffButton;
    //bhs_EdgeDetector mcTurretLockButton;
    bhs_EdgeDetector mcCellHolderOnButton;
    bhs_EdgeDetector mcCellHolderOffButton;
    /*bhs_EdgeDetector mcCameraZone1Button;
    bhs_EdgeDetector mcCameraZone2Button;
    bhs_EdgeDetector mcCameraZone3Button;*/
    //bhs_EdgeDetector mcDebugTurretButtons;
    bhs_EdgeDetector mcDebugDumperButtons;

    Servo *mpPanServo;
    Servo *mpTiltServo;
    
    DigitalInput* mpAirPressureSensor;
    
    //TargetPotentiometer* mpTargetPOT;
};

#endif
