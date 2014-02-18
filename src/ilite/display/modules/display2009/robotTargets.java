package ilite.display.modules.display2009;

//738  =  Maximum height

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JPanel;

public class robotTargets extends JPanel implements Runnable
{

  static final int LEFT_OF_ROBOT = 652;
  static final int TOP_OF_ROBOT = 621; 
    
  static final int LEFT_OF_ARCS = 407-25;
  static final int TOP_OF_ARCS = 301;
  
  static final int CENTER_POINT_OF_RADAR_THINGY = 682;
  static final int TOTAL_Y_IN_SCREEN = 601;
  
  //Total space represented by the radar grid in inches
  static float RANGE_OF_RADAR = 60*12;
  
  static float RANGE_OF_TURRET = RANGE_OF_RADAR/2;
  
  //In degrees
  static final int MAX_ANGLE = 135;
  static final int MIN_ANGLE = 45;
  
  static float INCHES_TO_PIXELS_RATIO = 500.0f/RANGE_OF_RADAR;
  
  static float SIZE_OF_TARGET = 36*INCHES_TO_PIXELS_RATIO;
  
  static float SIZE_OF_FRIEND = SIZE_OF_TARGET/2;
  
  static final int END_ZONE_1_ANGLE = 60;
  static final int END_ZONE_2_ANGLE = 120;
  
  static int ACTIVE_BOUNDS_LEFT;
  static int ACTIVE_BOUNDS_RIGHT;
  
  static String[] driveStates={"AUTOMATIC","FORWARD", "SIDEWAYS","VECTOR", "FORWARD HYBRID", "BACKWARD HYBRID", "LEFT HYBRID", "RIGHT HYBRID"};
  
  float[] maAngle;
  float[] maDistance;
  int[] maRedOrBlue;
  int[] maValidity;
   
  // Joystick
  float mnLauncherAngle;  
  float mnLauncherDistance;
  
  //Where turret is pointing
  float mnActualShootingDistance;
  float mnActualShootingAngle;
  
  // Where turret is attempting to go
  float mnDesiredShootingAngle;
  float mnDesiredShootingDistance;
  
  //Location of current target
  float mnCurrentTargetDistance;
  float mnCurrentTargetAngle;
  
  private Rectangle leftFront,leftBack,rightFront,rightBack;
  
  int mbPhysicalMotorAFacing;
  int mbPhysicalMotorBFacing;
  int mbPhysicalMotorCFacing;
  int mbPhysicalMotorDFacing;
  
  int mbRunCompressor;
  float mnBattery;
  int meDriveStatus;
  
  int mnCameraZone1Active;
  int mnCameraZone2Active;
  int mnCameraZone3Active;
  
  int mnFeederOpen;
  int mnConveyorOn;
  
  int mnTeamColor;
  
  Thread mcTestThread;
  
