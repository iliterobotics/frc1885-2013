package ilite.display.omi.widgets.dial;

import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.EDrawPriority;
import ilite.util.math.Geometry;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.geom.Line2D;
import java.util.ArrayList;
import java.util.List;

public class DialTicks extends AbstractLayer
{
  private final IRoundDataDial mDial;
  private int mNumTicksToDraw = 5;
  private List<Line2D> mTicks = new ArrayList<Line2D>();
  
  public DialTicks(IRoundDataDial pDial)
  {
    mDial = pDial;
    setNumTicks(12);
  }
  
  public void setNumTicks(int pNumTicks)
  {
    mNumTicksToDraw = pNumTicks;
    Point o = mDial.getOriginPoint();
    double lrad = mDial.getRadius();
    double srad = lrad * 0.90;
    mTicks.clear();
    
    double min = Math.min(mDial.getMinAngle(), mDial.getMaxAngle());
    double max = Math.max(mDial.getMaxAngle(), mDial.getMinAngle());
    double inc = Geometry.getAngleRangeRadians(min, max)/(mNumTicksToDraw);
    for(double ang = min; ang <= max; ang += inc)
    {
      double x1 = o.x + Math.cos(ang)*srad;
      double y1 = o.y - Math.sin(ang)*srad;
      double x2 = o.x + Math.cos(ang)*lrad;
      double y2 = o.y - Math.sin(ang)*lrad; 
      mTicks.add(new Line2D.Double(x1,y1,x2,y2));
    }
    
    double xe1 = o.x + Math.cos(max)*srad;
    double xe2 = o.x + Math.cos(max)*lrad;
    double ye1 = o.y - Math.sin(max)*srad;
    double ye2 = o.y - Math.sin(max)*lrad;
    mTicks.add(new Line2D.Double(xe1, ye1, xe2, ye2));
  }

  @Override
  public void paint(Graphics g)
  {
    Graphics2D g2 = (Graphics2D)g;
    g2.setColor(Color.red);
    for(Line2D l : mTicks)
    {
      g2.draw(l);
    }
  }

  @Override
  public EDrawPriority getPriority()
  {
    return EDrawPriority.CURSORS;
  }
}
