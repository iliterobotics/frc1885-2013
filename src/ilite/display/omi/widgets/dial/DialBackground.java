package ilite.display.omi.widgets.dial;

import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.EDrawPriority;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Shape;
import java.awt.Stroke;

public class DialBackground extends AbstractLayer
{
  private final Shape mBackgroundShape;
  private Stroke mAnnotationLine = null;
  
  public DialBackground(IRoundDataDial pDial, Shape pArcShape)
  {
    mBackgroundShape = pArcShape;
    setForeground(Color.green);
    mAnnotationLine = new BasicStroke(3);
  }
  
  @Override
  public void paint(Graphics g)
  {
    Graphics2D g2 = (Graphics2D)g;
    g2.setColor(getBackground());
    g2.fill(mBackgroundShape);
    g2.setColor(getForeground());
    
    Stroke s = g2.getStroke();
    g2.setStroke(mAnnotationLine);
    g2.draw(mBackgroundShape);
    g2.setStroke(s);
  }

  @Override
  public EDrawPriority getPriority()
  {
    return EDrawPriority.LOWEST_LAYER;
  }

}