  Font twitchFont;
  BufferedImage ilite;
  public void run()
  {
    float[] aaAngle = {45,75,90,105,135,0,0,0,0,0,0,0,0,0,0};
    float[] aaDistance = {50,50,50,50,50,0,0,0,0,0,0,0,0,0,0};
    int[] aaRedOrBlue = {0,0,1,1,1,0,0,0,0,0,0,0,0,0,0};
    int[] aaValidity = {1,1,1,1,1,0,0,0,0,0,0,0,0,0,0};
    float anLauncherAngle = 0;
    float anLauncherDistance = 0;
    float a;
    /*for(int i = 30;i<= 150; i+=5)
    {
      a = i%2;
      for(int j = 0; j <=360; j+=2)
      {
        try{
           mcTestThread.sleep(10);
        }
        catch(Exception e){
          System.out.println("Error: " + e);
        }
        maAngle=aaAngle;
        maDistance=aaDistance;
        maRedOrBlue=aaRedOrBlue;
        maValidity=aaValidity;
        mbPhysicalMotorAFacing=a;
        mbPhysicalMotorBFacing=a;
        mbPhysicalMotorCFacing=a;
        mbPhysicalMotorDFacing=a;
        mnLauncherAngle=i;
        mnLauncherDistance=j;
        
        
        
        updateOrientation();
    
        repaint();
      }//for j
      
       
    }//for i*/
    a=50;
    for(int x = 0; x<10;x++)
    {
      for(int k=0;k<5;k++)
        aaDistance[k]=50;
        
      for(int c=0;c<100;c++)
      {
        
        try
        {
           mcTestThread.sleep(49);
        }
        catch(Exception e)
        {
          System.out.println("Error: " + e);
        }
        
        
        aaDistance[0]+=5;
        aaDistance[1]+=5;
        aaDistance[2]+=5;
        aaDistance[3]+=5;
        aaDistance[4]+=5;
        maAngle=aaAngle;
        maDistance=aaDistance;
        maRedOrBlue=aaRedOrBlue;
        maValidity=aaValidity;
        mbPhysicalMotorAFacing=0;
        mbPhysicalMotorBFacing=0;
        mbPhysicalMotorCFacing=0;
        mbPhysicalMotorDFacing=0;
        mnLauncherAngle =30;
        mnLauncherDistance =500;
        mnActualShootingDistance=300;
        mnActualShootingAngle=90;
        mnDesiredShootingAngle=120;
        mnDesiredShootingDistance=300;
        mnCurrentTargetDistance=300;
        mnCurrentTargetAngle=70;
        
        updateOrientation();
    
        repaint();
        
        /*if(c%10==0)
        {
          a=aaAngle[0];
          aaAngle[0]-=6;
          try
          {
             mcTestThread.sleep(49);
          }
          catch(Exception e)
          {
            System.out.println("Error: " + e);
          }
          aaAngle[0]=a;
        }*/
        
                
      }
      aaAngle[0]+=10;
      aaAngle[1]+=10;
      aaAngle[2]+=10;
      aaAngle[3]+=10;
      aaAngle[4]+=10;
    }
      
  }//run
  
  public robotTargets()
  {
    maAngle = new float[15];
    maDistance = new float[15];
    maRedOrBlue = new int[15];
    maValidity = new int[15];
    
    leftFront = new Rectangle(0+LEFT_OF_ROBOT,0+TOP_OF_ROBOT,10,20);
    leftBack = new Rectangle(0+LEFT_OF_ROBOT,40+TOP_OF_ROBOT,10,20);
    rightFront = new Rectangle(50+LEFT_OF_ROBOT,0+TOP_OF_ROBOT,10,20);
    rightBack = new Rectangle(50+LEFT_OF_ROBOT,40+TOP_OF_ROBOT,10,20);
    
    File logo = new File("iliteLogo.gif");
    try
    {
      ilite =ImageIO.read(logo);
    }
    catch(IOException e)
    {
      e.printStackTrace();
    }
    
    Dimension size = new Dimension(1019,713);
    
    setMaximumSize(size);
    setPreferredSize(size);
    setMinimumSize(size);
    
    twitchFont = new Font("Arial", Font.PLAIN, 20);
    
    mcTestThread = new Thread(this);
    //mcTestThread.start();
  }
  
