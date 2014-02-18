package ilite.display.omi.widgets.bar;

import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.EDrawPriority;
import ilite.util.math.RunningAverage;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Stroke;
import java.text.DecimalFormat;
import java.text.NumberFormat;

/*package*/ abstract class UpdatingDataBar extends AbstractLayer
{
  protected static final int sTEXT_INSET = 5;
  protected boolean mShowTicks = false;
  protected boolean mShowMaxCursor = false;
  protected boolean mShowMinCursor = false;
  protected boolean mDrawLabels = false;
  protected int mNumTicksToDraw = 9;
  
  protected double mMaxValue = 100.0;
  protected double mMinValue = 0.0;
  
  protected NumberFormat mValueFormat = new DecimalFormat("0.0");
  protected Color mAnnotationsColor = Color.red;
  protected Stroke mAnnotationStroke = new BasicStroke(1.5f);
  
  protected RunningAverage mAvg = new RunningAverage();
  

  /*package*/ UpdatingDataBar(RunningAverage pDataProvider)
  {
    mAvg = pDataProvider;
  }
  
  public void setAnnotationWidth(float pSize)
  {
    mAnnotationStroke = new BasicStroke(pSize);
  }
  
  public RunningAverage getRunningAverage()
  {
    return mAvg;
  }
  
  public void setShowMaxCursor(boolean pShowMaxCursor)
  {
    mShowMaxCursor = pShowMaxCursor;
  }
  
  public void setShowMinCursor(boolean pShowMinCursor)
  {
    mShowMinCursor = pShowMinCursor;
  }
  
  public void setShowTicks(boolean pShowTicks)
  {
    mShowTicks = pShowTicks;
  }
  
  public void setDrawDataLabels(boolean pDrawLabels)
  {
    mDrawLabels = pDrawLabels;
  }
  
  /*package*/ void setNumTicks(int pNumTicks)
  {
    mNumTicksToDraw = pNumTicks;
  }

  @Override
  public EDrawPriority getPriority()
  {
    return EDrawPriority.ANNOTATIONS_LOW;
  }
  
  /*package*/ abstract Component getDataLabelPanel();
  
  protected int convertValueToPx(double pValue, EBarOrient pOrientation)
  {
    Dimension d = getPreferredSize();
    double pv = pValue / (mMaxValue - mMinValue);
    if(pOrientation == EBarOrient.HORIZONTAL)
    {
      pv *= d.width;
    }
    else
    {
      pv *= d.height;
      pv = d.height - pv;
    }
    return (int)pv;
  }

}
