package ilite.display.modules.display2009;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JPanel;

public class KeyPanel extends JPanel
{
  
  public static final Color almostBlack = new Color(10,10,10);
  BufferedImage iliteLogo;
  
  public KeyPanel()
  {
    super();
    
    Dimension size = new Dimension(847,213);
    
    setMaximumSize(size);
    setPreferredSize(size);
    setMinimumSize(size);
    
    File logo = new File("iliteLogo.gif");
    try
    {
      iliteLogo = ImageIO.read(logo);
    }
    catch(IOException e)
    {
      e.printStackTrace();
    }
  }
  
  public void paint(Graphics g)
  {
    super.paint(g);
    
    g.setColor(almostBlack);
    
    g.fillRect(0,0,847,213);
    
    g.drawImage(iliteLogo,300,10,null);
    
    paintKey(g);
    
    
  }
  
  public void paintKey(Graphics g)
  {
    g.setColor(Color.CYAN);
    g.drawOval(640, 10, 25, 25);
    g.drawLine(640, 22, 35+630, 22);
    g.drawLine(23+630, 10, 23+630, 35);
    g.drawString("= Joystick manual target", 670,26); 
      
    g.setColor(Color.YELLOW);
    g.drawRect(640, 50, 25, 25);
    g.drawLine(640, 50, 35+630, 75);
    g.drawLine(640, 75, 35+630, 50);
    g.drawString("= Current position of the turret", 670,66);
    
    /*
    g.setColor(Color.PINK);
    g.drawRoundRect(10,90,25,25,13,13);
    g.drawString("= Selected target", 40,106);*/
    
    g.setColor(Color.ORANGE);
    g.drawLine(640,90,35+630,90); 
    g.drawLine(640,90,35+630,115);
    g.drawLine(640,115,35+630,115);
    g.drawLine(35+630,90,10+630,115);
    g.drawString("= Target location of the turret", 670,106);
  }
}