  public void paint(Graphics g)
  {
    super.paint(g);
    
//    RANGE_OF_RADAR = new Integer(ControlPanel.range.getText())*12;
//    RANGE_OF_TURRET = new Integer(ControlPanel.turretRange.getText())*12;
    INCHES_TO_PIXELS_RATIO = 300.0f/RANGE_OF_RADAR;
    SIZE_OF_TARGET = 36*INCHES_TO_PIXELS_RATIO;
    SIZE_OF_FRIEND = SIZE_OF_TARGET/2;
    
    Color almostBlack = new Color(10,10,10);
    Color purple = new Color(138,43,226);
    Color jacobBlue = new Color(0,128,255);
    g.setColor(almostBlack);
    g.fillRect(0,0,1018,712);
    
    //paintKey(g);
    paintData(g);
   // colorTest(g);
    g.drawImage(ilite,400,5,null);
    
    //set active zones
    if(mnCameraZone1Active!=0)
      g.setColor(Color.GREEN);
    else
      g.setColor(almostBlack);
    g.fillArc(-25+LEFT_OF_ARCS,-25+TOP_OF_ARCS,650,650,120,60);
    
    if(mnCameraZone2Active!=0)
      g.setColor(Color.GREEN);
    else
      g.setColor(almostBlack);
    g.fillArc(-25+LEFT_OF_ARCS,-25+TOP_OF_ARCS,650,650,60,59);
    
    if(mnCameraZone3Active!=0)
      g.setColor(Color.GREEN);
    else
      g.setColor(almostBlack);
    g.fillArc(-25+LEFT_OF_ARCS,-25+TOP_OF_ARCS,650,650,0,59);
    
    
    g.setColor(almostBlack);
    g.fillArc(LEFT_OF_ARCS,TOP_OF_ARCS,600,600,0,180);
    g.setColor(Color.GREEN);
    g.drawArc(-25+LEFT_OF_ARCS,-25+TOP_OF_ARCS,650,650,0,180);
    
    double x,y;  
      
      //Paints targets 
      for(int k = 0; k < 15; k++) 
      {
        System.out.println("robotTargets--\nAngle: "+maAngle[0]+ "\nDistance: "+maDistance[0]);
        x=CENTER_POINT_OF_RADAR_THINGY+Math.cos(maAngle[k]/180*Math.PI)*maDistance[k]*INCHES_TO_PIXELS_RATIO;
        y=TOTAL_Y_IN_SCREEN-Math.sin(maAngle[k]/180*Math.PI)*maDistance[k]*INCHES_TO_PIXELS_RATIO;
        
        if(maRedOrBlue[k]==0)
          g.setColor(Color.RED);
        else if(maRedOrBlue[k]==1)
          g.setColor(jacobBlue);
        else
          g.setColor(Color.GRAY);
        if(maValidity[k]==1)
        {
          
          if(maAngle[k]>=45&&maAngle[k]<=135&&maDistance[k]<=RANGE_OF_TURRET)
          {
            //Target is a friend. Don't kill them.
            if(maRedOrBlue[k]==mnTeamColor)
            {
              g.fillRect((int)(x-SIZE_OF_FRIEND/2), (int)(y-SIZE_OF_FRIEND/2), (int)SIZE_OF_FRIEND,(int)SIZE_OF_FRIEND);
              g.setColor(Color.WHITE);
              g.drawRect((int)(x-SIZE_OF_FRIEND/2), (int)(y-SIZE_OF_FRIEND/2), (int)SIZE_OF_FRIEND,(int)SIZE_OF_FRIEND);
            }
            //Target is either an enemy, or we have no idea. To the death!
            else
            {
              g.fillRect((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)SIZE_OF_TARGET,(int)SIZE_OF_TARGET);
              g.setColor(Color.WHITE);
              g.drawRect((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)SIZE_OF_TARGET,(int)SIZE_OF_TARGET);
            }   
            
          }
          //If it's outside of the range, draw a "ghost" target, with just an outline. :D
          else if(maAngle[k]>=0&&maAngle[k]<=180)
          {
            //Target is a friend. Don't kill them.
            if(maRedOrBlue[k]==mnTeamColor)
              g.drawRect((int)(x-SIZE_OF_FRIEND/2), (int)(y-SIZE_OF_FRIEND/2), (int)SIZE_OF_FRIEND,(int)SIZE_OF_FRIEND);
            //Target is either an enemy, or we have no idea. To the death!
            else
              g.drawRect((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)SIZE_OF_TARGET,(int)SIZE_OF_TARGET);
          }
             
        }      
      }
    
    
    
    g.setColor(Color.GREEN);
    
    /*
    g.drawArc(-200+LEFT_OF_ARCS,-200+TOP_OF_ARCS,1000,1000,0,180);
    g.drawArc(-150+LEFT_OF_ARCS,-150+TOP_OF_ARCS,900,900,0,180);
    g.drawArc(-100+LEFT_OF_ARCS,-100+TOP_OF_ARCS,800,800,0,180);
    g.drawArc(-50+LEFT_OF_ARCS,-50+TOP_OF_ARCS,700,700,0,180);*/
    g.drawArc(0+LEFT_OF_ARCS,0+TOP_OF_ARCS,600,600,0,180);
    g.drawArc(50+LEFT_OF_ARCS,50+TOP_OF_ARCS,500,500,0,180);
    g.drawArc(100+LEFT_OF_ARCS,100+TOP_OF_ARCS,400,400,0,180);
    g.drawArc(150+LEFT_OF_ARCS,150+TOP_OF_ARCS,300,300,0,180);
    g.drawArc(200+LEFT_OF_ARCS,200+TOP_OF_ARCS,200,200,0,180);
    g.drawArc(250+LEFT_OF_ARCS,250+TOP_OF_ARCS,100,100,0,180);
    //end arcs
   
    
    //draw lines
    
    g.setColor(Color.GREEN);
    g.drawLine(LEFT_OF_ARCS-25,601,LEFT_OF_ARCS+625,601);   //bottom 
    g.drawLine(LEFT_OF_ARCS+300,601,LEFT_OF_ARCS+300,601-300);   //middle 
    g.drawLine(LEFT_OF_ARCS+300,601,480-10,439-50);  //left    
    g.drawLine(LEFT_OF_ARCS+300,601,904-10,439-50);  //right 
    g.drawLine(LEFT_OF_ARCS+300,601,417-10,533-50);  //left lower mid
    g.drawLine(LEFT_OF_ARCS+300,601,971-10,541-50);  //right lower mid
    g.drawLine(LEFT_OF_ARCS+300,601,579-10,373-50);  //left upper mid
    g.drawLine(LEFT_OF_ARCS+300,601,809-10,375-50);  //right upper mid
    //end lines   
    
    //Draw Robot! :D
    g.setColor(Color.WHITE);
    g.fillRect((int)leftFront.getX(),(int)leftFront.getY(),(int)leftFront.getSize().getWidth(), (int)leftFront.getSize().getHeight());
    g.fillRect((int)leftBack.getX(),(int)leftBack.getY(),(int)leftBack.getSize().getWidth(), (int)leftBack.getSize().getHeight());
    g.fillRect((int)rightFront.getX(),(int)rightFront.getY(),(int)rightFront.getSize().getWidth(), (int)rightFront.getSize().getHeight());
    g.fillRect((int)rightBack.getX(),(int)rightBack.getY(),(int)rightBack.getSize().getWidth(), (int)rightBack.getSize().getHeight());
    g.setColor(purple);
    g.fillRect(10+LEFT_OF_ROBOT,10+TOP_OF_ROBOT,40,40);
    
    //Draw waiting hourglass
    g.setColor(Color.ORANGE);
    x=CENTER_POINT_OF_RADAR_THINGY+Math.cos(mnDesiredShootingAngle/180*Math.PI)*mnDesiredShootingDistance*INCHES_TO_PIXELS_RATIO;
    y=TOTAL_Y_IN_SCREEN-Math.sin(mnDesiredShootingAngle/180*Math.PI)*mnDesiredShootingDistance*INCHES_TO_PIXELS_RATIO;    
    
    g.drawLine((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)(x+SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2));
    g.drawLine((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)(x+SIZE_OF_TARGET/2), (int)(y+SIZE_OF_TARGET/2));
    g.drawLine((int)(x+SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)(x-SIZE_OF_TARGET/2), (int)(y+SIZE_OF_TARGET/2));
    g.drawLine((int)(x-SIZE_OF_TARGET/2), (int)(y+SIZE_OF_TARGET/2), (int)(x+SIZE_OF_TARGET/2), (int)(y+SIZE_OF_TARGET/2));
    
    //Draw selected target
    g.setColor(Color.PINK);
    x=CENTER_POINT_OF_RADAR_THINGY+Math.cos(mnCurrentTargetAngle/180*Math.PI)*mnCurrentTargetDistance*INCHES_TO_PIXELS_RATIO;
    y=TOTAL_Y_IN_SCREEN-Math.sin(mnCurrentTargetAngle/180*Math.PI)*mnCurrentTargetDistance*INCHES_TO_PIXELS_RATIO; 
    
    g.drawRoundRect((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)SIZE_OF_TARGET, (int)SIZE_OF_TARGET, (int)(SIZE_OF_TARGET*.5), (int)(SIZE_OF_TARGET*.5));  
    
    //Draw joystick target
    g.setColor(Color.CYAN);
    x=CENTER_POINT_OF_RADAR_THINGY+Math.cos(mnLauncherAngle/180*Math.PI)*mnLauncherDistance*INCHES_TO_PIXELS_RATIO;
    y=TOTAL_Y_IN_SCREEN-Math.sin(mnLauncherAngle/180*Math.PI)*mnLauncherDistance*INCHES_TO_PIXELS_RATIO;    
    
    g.drawOval((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)SIZE_OF_TARGET, (int)SIZE_OF_TARGET);
    g.drawLine((int)(x-SIZE_OF_TARGET/2), (int)y, (int)(x+SIZE_OF_TARGET/2), (int)y);
    g.drawLine((int)x, (int)(y-SIZE_OF_TARGET/2), (int)x, (int)(y+SIZE_OF_TARGET/2));
    
    //Draw current turret
    g.setColor(Color.YELLOW);
    x=CENTER_POINT_OF_RADAR_THINGY+Math.cos(mnActualShootingAngle/180*Math.PI)*mnActualShootingDistance*INCHES_TO_PIXELS_RATIO;
    y=TOTAL_Y_IN_SCREEN-Math.sin(mnActualShootingAngle/180*Math.PI)*mnActualShootingDistance*INCHES_TO_PIXELS_RATIO;    
    
    g.drawRect((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)SIZE_OF_TARGET, (int)SIZE_OF_TARGET);
    g.drawLine((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)(x+SIZE_OF_TARGET/2), (int)(y+SIZE_OF_TARGET/2));
    g.drawLine((int)(x+SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)(x-SIZE_OF_TARGET/2), (int)(y+SIZE_OF_TARGET/2));
    //end headache
    
    //draw twitch state
    g.setFont(twitchFont);
    g.setColor(Color.WHITE);
    g.drawString("Twitch state: "+driveStates[meDriveStatus], LEFT_OF_ROBOT-316, TOP_OF_ROBOT+35);
    //end twitch-state
    
    
    
  }
  
