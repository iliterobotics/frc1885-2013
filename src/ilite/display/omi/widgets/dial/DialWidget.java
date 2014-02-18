package ilite.display.omi.widgets.dial;

import ilite.display.omi.widgets.AbstractWidget;
import ilite.util.gui.components.EPanelOrientation;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;
import ilite.util.math.Geometry;
import ilite.util.math.Geometry.QUADRANT;
import ilite.util.math.RunningAverage;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.Shape;
import java.awt.geom.Arc2D;
import java.awt.geom.Ellipse2D;
import java.util.ArrayList;
import java.util.List;

import javax.swing.BorderFactory;

public class DialWidget <N extends Number> extends AbstractWidget<N> implements IRoundDataDial
{
  private Point mOrigin;
  private double mMinValue = 0.0;
  private double mMaxValue = 100.0;
  private double mStartAngleRadians = 0.0; // Radians
  private double mEndAngleRadians = 0.0; // Radians
  private int mRadius = 0;
  private ILog mLog = Logger.createLog(DialWidget.class);
  private static final int INSET = 5;
  
  private List<DialHand> mHands = new ArrayList<DialHand>();
  private List<DialLabels> mLabels = new ArrayList<DialLabels>();
  private List<DialTicks> mTicks = new ArrayList<DialTicks>();
  private DialBackground mBackground;
  
  /**
   * @param pOrientation<br>
   * CENTER = Full 360-dial<br>
   * UP_CENTER/DOWN_CENTER/LEFT_CENTER/RIGHT_CENTER are half-dials<br>
   * UP_LEFT/UP_RIGHT/etc are quarter dials 
   */
  public DialWidget(int pRadiusPx, double pStartAngleDegrees, double pEndAngleDegrees)
  {
    mStartAngleRadians = Math.toRadians(pStartAngleDegrees);
    mEndAngleRadians = Math.toRadians(pEndAngleDegrees);
    mRadius = pRadiusPx;        
    
    Shape s = getDialShape();
    mBackground = new DialBackground(this,s);
    mLayeredPanel.addDrawLayer(mBackground);
    addHand(new DialHand(this));
    addTicks(new DialTicks(this));
    mLayeredPanel.setBorder(BorderFactory.createLineBorder(Color.RED));
    mLayeredPanel.setAntiAliasEnabled(true);
  }
    
  public final void addHand(DialHand pHand)
  {
    mHands.add(pHand);
    mLayeredPanel.addDrawLayer(pHand);    
  }
  
  public final void addLabels(DialLabels pLabels)
  {
    mLabels.add(pLabels);
    mLayeredPanel.addDrawLayer(pLabels);
  }
  
  public final void addTicks(DialTicks pTicks)
  {
    mTicks.add(pTicks);
    mLayeredPanel.addDrawLayer(pTicks);
  }

  @Override
  public Point getOriginPoint()
  {
    return mOrigin;
  }

  @Override
  public double getAngleOfValue()
  {
    double value = Math.max(Math.min(mData.getRunningAverage(), mMaxValue), mMinValue); // clamp
    double ratio = value / (mMaxValue - mMinValue);
    double angle = ratio * (mEndAngleRadians - mStartAngleRadians) + mStartAngleRadians;
    return angle;
  }
  
  @Override
  public RunningAverage<N> getData()
  {
    return mData;
  }

  @Override
  public double getMinAngle()
  {
    return mStartAngleRadians;
  }
  
  public void setMinAngle(double pMinAngle)
  {
    mStartAngleRadians = pMinAngle;
  }

  @Override
  public double getMaxAngle()
  {
    return mEndAngleRadians;
  }
  
  public void setMaxAngle(double pMaxAngle)
  {
    mEndAngleRadians = pMaxAngle;
  }

  public int getRadius()
  {
    return mRadius;
  }
  
  private Shape getDialShape()
  {
    
    int maxy = Geometry.round(Geometry.getMaxY(mStartAngleRadians, mEndAngleRadians, mRadius));
    int minx = Geometry.round(Geometry.getMinX(mStartAngleRadians, mEndAngleRadians, mRadius));
    int x = INSET - (mRadius - Math.abs(minx)); // Arc drawing offset
    int y = INSET - (mRadius - Math.abs(maxy)); // Arc drawing offset

    EPanelOrientation orient = getDialOrientation();
    switch(orient)
    {
    case UP_CENTER:     mOrigin = new Point(Math.abs(minx), maxy); break;
    case LEFT_CENTER:   mOrigin = new Point(Math.abs(minx), maxy); break;
    case DOWN_CENTER:   mOrigin = new Point(Math.abs(minx), 0); break;
    case RIGHT_CENTER:  mOrigin = new Point(0, maxy); break;
    case UP_RIGHT:      mOrigin = new Point(0, maxy); break;
    case UP_LEFT:       mOrigin = new Point(Math.abs(minx), maxy); break;
    case DOWN_LEFT:     mOrigin = new Point(Math.abs(minx), 0); break;
    case DOWN_RIGHT:    mOrigin = new Point(0,0); break;
    case CENTERED:      mOrigin = new Point(Math.abs(minx), maxy); break;
    } 

    double diff = Geometry.getAngleRangeRadians(mStartAngleRadians, mEndAngleRadians);
    double dia = mRadius * 2;
    Shape shape = null;
    if(diff % Geometry.TWO_PI == 0)
    {
      shape = new Ellipse2D.Double(INSET, INSET, dia, dia);
    }
    else
    {
      diff = Math.toDegrees(diff);
      shape = new Arc2D.Double(x, y , dia, dia, Math.toDegrees(mStartAngleRadians), diff,  Arc2D.PIE);
    }
    

    //Do insets here so other calcs don't worry about it
    mOrigin.x += INSET;
    mOrigin.y += INSET;
    Dimension d = Geometry.getMaxDimensionWithOrigin(mStartAngleRadians, mEndAngleRadians, mRadius);
    d.width += 2*INSET;
    d.height += 2*INSET;
    mLayeredPanel.setPreferredSize(d);
    return shape;
  }
  
  private EPanelOrientation getDialOrientation()
  {
    QUADRANT start_quad = Geometry.getQuadrantOfAngle(mStartAngleRadians);
    QUADRANT end_quad = Geometry.getQuadrantOfAngle(mEndAngleRadians);
    
    double start = Math.toDegrees(mStartAngleRadians);
    double end = Math.toDegrees(mEndAngleRadians);
    // Checking boundary conditions on the definition of a quadrant
    if(start != 0 && end > start && (start % 90 == 0))
    { // Hackish, but it's a boundary condition
      start_quad = start_quad.next();
    }
    
    if(end != 0 && end % 360 == 0)
    {
      end_quad = QUADRANT.Q4;
    }
    
    if(end_quad == start_quad && Math.abs(end - start) <= 90)
    {// 1-quadrant
      switch(start_quad)
      {
      case Q1: return EPanelOrientation.UP_RIGHT;
      case Q2: return EPanelOrientation.UP_LEFT;
      case Q3: return EPanelOrientation.DOWN_LEFT;
      case Q4: return EPanelOrientation.DOWN_RIGHT;
      }
    }
    else if(start_quad.next() == end_quad)
    { // 2-quadrant
      switch(start_quad)
      {
      case Q1: return EPanelOrientation.UP_CENTER;
      case Q2: return EPanelOrientation.LEFT_CENTER;
      case Q3: return EPanelOrientation.DOWN_CENTER;
      case Q4: return EPanelOrientation.RIGHT_CENTER;
      }
    }
    return EPanelOrientation.CENTERED;
  }
}
