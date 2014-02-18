#ifndef _BHS_HOSTLAPTOPDATA_
#define _BHS_HOSTLAPTOPDATA_

#include "DriverStation.h"
#include "RobotGlobalData.h"

struct bhs_ProcessedTargetLaptop
{
    float mnAngle;
    float mnDistance;
    long mnTeam;
    long mnValidity;
};

struct bhs_HostLaptopData
{
    long mnUserDataMarker1;
    long mnUserDataMarker2;

    float mnUserXaxis;
    float mnUserYaxis;
    float mnUserAngle;
    float meDriveStatus;
    float mnMotorAPower;
    float mnMotorBPower;
    float mnMotorCPower;
    float mnMotorDPower;

    float mnLeftMotorFront;
    float mnLeftMotorBack;
    float mnRightMotorFront;
    float mnRightMotorBack;

    long mbPhysicalMotorAFacing;
    long mbPhysicalMotorBFacing;
    long mbPhysicalMotorCFacing;
    long mbPhysicalMotorDFacing;

    float mnCameraTurretTiltDegree;
    float mnCameraTurretPanDegree;

    float mnCameraTurretActualTiltDegree;
    float mnCameraTurretActualPanDegree;

    float mnAccelerometerXValue;
    float mnAccelerometerYValue;
    float mnGyroValue;

    float mnAirPressure;
    float mnRunCompressor;

    float mnRobotPositionX;
    float mnRobotPositionY;
    float mnRobotSpeed;
    float mnRobotDirection;

    //joystick 1-4
    float mnJoystick1X;
    float mnJoystick1Y;
    long mnJoystick1Buttons;
    float mnJoystick2X;
    float mnJoystick2Y;
    long mnJoystick2Buttons;
    float mnJoystick3X;
    float mnJoystick3Y;
    long mnJoystick3Buttons;
    float mnJoystick4X;
    float mnJoystick4Y;
    long mnJoystick4Buttons;

    //Driverstation inputs
    float mnAnalogIn1;
    float mnAnalogIn2;
    float mnAnalogIn3;
    float mnAnalogIn4;
    long mnDigitalIn1;
    long mnDigitalIn2;
    long mnDigitalIn3;
    long mnDigitalIn4;
    long mnDigitalIn5;
    long mnDigitalIn6;
    long mnDigitalIn7;
    long mnDigitalIn8;

    bhs_ProcessedTargetLaptop maTargets[bhs_Constants::cnMAX_TARGET_REPORTS];

    float mnUserDesiredShootingDistance;
    float mnUserDesiredShootingAngle;
    
    long mnCellLoaderState;
    
    float mnActualShootingDistance;
    float mnActualShootingAngle;
    
    float mnDesiredShootingDistance;
    float mnDesiredShootingAngle;
    
    float mnCurrentTargetDistance;
    float mnCurrentTargetAngle;
    
    float mnBatteryVoltage;
    
    long mnCameraZone1Active;
    long mnCameraZone2Active;
    long mnCameraZone3Active;
    
    long mnAllianceColor;
    
    float mnCellLoaderPower;
    
    long mbAutonomousSwitchState;

    bhs_HostLaptopData();
    
    void setAllData(RobotGlobalData *apData, DriverStation *apDS);

  private:
    void getJoystickValues(RobotGlobalData *apData);
    void getMotorValues(RobotGlobalData *apData);
    void getSensorValues(RobotGlobalData *apData);
    void getProcessedTargets(RobotGlobalData *apData);
    void getAllRoboticValues(RobotGlobalData *apData);
    void getAllJoystickValues(DriverStation *apData);
};

#endif