  public void setData(DriverVisualsStruct acData)
  {
    mnLauncherAngle = acData.launcherAngle;
    mnLauncherDistance = acData.launcherDistance;
    maAngle = acData.angle;
    maDistance = acData.distance;
    maRedOrBlue = acData.redOrBlue;
    maValidity = acData.validity;
    
    mbPhysicalMotorAFacing = acData.mbPhysicalMotorAFacing;
    mbPhysicalMotorBFacing = acData.mbPhysicalMotorBFacing;
    mbPhysicalMotorCFacing = acData.mbPhysicalMotorCFacing;
    mbPhysicalMotorDFacing = acData.mbPhysicalMotorDFacing;
    
    mnActualShootingDistance = acData.mnActualShootingDistance;
    mnActualShootingAngle = acData.mnActualShootingAngle;
  
    mnDesiredShootingAngle = acData.mnDesiredShootingAngle;
    mnDesiredShootingDistance = acData.mnDesiredShootingDistance;
    
    mnCurrentTargetDistance = acData.mnCurrentTargetDistance;
    mnCurrentTargetAngle = acData.mnCurrentTargetAngle;
    
    meDriveStatus = (int)acData.meDriveStatus;
    
    mbRunCompressor = (int)acData.mbRunCompressor;
    
    mnBattery = acData.mnBattery;
    
    mnCameraZone1Active = acData.mnCameraZone1Active;
    mnCameraZone2Active = acData.mnCameraZone2Active;
    mnCameraZone3Active = acData.mnCameraZone3Active;
    
    mnTeamColor = acData.mnTeamColor;
    
    mnConveyorOn = acData.mnConveyorOn;
    mnFeederOpen = acData.mnFeederOpen;
    
    updateOrientation();
    repaint();
  }
  
