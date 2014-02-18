package ilite.display.omi.widgets.dial;

import ilite.util.math.RunningAverage;

import java.awt.Point;


public interface IRoundDataDial
{
  /**
   * @return a Point representing the center/origin of the dial.
   * All ticks, hands, and rotary annotations should be drawn in
   * reference to this point.
   */
  public Point getOriginPoint();
  
  /**
   * @param pValue
   * @return the angle that represents the value, such that<br>
   * min <= value <= max<br>
   * The return value is an absolute value in Cartesian coordinates, and will
   * always be between 0 & 360.  The angle already takes into account the orientation
   * of the dial.
   */
  public double getAngleOfValue();
    
  public RunningAverage getData();
  public double getMinAngle();
  public double getMaxAngle();
  
  public int getRadius();
}
