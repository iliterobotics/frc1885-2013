package ilite.display.modules.display2009;

import java.nio.ByteBuffer;

public class UDPDataInterpreter
{
  
  TargetViewDataStruct mcTargetViewStruct;
  RobotStateDataStruct mcRobotStateStruct;
  DriverVisualsStruct mcDriverVisualsStruct;

  DriverVisuals mcDriverVisuals;
  TargetView mcTargetView;
  RobotStateManager mcRobotStateManager;
  
  public UDPDataInterpreter(TargetView acTargetView, RobotStateManager acRobotStateManager, DriverVisuals acDriverVisuals)
  {
    mcTargetViewStruct = new TargetViewDataStruct();
    mcRobotStateStruct = new RobotStateDataStruct();
    mcDriverVisualsStruct = new DriverVisualsStruct();
    
    mcTargetView = acTargetView;
    mcRobotStateManager = acRobotStateManager;
    mcDriverVisuals = acDriverVisuals;
  }  
    
  public void interpretData(ByteBuffer acBuffer)
  {
    
    mcRobotStateStruct.mnUserDataMarker1 = acBuffer.getInt();
    mcRobotStateStruct.mnUserDataMarker2 = acBuffer.getInt();
    
    mcRobotStateStruct.mnUserXaxis = acBuffer.getFloat();
    mcRobotStateStruct.mnUserYaxis = acBuffer.getFloat();
    mcRobotStateStruct.mnUserAngle = acBuffer.getFloat();
    mcRobotStateStruct.meDriveStatus = acBuffer.getFloat();
    mcDriverVisualsStruct.meDriveStatus = mcRobotStateStruct.meDriveStatus;
    
    mcRobotStateStruct.mnMotorAPower = acBuffer.getFloat();
    mcRobotStateStruct.mnMotorBPower = acBuffer.getFloat();
    mcRobotStateStruct.mnMotorCPower = acBuffer.getFloat();
    mcRobotStateStruct.mnMotorDPower = acBuffer.getFloat();
    
    mcRobotStateStruct.mnLeftMotorFront = acBuffer.getFloat();
    mcRobotStateStruct.mnLeftMotorBack = acBuffer.getFloat();
    mcRobotStateStruct.mnRightMotorFront = acBuffer.getFloat();
    mcRobotStateStruct.mnRightMotorBack = acBuffer.getFloat();
    
    mcRobotStateStruct.mbPhysicalMotorAFacing = acBuffer.getInt();
    mcDriverVisualsStruct.mbPhysicalMotorAFacing = mcRobotStateStruct.mbPhysicalMotorAFacing;
    mcRobotStateStruct.mbPhysicalMotorBFacing = acBuffer.getInt();
    mcDriverVisualsStruct.mbPhysicalMotorBFacing = mcRobotStateStruct.mbPhysicalMotorBFacing;
    mcRobotStateStruct.mbPhysicalMotorCFacing = acBuffer.getInt();
    mcDriverVisualsStruct.mbPhysicalMotorCFacing = mcRobotStateStruct.mbPhysicalMotorCFacing;
    mcRobotStateStruct.mbPhysicalMotorDFacing = acBuffer.getInt();
    mcDriverVisualsStruct.mbPhysicalMotorDFacing = mcRobotStateStruct.mbPhysicalMotorDFacing;
    
    mcRobotStateStruct.mnCameraTurretTiltDegree = acBuffer.getFloat();
    mcRobotStateStruct.mnCameraTurretPanDegree = acBuffer.getFloat();
    
    mcRobotStateStruct.mnCameraTurretActualTiltDegree = acBuffer.getFloat();
    mcRobotStateStruct.mnCameraTurretActualPanDegree = acBuffer.getFloat();
    
    mcRobotStateStruct.mnAccelerometerXValue = acBuffer.getFloat();
    mcRobotStateStruct.mnAccelerometerYValue = acBuffer.getFloat();
    mcRobotStateStruct.mnGyroValue = acBuffer.getFloat();
    
    mcRobotStateStruct.mnAirPressure = acBuffer.getFloat();
    mcDriverVisualsStruct.mnAirPressure = mcRobotStateStruct.mnAirPressure;
    mcRobotStateStruct.mbRunCompressor = acBuffer.getFloat();
    mcDriverVisualsStruct.mbRunCompressor = mcRobotStateStruct.mbRunCompressor;
    
    mcRobotStateStruct.mnRobotPositionX = acBuffer.getFloat();
    mcRobotStateStruct.mnRobotPositionY = acBuffer.getFloat();
    mcRobotStateStruct.mnRobotSpeed = acBuffer.getFloat();
    mcDriverVisualsStruct.mnRobotSpeed = mcRobotStateStruct.mnRobotSpeed;
    mcRobotStateStruct.mnRobotDirection = acBuffer.getFloat();
    mcDriverVisualsStruct.mnRobotDirection = mcRobotStateStruct.mnRobotDirection;
    
    mcRobotStateStruct.mnJoystick1X = acBuffer.getFloat();
    mcRobotStateStruct.mnJoystick1Y = acBuffer.getFloat();
    mcRobotStateStruct.mnJoystick1Buttons = acBuffer.getInt();
    mcRobotStateStruct.mnJoystick2X = acBuffer.getFloat();
    mcRobotStateStruct.mnJoystick2Y = acBuffer.getFloat();
    mcRobotStateStruct.mnJoystick2Buttons = acBuffer.getInt();
    
    mcRobotStateStruct.mnJoystick3X = acBuffer.getFloat();
    mcRobotStateStruct.mnJoystick3Y = acBuffer.getFloat();
    mcRobotStateStruct.mnJoystick3Buttons = acBuffer.getInt();
    mcRobotStateStruct.mnJoystick4X = acBuffer.getFloat();
    mcRobotStateStruct.mnJoystick4Y = acBuffer.getFloat();
    mcRobotStateStruct.mnJoystick4Buttons = acBuffer.getInt();
    
    mcRobotStateStruct.mnAnalogIn1 = acBuffer.getInt();
    mcRobotStateStruct.mnAnalogIn2 = acBuffer.getInt();
    mcRobotStateStruct.mnAnalogIn3 = acBuffer.getInt();
    mcRobotStateStruct.mnAnalogIn4 = acBuffer.getInt();
    mcRobotStateStruct.mnDigitalIn1 = acBuffer.getInt();
    mcRobotStateStruct.mnDigitalIn2 = acBuffer.getInt();
    mcRobotStateStruct.mnDigitalIn3 = acBuffer.getInt();
    mcRobotStateStruct.mnDigitalIn4 = acBuffer.getInt();
    mcRobotStateStruct.mnDigitalIn5 = acBuffer.getInt();
    mcRobotStateStruct.mnDigitalIn6 = acBuffer.getInt();
    mcRobotStateStruct.mnDigitalIn7 = acBuffer.getInt();
    mcRobotStateStruct.mnDigitalIn8 = acBuffer.getInt();   
      
    for(int i=0;i<15;i++)
    {
      mcTargetViewStruct.angle[i] = acBuffer.getFloat();
      mcTargetViewStruct.distance[i] = acBuffer.getFloat(); // Distance comes in in inches
      mcTargetViewStruct.redOrBlue[i] = acBuffer.getInt();
      mcTargetViewStruct.validity[i] = acBuffer.getInt();
    }
    
    for(int i=0;i<15;i++)
    {
      mcDriverVisualsStruct.angle[i] = mcTargetViewStruct.angle[i];
      mcDriverVisualsStruct.distance[i] = mcTargetViewStruct.distance[i];
      mcDriverVisualsStruct.redOrBlue[i] = mcTargetViewStruct.redOrBlue[i];
      mcDriverVisualsStruct.validity[i] = mcTargetViewStruct.validity[i];
    }
    
    
    mcTargetViewStruct.launcherDistance = acBuffer.getFloat(); // Distance comes in in inches
    mcDriverVisualsStruct.launcherDistance = mcTargetViewStruct.launcherDistance;
    mcTargetViewStruct.launcherAngle = acBuffer.getFloat();
    mcDriverVisualsStruct.launcherAngle = mcTargetViewStruct.launcherAngle; 
      
    mcDriverVisualsStruct.mnConveyorOn = acBuffer.getInt();
    
    mcDriverVisualsStruct.mnActualShootingDistance = acBuffer.getFloat();
    mcDriverVisualsStruct.mnActualShootingAngle = acBuffer.getFloat();
    
    mcDriverVisualsStruct.mnDesiredShootingDistance = acBuffer.getFloat();
    mcDriverVisualsStruct.mnDesiredShootingAngle = acBuffer.getFloat();
    
    mcDriverVisualsStruct.mnCurrentTargetDistance = acBuffer.getFloat();
    mcDriverVisualsStruct.mnCurrentTargetAngle = acBuffer.getFloat();
    
    mcDriverVisualsStruct.mnBattery = acBuffer.getFloat();
    
    mcDriverVisualsStruct.mnCameraZone1Active = acBuffer.getInt();
    mcDriverVisualsStruct.mnCameraZone2Active = acBuffer.getInt();
    mcDriverVisualsStruct.mnCameraZone3Active = acBuffer.getInt();
    
    mcDriverVisualsStruct.mnTeamColor = acBuffer.getInt();
    
    mcDriverVisualsStruct.mnFeederOpen = acBuffer.getInt();
    mcDriverVisualsStruct.mnTurretLocked = acBuffer.getInt();
      
    mcTargetView.setData(mcTargetViewStruct);
    mcRobotStateManager.setData(mcRobotStateStruct);
    mcDriverVisuals.setData(mcDriverVisualsStruct);
  }
}