  //Facing 0 is forward
  //Other is sideways
  public void updateOrientation()
  {
    if(mbPhysicalMotorAFacing==0)
    {
      leftBack.setLocation(0+LEFT_OF_ROBOT,40+TOP_OF_ROBOT);
      leftBack.setSize(10,20);
    }
    else
    {
      leftBack.setLocation(0+LEFT_OF_ROBOT,50+TOP_OF_ROBOT);
      leftBack.setSize(20,10);
    }
    if(mbPhysicalMotorBFacing==0)
    {
      leftFront.setLocation(0+LEFT_OF_ROBOT,0+TOP_OF_ROBOT);
      leftFront.setSize(10,20);
    }
    else
    {
      leftFront.setLocation(0+LEFT_OF_ROBOT,0+TOP_OF_ROBOT);
      leftFront.setSize(20,10);
    }
    if(mbPhysicalMotorCFacing==0)
    {
      rightFront.setLocation(50+LEFT_OF_ROBOT,0+TOP_OF_ROBOT);
      rightFront.setSize(10,20);
    }
    else
    {
      rightFront.setLocation(40+LEFT_OF_ROBOT,0+TOP_OF_ROBOT);
      rightFront.setSize(20,10);
    }
    if(mbPhysicalMotorDFacing==0)
    {
      rightBack.setLocation(50+LEFT_OF_ROBOT,40+TOP_OF_ROBOT);
      rightBack.setSize(10,20);
    }
    else
    {
      rightBack.setLocation(40+LEFT_OF_ROBOT,50+TOP_OF_ROBOT);
      rightBack.setSize(20,10);
    }
                                                       
    /*b  c
      a  d*/
  }
  
