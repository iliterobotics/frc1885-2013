package ilite.display.modules.display2009;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;

import javax.swing.JPanel;

public class DataDisplayPanelTest extends JPanel implements Runnable
{
  private static Color almostBlack = new Color(10,10,10);
  private static Color purple = new Color(138,43,226);
  private static Font displayFont = new Font("Arial", Font.PLAIN, 16);
  
  private int mnFeederOpen;
  private int mnConveyorOn;
  private int mbRunCompressor;
  private float mnBattery;
  
  private Thread mcTestThread;
  
  public DataDisplayPanelTest()
  {
    super();
    
    Dimension size = new Dimension(173,450);
    
    setMaximumSize(size);
    setPreferredSize(size);
    setMinimumSize(size);
    
    mcTestThread = new Thread(this);
   // mcTestThread.start();
  }
  
  public void paint(Graphics g)
  {
    super.paint(g);
    
    /*g.setColor(almostBlack);
    g.fillRect(0,0,172,713);*/
    
    g.setFont(displayFont);
    
    g.setColor(Color.BLACK);
    String str = "Air compressor: ";
    if(mbRunCompressor==0)
      g.setColor(Color.RED);
    else
      g.setColor(Color.GREEN);
      
    g.fillOval(30,30,80,80); 
    g.setColor(Color.BLACK); 
    g.drawString("Air compressor: ", 20,20);
    g.drawOval(30,30,80,80);
    //g.drawString("Battery: "+mnBattery,850,46);
    
    
    int[] xVal = new int[7];
    int[] yVal = new int[7];
    
    if(mnFeederOpen == 0)
      g.setColor(Color.RED);
    else if (mnFeederOpen == 1)
      g.setColor(Color.GREEN);
    else if (mnFeederOpen == 2)
    {
      int middleX=70; //Center tip
      int middleY=192; //Arrow crux
      
      str = "FORWARD";
      xVal[0] = middleX; //Top most
      xVal[1] = middleX-20; //Top left
      xVal[2] = middleX-6; //Left crux
      xVal[3] = middleX-6; //Bottom left
      xVal[4] = middleX+6; //Bottom right
      xVal[5] = middleX+6; //Right crux
      xVal[6] = middleX+20; //Top right
      
      yVal[0] = middleY-20;
      yVal[1] = middleY;
      yVal[2] = middleY;
      yVal[3] = middleY+24;
      yVal[4] = middleY+24;
      yVal[5] = middleY;
      yVal[6] = middleY; 
    }
      
    else
    {
      str = "REVERSE";
      
      int middleX=70; //Center tip
      int middleY=197; //Arrow crux
      
      str = "FORWARD";
      xVal[0] = middleX;
      xVal[1] = middleX-20;
      xVal[2] = middleX-6;
      xVal[3] = middleX-6;
      xVal[4] = middleX+6;
      xVal[5] = middleX+6;
      xVal[6] = middleX+20;
      
      yVal[0] = middleY+20; //Bottom point
      yVal[1] = middleY; //Left point
      yVal[2] = middleY; 
      yVal[3] = middleY-24; //Top left
      yVal[4] = middleY-24; //Top right
      yVal[5] = middleY;
      yVal[6] = middleY; 
    }
      
    
    if(str.equals("REVERSE") || str.equals("FORWARD"))
    {
      g.setColor(Color.GREEN);
      g.fillOval(30,160,80,80);  
      g.setColor(Color.BLACK);  
      g.drawString("Feeder gate: ", 20, 150);
      g.drawOval(30,160,80,80);
      g.setColor(Color.WHITE);
      g.fillPolygon(xVal,yVal,7);
    } 
    else
    {
      g.fillOval(30,160,80,80);  
      g.setColor(Color.BLACK);  
      g.drawString("Feeder gate: ", 20, 150);
      g.drawOval(30,160,80,80);
    }
      
    
    
    if(mnConveyorOn == 0)
      g.setColor(Color.RED);
    else if (mnConveyorOn == 1)
      g.setColor(Color.GREEN);
    else if (mnConveyorOn == 2)
    {
      int middleX=70; //Center tip
      int middleY=324; //Arrow crux
      
      str = "FORWARD";
      xVal[0] = middleX; //Top most
      xVal[1] = middleX-20; //Top left
      xVal[2] = middleX-6; //Left crux
      xVal[3] = middleX-6; //Bottom left
      xVal[4] = middleX+6; //Bottom right
      xVal[5] = middleX+6; //Right crux
      xVal[6] = middleX+20; //Top right
      
      yVal[0] = middleY-20;
      yVal[1] = middleY;
      yVal[2] = middleY;
      yVal[3] = middleY+24;
      yVal[4] = middleY+24;
      yVal[5] = middleY;
      yVal[6] = middleY; 
    }
      
    else
    {
      str = "REVERSE";
      
      int middleX=70; //Center tip
      int middleY=329; //Arrow crux
      
      str = "FORWARD";
      xVal[0] = middleX;
      xVal[1] = middleX-20;
      xVal[2] = middleX-6;
      xVal[3] = middleX-6;
      xVal[4] = middleX+6;
      xVal[5] = middleX+6;
      xVal[6] = middleX+20;
      
      yVal[0] = middleY+20; //Bottom point
      yVal[1] = middleY; //Left point
      yVal[2] = middleY; 
      yVal[3] = middleY-24; //Top left
      yVal[4] = middleY-24; //Top right
      yVal[5] = middleY;
      yVal[6] = middleY; 
    }
      
    
    if(str.equals("REVERSE") || str.equals("FORWARD"))
    {
      g.setColor(Color.GREEN);
      g.fillOval(30,290,80,80);  
      g.setColor(Color.BLACK);  
      g.drawString("Conveyor: ", 20, 280);
      g.drawOval(30,290,80,80);
      g.setColor(Color.WHITE);
      g.fillPolygon(xVal,yVal,7);
    } 
    else
    {
      g.fillOval(30,290,80,80);  
      g.setColor(Color.BLACK);  
      g.drawString("Conveyor: ", 20, 280);
      g.drawOval(30,290,80,80);
    }
    
    paintBattery(g);
  }
  
  public void paintBattery(Graphics g)
  {
    int LEFT_OF_BATTERY =20;
    int TOP_OF_BATTERY = 400;
    Graphics g2 = g.create();
    int width;
    if(mnBattery<8)
      mnBattery=8;
    else if(mnBattery>13)
      mnBattery=13;
    
    mnBattery=(float)(((int)(mnBattery*1000))/1000.0);  

    width=(int)((mnBattery-8) * 20 + 5);
      
      
    g2.setColor(Color.WHITE); //Color.WHITE
    g2.fillRect(LEFT_OF_BATTERY,TOP_OF_BATTERY,106,41);  
    g2.setColor(Color.BLACK);  
    g2.drawRect(LEFT_OF_BATTERY,TOP_OF_BATTERY,106,41);
    g2.fillRect(LEFT_OF_BATTERY+106,TOP_OF_BATTERY+11,10,20);
    g2.setColor(Color.BLACK); //Color.BLACK
    g2.setXORMode(Color.WHITE); //Color.WHITE
    g2.drawString(""+mnBattery+" V",LEFT_OF_BATTERY+26,TOP_OF_BATTERY+26);
    
    
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
    mnFeederOpen=2;
    pause();
    mnFeederOpen=3;
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