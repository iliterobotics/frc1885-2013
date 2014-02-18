package ilite.display.omi.widgets.dial;

import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.EDrawPriority;
import ilite.util.gui.stroke.WobbleStroke;
import ilite.util.lang.FixedSizeQueue;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Stroke;

public class DialHand extends AbstractLayer
{
  private final IRoundDataDial mDial;
  private FixedSizeQueue<Double> mAveragedValues = new FixedSizeQueue<Double>(1);
  private Stroke mAnnotationLine = new WobbleStroke(0.5f, 2.5f, 2.0f);
  
  /**
   * @param pRadius -- The radius, in pixels, that the hand should be
   * drawn at.
   */
  public DialHand(IRoundDataDial pDial)
  {
    mDial = pDial;
  }
  
  public void setRunningAverage(int pNumToAvg)
  {
    mAveragedValues.setQueueSize(pNumToAvg);
  }

  @Override
  public void paint(Graphics g)
  {
    double angle = mDial.getAngleOfValue();
    Point p = mDial.getOriginPoint();
    int x = (int)(p.x + Math.cos(angle)*mDial.getRadius());
    int y = (int)(p.y - Math.sin(angle)*mDial.getRadius());
    
    Graphics2D g2 = (Graphics2D)g;
    Stroke s = g2.getStroke();
    g2.setStroke(mAnnotationLine);
    g2.drawLine(p.x, p.y, x, y);
    g2.setStroke(s);
  }

  @Override
  public EDrawPriority getPriority()
  {
    return EDrawPriority.ANNOTATION_HIGH;
  }
}