  public void paintKey(Graphics g)
  {
    g.setColor(Color.CYAN);
    g.drawOval(10, 10, 25, 25);
    g.drawLine(10, 22, 35, 22);
    g.drawLine(23, 10, 23, 35);
    g.drawString("= Joystick manual target", 40,26); 
      
    g.setColor(Color.YELLOW);
    g.drawRect(10, 50, 25, 25);
    g.drawLine(10, 50, 35, 75);
    g.drawLine(10, 75, 35, 50);
    g.drawString("= Current position of the turret", 40,66);
    
    /*
    g.setColor(Color.PINK);
    g.drawRoundRect(10,90,25,25,13,13);
    g.drawString("= Selected target", 40,106);*/
    
    g.setColor(Color.ORANGE);
    g.drawLine(10,90,35,90); 
    g.drawLine(10,90,35,115);
    g.drawLine(10,115,35,115);
    g.drawLine(35,90,10,115);
    g.drawString("= Position the turret is attempting to achieve", 40,106);
    
   /* Image goomba = null;
    try
    {
      goomba =ImageIO.read(new File("Goomba.gif"));
    }
    catch(IOException e)
    {
      e.printStackTrace();
    }
    
    g.drawImage(goomba,10,170,null);
    g.setColor(new Color(227,92,10));
    g.drawString("= Seth's Goomba", 70,196);*/
  }
  
