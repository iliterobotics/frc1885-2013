package ilite.display.modules.display2009;

import java.awt.Component;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import javax.swing.JComponent;

public class RobotStateManager
{
  private JComponent mcRobotStatePanel;
  
  BHSCustomLabel mnUserDataMarker1;
  BHSCustomLabel mnUserDataMarker2;
  
  BHSCustomLabel mnUserXaxis;
  BHSCustomLabel mnUserYaxis;
  BHSCustomLabel mnUserAngle;
  BHSCustomLabel meDriveStatus;
  BHSCustomLabel mnMotorAPower;
  BHSCustomLabel mnMotorBPower;
  BHSCustomLabel mnMotorCPower;
  BHSCustomLabel mnMotorDPower;
  
  BHSCustomLabel mnLeftMotorFront;
  BHSCustomLabel mnLeftMotorBack;
  BHSCustomLabel mnRightMotorFront;
  BHSCustomLabel mnRightMotorBack;
  
  BHSCustomLabel mbPhysicalMotorAFacing;
  BHSCustomLabel mbPhysicalMotorBFacing;
  BHSCustomLabel mbPhysicalMotorCFacing;
  BHSCustomLabel mbPhysicalMotorDFacing;
  
  BHSCustomLabel mnCameraTurretTiltDegree;
  BHSCustomLabel mnCameraTurretPanDegree;
  
  BHSCustomLabel mnCameraTurretActualTiltDegree;
  BHSCustomLabel mnCameraTurretActualPanDegree;
  
  BHSCustomLabel mnAccelerometerXValue;
  BHSCustomLabel mnAccelerometerYValue;
  BHSCustomLabel mnGyroValue;
  
  BHSCustomLabel mnAirPressure;
  BHSCustomLabel mbRunCompressor;
  
  BHSCustomLabel mnRobotPositionX;
  BHSCustomLabel mnRobotPositionY;
  BHSCustomLabel mnRobotSpeed;
  BHSCustomLabel mnRobotDirection;
  
  BHSCustomLabel mnJoystick1X;
  BHSCustomLabel mnJoystick1Y;
  BHSCustomLabel mnJoystick1Buttons;
  BHSCustomLabel mnJoystick2X;
  BHSCustomLabel mnJoystick2Y;
  BHSCustomLabel mnJoystick2Buttons;
  
  BHSCustomLabel mnJoystick3X;
  BHSCustomLabel mnJoystick3Y;
  BHSCustomLabel mnJoystick3Buttons;
  BHSCustomLabel mnJoystick4X;
  BHSCustomLabel mnJoystick4Y;
  BHSCustomLabel mnJoystick4Buttons;
  
  BHSCustomLabel mnAnalogIn1;
  BHSCustomLabel mnAnalogIn2;
  BHSCustomLabel mnAnalogIn3;
  BHSCustomLabel mnAnalogIn4;
  BHSCustomLabel mnDigitalIn1;
  BHSCustomLabel mnDigitalIn2;
  BHSCustomLabel mnDigitalIn3;
  BHSCustomLabel mnDigitalIn4;
  BHSCustomLabel mnDigitalIn5;
  BHSCustomLabel mnDigitalIn6;
  BHSCustomLabel mnDigitalIn7;
  BHSCustomLabel mnDigitalIn8;
  
  protected GridBagLayout mcGblRobotStatePanel;
  protected GridBagConstraints mcGbc;
  
