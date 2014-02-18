package ilite.display.modules.display2009;

import java.awt.Component;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import javax.swing.JCheckBox;
import javax.swing.JPanel;

public class DriverVisualsBACKUP extends JPanel
{  
  JCheckBox targets,recommendedLauncher,manualLauncher;
  BHSCustomLabel airPressure,runCompressor,robotSpeed,robotDirection,driveStatus;
  
  robotTargets visibleRobots;
  
  float[] angle = new float[15];
  float[] distance = new float[15];
  int[] redOrBlue = new int[15];
  int[] validity = new int[15];
   
  float launcherAngle;
  float launcherDistance;
  
  int mbPhysicalMotorAFacing;
  int mbPhysicalMotorBFacing;
  int mbPhysicalMotorCFacing;
  int mbPhysicalMotorDFacing;
  
  protected GridBagLayout mcGblDriverVisualsPanel;
  protected GridBagConstraints mcGbc;
  
  public DriverVisualsBACKUP()
  {
    
    visibleRobots = new robotTargets();
    
    //make GBL work
    mcGblDriverVisualsPanel = new GridBagLayout();
    mcGbc = new GridBagConstraints();
    
    mcGbc.weightx = 100;
    mcGbc.weighty = 100;
    mcGbc.insets.bottom = 0;
    mcGbc.insets.left = 0;
    mcGbc.insets.right = 0;
    mcGbc.insets.top = 0;
    mcGbc.fill = GridBagConstraints.NONE;
    mcGbc.anchor = GridBagConstraints.NORTHWEST;
    
    setLayout(mcGblDriverVisualsPanel);
    //end GBL working
    
    /*//check boxes
    targets = new JCheckBox("Visual Targets",true);
    recommendedLauncher = new JCheckBox("Computer Calculated Launcher",true);
    manualLauncher = new JCheckBox("Manual Launcher",true);
    //end check boxes*/
    
    //BHSCustomLabels
    airPressure = new BHSCustomLabel("Air pressure: ");
    runCompressor = new BHSCustomLabel("Run compressor: ");
    robotSpeed = new BHSCustomLabel("Robot speed: ");
    robotDirection = new BHSCustomLabel("Robot direction: ");
    driveStatus = new BHSCustomLabel("Drive Status: ");
    //end BHSCustomLabels
    
    //add stuff
   /* add(targets,1,1,1,1);
    add(recommendedLauncher,2,1,1,1);
    add(manualLauncher,3,1,1,1);*/
    mcGbc.anchor = GridBagConstraints.CENTER;
    add(visibleRobots,1,1,1,1);
    mcGbc.anchor = GridBagConstraints.NORTHWEST;
    /*add(airPressure,4,1,1,1);
    add(runCompressor,5,1,1,1);
    add(robotSpeed,6,1,1,1);
    add(robotDirection,7,1,1,1);
    add(driveStatus,8,1,1,1);*/
    //end add stuff
      
  }
  
  protected void add (Component c, int y, int x, int w, int h)
  {
    //setup constraints
    mcGbc.gridx = x - 1;
    mcGbc.gridy = y - 1;
    mcGbc.gridwidth = w;
    mcGbc.gridheight = h;
    // apply constraints to layout
    mcGblDriverVisualsPanel.setConstraints(c, mcGbc);
    // add new element to Panel
    super.add(c);
  
  
//    recommendedLauncher,manualLauncher;
  }
  
  public void setData(DriverVisualsStruct acData)
  {
    angle     = acData.angle;
    distance  = acData.distance;
    redOrBlue = acData.redOrBlue;
    validity  = acData.validity;
     
    launcherAngle    = acData.launcherAngle;
    launcherDistance = acData.launcherDistance;
    
    driveStatus.update(acData.meDriveStatus);
    
    mbPhysicalMotorAFacing = acData.mbPhysicalMotorAFacing;
    mbPhysicalMotorBFacing = acData.mbPhysicalMotorBFacing;
    mbPhysicalMotorCFacing = acData.mbPhysicalMotorCFacing;
    mbPhysicalMotorDFacing = acData.mbPhysicalMotorDFacing;
    
    airPressure.update(acData.mnAirPressure);
    runCompressor.update(acData.mbRunCompressor);
    
    robotSpeed.update(acData.mnRobotSpeed);
    robotDirection.update(acData.mnRobotDirection);
    
    visibleRobots.setData(acData);
  }  
}