  public void colorTest(Graphics g)
  {
    Color jacobBlue = new Color(0,128,255);
    g.setColor(jacobBlue);
    g.fillRect(998,0,20,20);
  }
  
  public void paintData(Graphics g)
  {
    g.setColor(Color.WHITE);
    String str = "Air compressor: ";
    if(mbRunCompressor==0)
      g.setColor(Color.RED);
    else
      g.setColor(Color.GREEN);
      
    g.fillOval(130,56,40,40); 
    g.setColor(Color.WHITE); 
    g.drawString("Air compressor: ", 20,76);
    g.drawOval(130,56,40,40);
    //g.drawString("Battery: "+mnBattery,850,46);
    
    
    if(mnFeederOpen == 0)
      g.setColor(Color.RED);
    else
      g.setColor(Color.GREEN);
      
    g.fillOval(130,196,40,40);  
    g.setColor(Color.WHITE);  
    g.drawString("Feeder gate: ", 20, 216);
    g.drawOval(130,196,40,40);
    
    int[] xVal = new int[7];
    int[] yVal = new int[7];
    
    mnConveyorOn=2;
    if(mnConveyorOn == 0)
      g.setColor(Color.RED);
    else if (mnConveyorOn == 1)
      g.setColor(Color.GREEN);
    else if (mnConveyorOn == 2)
    {
      str = "FORWARD";
      xVal[0] = 150;
      xVal[1] = 140;
      xVal[2] = 147;
      xVal[3] = 147;
      xVal[4] = 153;
      xVal[5] = 153;
      xVal[6] = 160;
      
      yVal[0] = 345;
      yVal[1] = 355;
      yVal[2] = 355;
      yVal[3] = 367;
      yVal[4] = 367;
      yVal[5] = 355;
      yVal[6] = 355; 
    }
      
    else
    {
      str = "REVERSE";
      
      
    }
      
    
    if(str.equals("REVERSE") || str.equals("FORWARD"))
    {
      g.setColor(Color.GREEN);
      g.fillOval(130,336,40,40);  
      g.setColor(Color.WHITE);  
      g.drawString("Conveyor: ", 20, 356);
      g.drawOval(130,336,40,40);
      //g.setColor(Color.BLACK);
      g.fillPolygon(xVal,yVal,7);
    } 
    else
    {
      g.fillOval(130,336,40,40);  
      g.setColor(Color.WHITE);  
      g.drawString("Conveyor: ", 20, 356);
      g.drawOval(130,336,40,40);
    }
    
    paintBattery(g);
  }
  
  public void paintBattery(Graphics g)
  {
    int LEFT_OF_BATTERY =20;
    int TOP_OF_BATTERY = 506;
    Graphics g2 = g.create();
    int width;
    if(mnBattery<8)
      mnBattery=8;
    else if(mnBattery>13)
      mnBattery=13;
    
    mnBattery= (float)13; 
    mnBattery=(float)(((int)(mnBattery*1000))/1000.0);  
    

    width=(int)((mnBattery-8) * 20);
      
    g2.setColor(Color.WHITE);  
    g2.drawRect(LEFT_OF_BATTERY,TOP_OF_BATTERY,101,41);
    g2.fillRect(LEFT_OF_BATTERY+101,TOP_OF_BATTERY+11,10,20);
    g2.setColor(Color.GREEN);
    g2.drawString(""+mnBattery+" V",LEFT_OF_BATTERY+26,TOP_OF_BATTERY+26);
    g2.setXORMode(Color.BLACK);
    if(mnBattery>12)
      g2.setColor(Color.GREEN);
    else if(mnBattery>11)
      g2.setColor(Color.YELLOW);
    else
      g2.setColor(Color.RED);
    g2.fillRect(LEFT_OF_BATTERY+1,TOP_OF_BATTERY+1,width,40);
    
  }
}