  public RobotStateManager(JComponent acPanel)
  {
    mcRobotStatePanel=acPanel;
    
    mcGblRobotStatePanel = new GridBagLayout();
    mcGbc = new GridBagConstraints();
    
    mcGbc.weightx = 100;
    mcGbc.weighty = 100;
    mcGbc.insets.bottom = 0;
    mcGbc.insets.left = 5;
    mcGbc.insets.right = 5;
    mcGbc.insets.top = 0;
    mcGbc.fill = GridBagConstraints.NONE;
    mcGbc.anchor = GridBagConstraints.NORTHWEST;
    
    mcRobotStatePanel.setLayout(mcGblRobotStatePanel);
    
    mnUserDataMarker1 = new BHSCustomLabel("User Data Marker 1: ");
    mnUserDataMarker2 = new BHSCustomLabel("User Data Marker 2: ");
    
    mnUserXaxis = new BHSCustomLabel("User X Axis: ");
    mnUserYaxis = new BHSCustomLabel("User Y Axis: ");
    mnUserAngle = new BHSCustomLabel("User Angle: ");
    meDriveStatus = new BHSCustomLabel("Drive Status: ");
    mnMotorAPower = new BHSCustomLabel("Motor A Power: ");
    mnMotorBPower = new BHSCustomLabel("Motor B Power: ");
    mnMotorCPower = new BHSCustomLabel("Motor B Power: ");
    mnMotorDPower = new BHSCustomLabel("Motor B Power: ");
    
    mnLeftMotorFront = new BHSCustomLabel("Left Motor Front: ");
    mnLeftMotorBack = new BHSCustomLabel("Left Motor Back: ");
    mnRightMotorFront = new BHSCustomLabel("Right Motor Front: ");
    mnRightMotorBack = new BHSCustomLabel("Right Motot Back: ");
    
    mbPhysicalMotorAFacing = new BHSCustomLabel("Physical Motor A Facing: ");
    mbPhysicalMotorBFacing = new BHSCustomLabel("Physical Motor B Facing: ");
    mbPhysicalMotorCFacing = new BHSCustomLabel("Physical Motor C Facing: ");
    mbPhysicalMotorDFacing = new BHSCustomLabel("Physical Motor D Facing: ");
    
    mnCameraTurretTiltDegree = new BHSCustomLabel("Camera Turret Tilt Degree: ");
    mnCameraTurretPanDegree = new BHSCustomLabel("Camera Turret Pan Degree: ");
    
    mnCameraTurretActualTiltDegree = new BHSCustomLabel("Camera Turret Actual Tilt Degree: ");
    mnCameraTurretActualPanDegree = new BHSCustomLabel("Camera Turret Actual Pan Degree: ");
    
    mnAccelerometerXValue = new BHSCustomLabel("Accelerometer X Value: ");
    mnAccelerometerYValue = new BHSCustomLabel("Accelerometer Y Value: ");
    mnGyroValue = new BHSCustomLabel("Gyro Value: ");
    
    
    mnAirPressure = new BHSCustomLabel("Air Pressure: ");
    mbRunCompressor = new BHSCustomLabel("Run Compressor: ");
    
    mnRobotPositionX = new BHSCustomLabel("Robot Position X: ");
    mnRobotPositionY = new BHSCustomLabel("Robot Position Y: ");
    mnRobotSpeed = new BHSCustomLabel("Robot Speed: ");
    mnRobotDirection = new BHSCustomLabel("Robot Direction: ");
    
    
    mnJoystick1X = new BHSCustomLabel("Joystick 1 X: ");
    mnJoystick1Y = new BHSCustomLabel("Joystick 1 Y: ");
    mnJoystick1Buttons = new BHSCustomLabel("Joystick 1 Buttons: ");
    mnJoystick2X = new BHSCustomLabel("Joystick 2 X: ");
    mnJoystick2Y = new BHSCustomLabel("Joystick 2 Y: ");
    mnJoystick2Buttons = new BHSCustomLabel("Joystick 2 Buttons: ");
    
    mnJoystick3X = new BHSCustomLabel("Joystick 3 X: ");
    mnJoystick3Y = new BHSCustomLabel("Joystick 3 Y: ");
    mnJoystick3Buttons = new BHSCustomLabel("Joystick 3 Buttons: ");
    
    mnJoystick4X = new BHSCustomLabel("Joystick 4 X: ");
    mnJoystick4Y = new BHSCustomLabel("Joystick 4 Y: ");
    mnJoystick4Buttons = new BHSCustomLabel("Joystick 4 Buttons: ");
    
    mnAnalogIn1 = new BHSCustomLabel("Analog in 1: ");
    mnAnalogIn2 = new BHSCustomLabel("Analog in 2: ");
    mnAnalogIn3 = new BHSCustomLabel("Analog in 3: ");
    mnAnalogIn4 = new BHSCustomLabel("Analog in 4: ");
    mnDigitalIn1 = new BHSCustomLabel("Digital in 1: ");
    mnDigitalIn2 = new BHSCustomLabel("Digital in 2: ");
    mnDigitalIn3 = new BHSCustomLabel("Digital in 3: ");
    mnDigitalIn4 = new BHSCustomLabel("Digital in 4: ");
    mnDigitalIn5 = new BHSCustomLabel("Digital in 5: ");
    mnDigitalIn6 = new BHSCustomLabel("Digital in 6: ");
    mnDigitalIn7 = new BHSCustomLabel("Digital in 7: ");
    mnDigitalIn8 = new BHSCustomLabel("Digital in 8: ");
    
    addRobotStatePanel(mnUserDataMarker1, 1, 1, 1, 1);
    addRobotStatePanel(mnUserDataMarker2, 2, 1, 1, 1);
    
    addRobotStatePanel(mnUserXaxis, 4, 1, 1, 1);
    addRobotStatePanel(mnUserYaxis, 5, 1, 1, 1);
    addRobotStatePanel(mnUserAngle, 6, 1, 1, 1);
  
    addRobotStatePanel(meDriveStatus, 8, 1, 1, 1);
    addRobotStatePanel(mnMotorAPower, 9, 1, 1, 1);
    addRobotStatePanel(mnMotorBPower, 10, 1, 1, 1);
    addRobotStatePanel(mnMotorCPower, 11, 1, 1, 1);
    addRobotStatePanel(mnMotorDPower, 12, 1, 1, 1);
    
    addRobotStatePanel(mnLeftMotorFront, 14, 1, 1, 1);
    addRobotStatePanel(mnLeftMotorBack, 15, 1, 1, 1);
    addRobotStatePanel(mnRightMotorFront, 16, 1, 1, 1);
    addRobotStatePanel(mnRightMotorBack, 17, 1, 1, 1);
    
    addRobotStatePanel(mbPhysicalMotorAFacing, 1, 2, 1, 1);
    addRobotStatePanel(mbPhysicalMotorBFacing, 2, 2, 1, 1);
    addRobotStatePanel(mbPhysicalMotorCFacing, 3, 2, 1, 1);
    addRobotStatePanel(mbPhysicalMotorDFacing, 4, 2, 1, 1);
    
    addRobotStatePanel(mnCameraTurretTiltDegree, 6, 2, 1, 1);
    addRobotStatePanel(mnCameraTurretPanDegree, 7, 2, 1, 1);
    
    addRobotStatePanel(mnCameraTurretActualTiltDegree, 9, 2, 1, 1);
    addRobotStatePanel(mnCameraTurretActualPanDegree, 10, 2, 1, 1);
    
    addRobotStatePanel(mnAccelerometerXValue, 12, 2, 1, 1);
    addRobotStatePanel(mnAccelerometerYValue, 13, 2, 1, 1);
    addRobotStatePanel(mnGyroValue, 14, 2, 1, 1);
    
    addRobotStatePanel(mnAirPressure, 16, 2, 1, 1);
    addRobotStatePanel(mbRunCompressor, 17, 2, 1, 1);
    
    addRobotStatePanel(mnRobotPositionX, 1, 3, 1, 1);
    addRobotStatePanel(mnRobotPositionY, 2, 3, 1, 1);
    addRobotStatePanel(mnRobotSpeed, 3, 3, 1, 1);
    addRobotStatePanel(mnRobotDirection, 4, 3, 1, 1);
    
    addRobotStatePanel(mnJoystick1X, 6, 3, 1, 1);
    addRobotStatePanel(mnJoystick1Y, 7, 3, 1, 1);
    addRobotStatePanel(mnJoystick1Buttons, 8, 3, 1, 1);
    
    addRobotStatePanel(mnJoystick2X, 10, 3, 1, 1);
    addRobotStatePanel(mnJoystick2Y, 11, 3, 1, 1);
    addRobotStatePanel(mnJoystick2Buttons, 12, 3, 1, 1);
    
    addRobotStatePanel(mnJoystick3X, 14, 3, 1, 1);
    addRobotStatePanel(mnJoystick3Y, 15, 3, 1, 1);
    addRobotStatePanel(mnJoystick3Buttons, 16, 3, 1, 1);
    
    addRobotStatePanel(mnJoystick4X, 1, 4, 1, 1);
    addRobotStatePanel(mnJoystick4Y, 2, 4, 1, 1);
    addRobotStatePanel(mnJoystick4Buttons, 3, 4, 1, 1);
    
    addRobotStatePanel(mnAnalogIn1, 5, 4, 1, 1);
    addRobotStatePanel(mnAnalogIn2, 6, 4, 1, 1);
    addRobotStatePanel(mnAnalogIn3, 7, 4, 1, 1);
    addRobotStatePanel(mnAnalogIn4, 8, 4, 1, 1);
  
    addRobotStatePanel(mnDigitalIn1, 10, 4, 1, 1);
    addRobotStatePanel(mnDigitalIn2, 11, 4, 1, 1);
    addRobotStatePanel(mnDigitalIn3, 12, 4, 1, 1);
    addRobotStatePanel(mnDigitalIn4, 13, 4, 1, 1);
  
    addRobotStatePanel(mnDigitalIn5, 15, 4, 1, 1);
    addRobotStatePanel(mnDigitalIn6, 16, 4, 1, 1);
    addRobotStatePanel(mnDigitalIn7, 17, 4, 1, 1);
    addRobotStatePanel(mnDigitalIn8, 18, 4, 1, 1);
    
    
  }
  
