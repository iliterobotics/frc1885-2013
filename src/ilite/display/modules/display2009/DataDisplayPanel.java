package ilite.display.modules.display2009;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;

import javax.swing.JPanel;

public class DataDisplayPanel extends JPanel implements Runnable
{
  private static Color almostBlack = new Color(10,10,10);
  
  private int mnFeederOpen;
  private int mnConveyorOn;
  private int mbRunCompressor;
  private float mnBattery;
  
  private Thread mcTestThread;
  
  public DataDisplayPanel()
  {
    super();
    
    Dimension size = new Dimension(173,713);
    
    setMaximumSize(size);
    setPreferredSize(size);
    setMinimumSize(size);
    
    mcTestThread = new Thread(this);
    mcTestThread.start();
  }
  
  public void paint(Graphics g)
  {
    super.paint(g);
    
    g.setColor(almostBlack);
    g.fillRect(0,0,172,713);
    
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
      
      xVal[0] = 150; //Top most
      xVal[1] = 140; //Top left
      xVal[2] = 147; //Left crux
      xVal[3] = 147; //Bottom left
      xVal[4] = 153; //Bottom right
      xVal[5] = 153; //Right crux
      xVal[6] = 160; //Top right
      
      yVal[0] = 367; //Bottom point
      yVal[1] = 357; //Left point
      yVal[2] = 357; 
      yVal[3] = 345; //Top left
      yVal[4] = 345; //Top right
      yVal[5] = 357;
      yVal[6] = 357; 
      
      
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
  
  public void setData(DriverVisualsStruct acData)
  {
    mbRunCompressor = (int)acData.mbRunCompressor;
    mnBattery = acData.mnBattery;
    mnConveyorOn = acData.mnConveyorOn;
    mnFeederOpen = acData.mnFeederOpen;

    repaint();
  }
  
  public void run()
  {
    mnBattery=13;
    pause();
    mbRunCompressor=1;
    pause();
    mbRunCompressor=0;
    mnFeederOpen=1;
    pause();
    mnFeederOpen=0;
    mnConveyorOn=1;
    pause();
    mnConveyorOn=2;
    pause();
    mnConveyorOn=3;
    pause();
    mnConveyorOn=0;
    for(mnBattery=13;mnBattery>=8;mnBattery-=.25)
    {
      try
      {
         mcTestThread.sleep(75);
      }
      catch(Exception e)
      {
        e.printStackTrace();
      }
      
      repaint();
    }
    
  }
  private void pause()
  {
    try
    {
       mcTestThread.sleep(750);
    }
    catch(Exception e)
    {
      e.printStackTrace();
    }
    
    repaint();
  }
  
}