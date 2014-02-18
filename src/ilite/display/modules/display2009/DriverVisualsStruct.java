package ilite.display.modules.display2009;

public class DriverVisualsStruct
{
  float[] angle = new float[15];
  float[] distance = new float[15];
  int[] redOrBlue = new int[15];
  int[] validity = new int[15];
   
  float launcherAngle;
  float launcherDistance;
  
  float meDriveStatus;
  
  int mbPhysicalMotorAFacing;
  int mbPhysicalMotorBFacing;
  int mbPhysicalMotorCFacing;
  int mbPhysicalMotorDFacing;
  
  float mnAirPressure;
  float mbRunCompressor;
  
  float mnRobotSpeed;
  float mnRobotDirection;
  
  float mnActualShootingDistance;
  float mnActualShootingAngle;
  
  float mnDesiredShootingAngle;
  float mnDesiredShootingDistance;
  
  float mnCurrentTargetDistance;
  float mnCurrentTargetAngle;
  
  float mnBattery;
  
  int mnCameraZone1Active;
  int mnCameraZone2Active;
  int mnCameraZone3Active;
  
  int mnTeamColor;
  
  int mnConveyorOn;
  int mnFeederOpen;
  
  int mnTurretLocked;
}