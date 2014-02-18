package ilite.display.modules.display2009;

import java.awt.Color;
import java.awt.Graphics;

import javax.swing.JPanel;

public class TargetView extends JPanel
{
  float[] angle = new float[15];
  float[] distance = new float[15];
  int[] redOrBlue = new int[15];
  int[] validity = new int[15];
  
  float launcherAngle;
  float launcherDistance;
  
  
  public void paint (Graphics g)
  {
    double x;
    double y;
    
    super.paint(g);   
    //Paints location of targets  
    for(int k = 0; k<5; k++) 
    {
      x=Math.sin(angle[k])*distance[k];
      y=Math.cos(angle[k])*distance[k];
      
      if(redOrBlue[k]==0)
        g.setColor(Color.RED);
      else if(redOrBlue[k]==1)
        g.setColor(Color.BLUE);
      else
        g.setColor(Color.GRAY);
      if(validity[k]==1)
        g.fillRect((int)x-25, (int)y-25, 50,50);
    }
    
    //Paints location of launcher
    
    g.setColor(Color.CYAN);
    
    x=Math.sin(launcherAngle)*launcherDistance;
    y=Math.cos(launcherAngle)*launcherDistance;
    g.drawOval((int)x-25, (int)y-25, 50,50);
    g.drawLine((int)x-25, (int)y, (int)x+25, (int)y);
    g.drawLine((int)x, (int)y-25, (int)x, (int)y+25);
  } 
    
  public void setData(TargetViewDataStruct acData)
  {
    for(int i=0;i<5;i++)
    {
      angle[i] = acData.angle[i];
      distance[i] = acData.distance[i];
      redOrBlue[i] = acData.redOrBlue[i];
      validity[i] = acData.validity[i];
    }
    
    launcherAngle = acData.launcherAngle;
    launcherDistance = acData.launcherDistance; 
  }
}