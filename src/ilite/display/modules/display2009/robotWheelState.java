package ilite.display.modules.display2009;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Rectangle;

import javax.swing.JPanel;

public class robotWheelState extends JPanel
{
  
  private Rectangle leftFront,leftBack,rightFront,rightBack;
  
  int mbPhysicalMotorAFacing;
  int mbPhysicalMotorBFacing;
  int mbPhysicalMotorCFacing;
  int mbPhysicalMotorDFacing;
    
  
  
  public robotWheelState()
  {
    leftFront = new Rectangle(0,0,10,20);
    leftBack = new Rectangle(0,40,10,20);
    rightFront = new Rectangle(50,0,10,20);
    rightBack = new Rectangle(50,40,10,20);
    
    Dimension size = new Dimension(61,61);
    
    setSize(60,60);
    setMaximumSize(size);
    setPreferredSize(size);
    setMinimumSize(size);
  }
  
  
  
  public void paint (Graphics g)
  {
    super.paint(g);
    Color almostBlack = new Color(10,10,10);
    g.setColor(almostBlack);
    g.fillRect(0,0,100,100);
    
    Color purple = new Color(138,43,226);
    
    g.setColor(Color.WHITE);
    g.fillRect((int)leftFront.getX(),(int)leftFront.getY(),(int)leftFront.getSize().getWidth(), (int)leftFront.getSize().getHeight());
    g.fillRect((int)leftBack.getX(),(int)leftBack.getY(),(int)leftBack.getSize().getWidth(), (int)leftBack.getSize().getHeight());
    g.fillRect((int)rightFront.getX(),(int)rightFront.getY(),(int)rightFront.getSize().getWidth(), (int)rightFront.getSize().getHeight());
    g.fillRect((int)rightBack.getX(),(int)rightBack.getY(),(int)rightBack.getSize().getWidth(), (int)rightBack.getSize().getHeight());
    g.setColor(purple);
    g.fillRect(10,10,40,40);
    
  }  
    
  public void setData(int abPhysicalMotorAFacing, int abPhysicalMotorBFacing, int abPhysicalMotorCFacing, int abPhysicalMotorDFacing)
  {
    mbPhysicalMotorAFacing = abPhysicalMotorAFacing;
    mbPhysicalMotorBFacing = abPhysicalMotorBFacing;
    mbPhysicalMotorCFacing = abPhysicalMotorCFacing;
    mbPhysicalMotorDFacing = abPhysicalMotorDFacing;
    
    updateOrientation();
    
  }
  //Facing 0 is forward
  //Other is sideways
  public void updateOrientation()
  {
    if(mbPhysicalMotorAFacing==0)
    {
      leftBack.setLocation(0,40);
      leftBack.setSize(10,20);
    }
    else
    {
      leftBack.setLocation(0,50);
      leftBack.setSize(20,10);
    }
    if(mbPhysicalMotorBFacing==0)
    {
      leftFront.setLocation(0,0);
      leftFront.setSize(10,20);
    }
    else
    {
      leftFront.setLocation(0,0);
      leftFront.setSize(20,10);
    }
    if(mbPhysicalMotorCFacing==0)
    {
      rightFront.setLocation(50,0);
      rightFront.setSize(10,20);
    }
    else
    {
      rightFront.setLocation(40,0);
      rightFront.setSize(20,10);
    }
    if(mbPhysicalMotorDFacing==0)
    {
      rightBack.setLocation(50,40);
      rightBack.setSize(10,20);
    }
    else
    {
      rightBack.setLocation(40,50);
      rightBack.setSize(20,10);
    }
    
    repaint();
                                                       
    /*b  c
      a  d*/
  }
}