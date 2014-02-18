#ifndef _ROBOTGLOBALDATA_H_
#define _ROBOTGLOBALDATA_H_

#include "WPILib.h"
#include "semLibCommon.h"
#include "bhs_Constants.h"
#include <set>

class bhs_HostLaptopData;

struct bhs_ProcessedTarget
{
    float mnAngle;
    float mnDistance;
    DriverStation::Alliance meTeam;
    bool mbValid;

    friend bool operator==(const bhs_ProcessedTarget& acFirst,
                           const bhs_ProcessedTarget& acSecond);
    friend bool operator>(const bhs_ProcessedTarget& acFirst,
                         const bhs_ProcessedTarget& acSecond);
    friend bool operator<(const bhs_ProcessedTarget& acFirst,
                          const bhs_ProcessedTarget& acSecond);

    bhs_ProcessedTarget()
    {
      mnAngle = 0;
      mnDistance = 0;
      meTeam = DriverStation::kInvalid;
      mbValid = false;
    }
};

struct bhs_TargetingData
{
    std::set<bhs_ProcessedTarget> mmTargets;
    float mnCurrentTargetDistance;
    float mnCurrentTargetAngle;
    bool mbCameraZone1Active;
    bool mbCameraZone2Active;
    bool mbCameraZone3Active;
};

struct bhs_CameraTurretData
{
    float mnCameraTurretTiltDegree;
    float mnCameraTurretPanDegree;
    float mnCameraTurretActualTiltDegree;
    float mnCameraTurretActualPanDegree;
};

class RobotGlobalData
{
    //nasty but neccessary
    friend class bhs_HostLaptopData;

  public:
    RobotGlobalData();
    ~RobotGlobalData();

    /**********DriverStation**********/
    float mnUserXaxis;
    float mnUserYaxis;
    float mnRawXaxis;
    float mnRawYaxis;
    float mnRawZaxis;
    float mnUserThrottle;
    double mnUserAngle;

    /**********DriveTrain************/
    //Status of the drive train - what mode it is functioning in
    enum DriveStatus
    { kAutomatic, kMotorsForward, kMotorsSideways, kVector, kVectorForwardHybrid, kVectorBackwardHybrid, kVectorLeftHybrid, kVectorRightHybrid};
    DriveStatus meDriveStatus;

    //Power for each real motor
    //See documentation to see what motors are where on the robot
    float mnMotorAPower;
    float mnMotorBPower;
    float mnMotorCPower;
    float mnMotorDPower;

    //Turret motor power
    //float mnTurretPower;

    //Power for each virtual motor
    float mnLeftMotorFront;
    float mnLeftMotorBack;
    float mnRightMotorFront;
    float mnRightMotorBack;

    static const bool gbMotorFacingForward = false;
    static const bool gbMotorFacingSideways = true;

    bool mbPhysicalMotorAFacing;
    bool mbPhysicalMotorBFacing;
    bool mbPhysicalMotorCFacing;
    bool mbPhysicalMotorDFacing;

    //Raw values from Accelerometer and Gyros
    float mnAccelerometerXValue;
    float mnAccelerometerYValue;
    double mnGyroValue;

    //Pneumatic air pressure sensor
    //0=below limit, else above limit
    int mnAirPressure;
    bool mbRunCompressor;

    //Position, acceleration, and orientation from accelerometer and gyro
    float mnRobotPositionX;
    float mnRobotPositionY;
    float mnRobotSpeed;
    float mnRobotDirection;

    //Shooter desired values
    float mnUserDesiredShootingDistance;
    float mnUserDesiredShootingAngle;

    //Current shooter position
    float mnActualShootingDistance;
    float mnActualShootingAngle;
    float mnPOTRawValue;

    float mnCellLoaderPower;
    bool mbDumperSpinBackwards;

    Relay::Value meConveyorState;
    Relay::Value meCellFeederState;
    Relay::Value meCellHolderState;
    
    //bool mbTurretLockOn;

    enum ShooterState
    { kShooterManualActive, kShooterManualIdle, kShooterAutomatic};
    ShooterState meShooterState;

    //Current goals for the turret and shooter
    //Used for aiming the shooter and setting the speed of the shooter
    float mnDesiredShootingAngle;
    float mnDesiredShootingDistance;
    
    //Debug Modes; activated by Joystick 4
    //bool mbDebugTurret;
    bool mbDebugDumper;
    
    //Autonomous switch state.  Only set when autonomous is initialized
    bool mbAutonomousSwitchState;

    void getCameraTurretData(bhs_CameraTurretData &acCameraTurretData);
    void setCameraTurretData(bhs_CameraTurretData acCameraTurretData);

    void getTargetingData(bhs_TargetingData &acTargetingData);
    void setTargetingData(bhs_TargetingData acTargetingData);

  protected:
    float mnCameraTurretTiltDegree;
    float mnCameraTurretPanDegree;
    float mnCameraTurretActualTiltDegree;
    float mnCameraTurretActualPanDegree;

    //Camera image report after processing
    std::set<bhs_ProcessedTarget> mmTargets;

    //Chosen target data
    float mnCurrentTargetDistance;
    float mnCurrentTargetAngle;
    bool mbCameraZone1Active;
    bool mbCameraZone2Active;
    bool mbCameraZone3Active;

  private:
    SEM_ID mcTurretSem;
    SEM_ID mcTargetingSem;

    void lockAllData();
    void releaseAllData();
};

#endif
