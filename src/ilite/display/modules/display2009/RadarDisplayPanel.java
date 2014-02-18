package ilite.display.modules.display2009;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Rectangle;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class RadarDisplayPanel extends JPanel implements Runnable
{
  //Constants
  private static final int CENTER_POINT_X = 423;
  private static final int CENTER_POINT_Y = 423;
  private static final int WIDTH = 410;
  private static final int NUM_OF_ARCS = 10;
  private static final int WIDTH_OF_ZONE_INDICATOR = 10;
  
  private static final int TOP_OF_ROBOT = CENTER_POINT_Y + 10;
  private static final int LEFT_OF_ROBOT = CENTER_POINT_X - 30;
  private static final String[] driveStates={"AUTOMATIC","FORWARD", "SIDEWAYS","VECTOR", "FORWARD HYBRID", "BACKWARD HYBRID", "LEFT HYBRID", "RIGHT HYBRID"};
  private static final int MAX_ANGLE = 135;
  private static final int MIN_ANGLE = 45;
  
  private static float RANGE_OF_RADAR = 30*12; //in inches
  private static float RANGE_OF_TURRET = RANGE_OF_RADAR/2; //in inches
  private static float INCHES_TO_PIXELS_RATIO = WIDTH/RANGE_OF_RADAR;
  private static float SIZE_OF_TARGET = 36*INCHES_TO_PIXELS_RATIO;
  private static float SIZE_OF_FRIEND = SIZE_OF_TARGET/2;
  
  private static final Font twitchFont = new Font("Arial", Font.PLAIN, 20);
  
  //Colors
  private static Color almostBlack = new Color(10,10,10);
  private static Color purple = new Color(138,43,226);
  private static Color jacobBlue = new Color(0,128,255);
  private static Color[] activeZonesColor = {almostBlack,Color.GREEN};
  
  
  //Rectangles (Wheels)
  private Rectangle leftFront = new Rectangle(0+LEFT_OF_ROBOT,0+TOP_OF_ROBOT,10,20);
  private Rectangle leftBack = new Rectangle(0+LEFT_OF_ROBOT,40+TOP_OF_ROBOT,10,20);
  private Rectangle rightFront = new Rectangle(50+LEFT_OF_ROBOT,0+TOP_OF_ROBOT,10,20);
  private Rectangle rightBack = new Rectangle(50+LEFT_OF_ROBOT,40+TOP_OF_ROBOT,10,20);
  
  //Data variables
  int mbPhysicalMotorAFacing;
  int mbPhysicalMotorBFacing;
  int mbPhysicalMotorCFacing;
  int mbPhysicalMotorDFacing;
  
  int mnCameraZone1Active;
  int mnCameraZone2Active;
  int mnCameraZone3Active;
  
  int mnTeamColor;
  
  int meDriveStatus;
  
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
  
  
  float[] maAngle = new float[15];
  float[] maDistance = new float[15];
  int[] maRedOrBlue = new int[15];
  int[] maValidity = new int[15];
  
  //Test thread
  Thread mcTestThread;
  
  
  public RadarDisplayPanel()
  {
    super();
    
    Dimension size = new Dimension(847,500);
    
    setMaximumSize(size);
    setPreferredSize(size);
    setMinimumSize(size);
    
    mcTestThread = new Thread(this);
    //mcTestThread.start();
  }
  
  public void paint (Graphics g)
  {
    super.paint(g);
    
//    RANGE_OF_RADAR = new Integer(ControlPanel.range.getText())*12;
 //   RANGE_OF_TURRET = new Integer(ControlPanel.turretRange.getText())*12;
    INCHES_TO_PIXELS_RATIO = WIDTH/RANGE_OF_RADAR;
    SIZE_OF_TARGET = 36*INCHES_TO_PIXELS_RATIO;
    SIZE_OF_FRIEND = SIZE_OF_TARGET/2;
    
    g.setColor(almostBlack);
    g.fillRect(0,0,847,500);
    
    paintRadar(g);
    paintTargets(g);
    paintRobot(g);
  }
  
  public void paintRadar(Graphics g)
  {
    g.setColor(activeZonesColor[mnCameraZone1Active]);
    g.fillArc(CENTER_POINT_X-WIDTH-WIDTH_OF_ZONE_INDICATOR,
      CENTER_POINT_Y-WIDTH-WIDTH_OF_ZONE_INDICATOR,2*(WIDTH+WIDTH_OF_ZONE_INDICATOR),
      2*(WIDTH+WIDTH_OF_ZONE_INDICATOR),120,60);
    
    g.setColor(activeZonesColor[mnCameraZone2Active]);
    g.fillArc(CENTER_POINT_X-WIDTH-WIDTH_OF_ZONE_INDICATOR,
      CENTER_POINT_Y-WIDTH-WIDTH_OF_ZONE_INDICATOR,2*(WIDTH+WIDTH_OF_ZONE_INDICATOR),
      2*(WIDTH+WIDTH_OF_ZONE_INDICATOR),60,59);
    
    g.setColor(activeZonesColor[mnCameraZone3Active]);
    g.fillArc(CENTER_POINT_X-WIDTH-WIDTH_OF_ZONE_INDICATOR,
      CENTER_POINT_Y-WIDTH-WIDTH_OF_ZONE_INDICATOR,2*(WIDTH+WIDTH_OF_ZONE_INDICATOR),
      2*(WIDTH+WIDTH_OF_ZONE_INDICATOR),0,59);
    
    
    g.setColor(almostBlack);
    g.fillArc(CENTER_POINT_X-WIDTH,CENTER_POINT_Y-WIDTH,2*(WIDTH),2*(WIDTH),0,180);
    
    double widthOfArc;
    double angle;
      
    g.setColor(Color.GREEN);
      
    for(int c=0;c<NUM_OF_ARCS;c++)
    {  
      widthOfArc=(c+1)/(double)NUM_OF_ARCS;
      
      g.drawArc((int)(CENTER_POINT_X-widthOfArc*WIDTH),
        (int)(CENTER_POINT_Y-widthOfArc*WIDTH),(int)(widthOfArc*2*WIDTH),
        (int)(widthOfArc*2*WIDTH),0,180);
    }
    
    g.drawLine(CENTER_POINT_X-WIDTH-WIDTH_OF_ZONE_INDICATOR,CENTER_POINT_Y,
      CENTER_POINT_X+WIDTH+WIDTH_OF_ZONE_INDICATOR,CENTER_POINT_Y);
    for(int c=1;c<8;c++)
    {
      angle = c/8.0*Math.PI;
      g.drawLine(CENTER_POINT_X,CENTER_POINT_Y,
        CENTER_POINT_X+(int)(Math.cos(angle)*WIDTH), 
        CENTER_POINT_Y-(int)(Math.sin(angle)*WIDTH));
    }
    
    g.drawArc(CENTER_POINT_X-WIDTH-WIDTH_OF_ZONE_INDICATOR,
      CENTER_POINT_Y-WIDTH-WIDTH_OF_ZONE_INDICATOR,2*(WIDTH+WIDTH_OF_ZONE_INDICATOR),
      2*(WIDTH+WIDTH_OF_ZONE_INDICATOR),0,180);
  }
  
  public void paintTargets(Graphics g)
  {
    double x,y;  
      
      //Paints targets 
      for(int k = 0; k < 15; k++) 
      {
        //System.out.println("robotTargets--\nAngle: "+maAngle[0]+ "\nDistance: "+maDistance[0]);
        x=CENTER_POINT_X+Math.cos(maAngle[k]/180*Math.PI)*maDistance[k]*INCHES_TO_PIXELS_RATIO;
        y=CENTER_POINT_Y-Math.sin(maAngle[k]/180*Math.PI)*maDistance[k]*INCHES_TO_PIXELS_RATIO;
        
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
  }
  
  public void paintRobot(Graphics g)
  {
    g.setColor(Color.WHITE);
    g.fillRect((int)leftFront.getX(),(int)leftFront.getY(),(int)leftFront.getSize().getWidth(), (int)leftFront.getSize().getHeight());
    g.fillRect((int)leftBack.getX(),(int)leftBack.getY(),(int)leftBack.getSize().getWidth(), (int)leftBack.getSize().getHeight());
    g.fillRect((int)rightFront.getX(),(int)rightFront.getY(),(int)rightFront.getSize().getWidth(), (int)rightFront.getSize().getHeight());
    g.fillRect((int)rightBack.getX(),(int)rightBack.getY(),(int)rightBack.getSize().getWidth(), (int)rightBack.getSize().getHeight());
    g.setColor(purple);
    g.fillRect(10+LEFT_OF_ROBOT,10+TOP_OF_ROBOT,40,40);
    
    g.setFont(twitchFont);
    g.setColor(Color.WHITE);
    g.drawString("Twitch state: "+driveStates[meDriveStatus], LEFT_OF_ROBOT-316, TOP_OF_ROBOT+35);
    //end twitch-state
  }
  
  public void paintCursors(Graphics g)
  {
    double x;
    double y;
    //Draw waiting hourglass
    g.setColor(Color.ORANGE);
    x=CENTER_POINT_X+Math.cos(mnDesiredShootingAngle/180*Math.PI)*mnDesiredShootingDistance*INCHES_TO_PIXELS_RATIO;
    y=CENTER_POINT_Y-Math.sin(mnDesiredShootingAngle/180*Math.PI)*mnDesiredShootingDistance*INCHES_TO_PIXELS_RATIO;    
    
    g.drawLine((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)(x+SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2));
    g.drawLine((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)(x+SIZE_OF_TARGET/2), (int)(y+SIZE_OF_TARGET/2));
    g.drawLine((int)(x+SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)(x-SIZE_OF_TARGET/2), (int)(y+SIZE_OF_TARGET/2));
    g.drawLine((int)(x-SIZE_OF_TARGET/2), (int)(y+SIZE_OF_TARGET/2), (int)(x+SIZE_OF_TARGET/2), (int)(y+SIZE_OF_TARGET/2));
    
    //Draw selected target
    g.setColor(Color.PINK);
    x=CENTER_POINT_X+Math.cos(mnCurrentTargetAngle/180*Math.PI)*mnCurrentTargetDistance*INCHES_TO_PIXELS_RATIO;
    y=CENTER_POINT_Y-Math.sin(mnCurrentTargetAngle/180*Math.PI)*mnCurrentTargetDistance*INCHES_TO_PIXELS_RATIO; 
    
    g.drawRoundRect((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)SIZE_OF_TARGET, (int)SIZE_OF_TARGET, (int)(SIZE_OF_TARGET*.5), (int)(SIZE_OF_TARGET*.5));  
    
    //Draw joystick target
    g.setColor(Color.CYAN);
    x=CENTER_POINT_X+Math.cos(mnLauncherAngle/180*Math.PI)*mnLauncherDistance*INCHES_TO_PIXELS_RATIO;
    y=CENTER_POINT_Y-Math.sin(mnLauncherAngle/180*Math.PI)*mnLauncherDistance*INCHES_TO_PIXELS_RATIO;    
    
    g.drawOval((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)SIZE_OF_TARGET, (int)SIZE_OF_TARGET);
    g.drawLine((int)(x-SIZE_OF_TARGET/2), (int)y, (int)(x+SIZE_OF_TARGET/2), (int)y);
    g.drawLine((int)x, (int)(y-SIZE_OF_TARGET/2), (int)x, (int)(y+SIZE_OF_TARGET/2));
    
    //Draw current turret
    g.setColor(Color.YELLOW);
    x=CENTER_POINT_X+Math.cos(mnActualShootingAngle/180*Math.PI)*mnActualShootingDistance*INCHES_TO_PIXELS_RATIO;
    y=CENTER_POINT_Y-Math.sin(mnActualShootingAngle/180*Math.PI)*mnActualShootingDistance*INCHES_TO_PIXELS_RATIO;    
    
    g.drawRect((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)SIZE_OF_TARGET, (int)SIZE_OF_TARGET);
    g.drawLine((int)(x-SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)(x+SIZE_OF_TARGET/2), (int)(y+SIZE_OF_TARGET/2));
    g.drawLine((int)(x+SIZE_OF_TARGET/2), (int)(y-SIZE_OF_TARGET/2), (int)(x-SIZE_OF_TARGET/2), (int)(y+SIZE_OF_TARGET/2));
    //end headache
  }
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
    
    mnCameraZone1Active = acData.mnCameraZone1Active;
    mnCameraZone2Active = acData.mnCameraZone2Active;
    mnCameraZone3Active = acData.mnCameraZone3Active;
    
    mnTeamColor = acData.mnTeamColor;
   
    updateOrientation();
    repaint();
  }
  
  public static void main(String[] args)
  {
    JFrame frame = new JFrame("Radar test");
    KeyPanel radar = new KeyPanel();
    
    frame.add(radar);
    frame.pack();
    frame.setVisible(true);
    frame.setLocationRelativeTo(null);
  }
}