  protected void addRobotStatePanel(Component c, int y, int x, int w, int h)
  {
    //setup constraints
    mcGbc.gridx = x - 1;
    mcGbc.gridy = y - 1;
    mcGbc.gridwidth = w;
    mcGbc.gridheight = h;
    // apply constraints to layout
    mcGblRobotStatePanel.setConstraints(c, mcGbc);
    // add new element to Panel
    mcRobotStatePanel.add(c);
  }
  
  public void setData(RobotStateDataStruct acData)
  {
    mnUserDataMarker1.update(acData.mnUserDataMarker1);
    mnUserDataMarker2.update(acData.mnUserDataMarker2);
    
    mnUserXaxis.update(acData.mnUserXaxis);
    mnUserYaxis.update(acData.mnUserYaxis);
    mnUserAngle.update(acData.mnUserAngle);
    
    meDriveStatus.update(acData.meDriveStatus);
    mnMotorAPower.update(acData.mnMotorAPower);
    mnMotorBPower.update(acData.mnMotorBPower);
    mnMotorCPower.update(acData.mnMotorCPower);
    mnMotorDPower.update(acData.mnMotorDPower);
    
    mnLeftMotorFront.update(acData.mnLeftMotorFront);
    mnLeftMotorBack.update(acData.mnLeftMotorBack);
    mnRightMotorFront.update(acData.mnRightMotorFront);
    mnRightMotorBack.update(acData.mnRightMotorBack);
    
    mbPhysicalMotorAFacing.update(acData.mbPhysicalMotorAFacing);
    mbPhysicalMotorBFacing.update(acData.mbPhysicalMotorBFacing);
    mbPhysicalMotorCFacing.update(acData.mbPhysicalMotorCFacing);
    mbPhysicalMotorDFacing.update(acData.mbPhysicalMotorDFacing);
    
    mnCameraTurretTiltDegree.update(acData.mnCameraTurretTiltDegree);
    mnCameraTurretPanDegree.update(acData.mnCameraTurretPanDegree);
    
    mnCameraTurretActualTiltDegree.update(acData.mnCameraTurretActualTiltDegree);
    mnCameraTurretActualPanDegree.update(acData.mnCameraTurretActualPanDegree);
    
    mnAccelerometerXValue.update(acData.mnAccelerometerXValue);
    mnAccelerometerYValue.update(acData.mnAccelerometerYValue);
    mnGyroValue.update(acData.mnGyroValue);
    
    mnAirPressure.update(acData.mnAirPressure);
    mbRunCompressor.update(acData.mbRunCompressor);
    
    mnRobotPositionX.update(acData.mnRobotPositionX);
    mnRobotPositionY.update(acData.mnRobotPositionY);
    mnRobotSpeed.update(acData.mnRobotSpeed);
    mnRobotDirection.update(acData.mnRobotDirection);
    
    mnJoystick1X.update(acData.mnJoystick1X);
    mnJoystick1Y.update(acData.mnJoystick1Y);
    mnJoystick1Buttons.update(acData.mnJoystick1Buttons);
    mnJoystick2X.update(acData.mnJoystick2X);
    mnJoystick2Y.update(acData.mnJoystick2Y);
    mnJoystick2Buttons.update(acData.mnJoystick2Buttons);
    
    mnJoystick3X.update(acData.mnJoystick3X);
    mnJoystick3Y.update(acData.mnJoystick3Y);
    mnJoystick3Buttons.update(acData.mnJoystick3Buttons);
    mnJoystick4X.update(acData.mnJoystick4X);
    mnJoystick4Y.update(acData.mnJoystick4Y);
    mnJoystick4Buttons.update(acData.mnJoystick4Buttons);
    
    mnAnalogIn1.update(acData.mnAnalogIn1);
    mnAnalogIn2.update(acData.mnAnalogIn2);
    mnAnalogIn3.update(acData.mnAnalogIn3);
    mnAnalogIn4.update(acData.mnAnalogIn4);
    
    mnDigitalIn1.update(acData.mnDigitalIn1);
    mnDigitalIn2.update(acData.mnDigitalIn2);
    mnDigitalIn3.update(acData.mnDigitalIn3);
    mnDigitalIn4.update(acData.mnDigitalIn4);
    
    mnDigitalIn5.update(acData.mnDigitalIn5);
    mnDigitalIn6.update(acData.mnDigitalIn6);
    mnDigitalIn7.update(acData.mnDigitalIn7);
    mnDigitalIn8.update(acData.mnDigitalIn8); 
  }
}