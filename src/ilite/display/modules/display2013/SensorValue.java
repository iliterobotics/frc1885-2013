package ilite.display.modules.display2013;

import ilite.util.math.RunningAverage;

import java.text.DecimalFormat;
import java.text.NumberFormat;

/**
 * @author JesseK
 *
 *  This class defines a tilt angle 
 */
public class SensorValue
{
  private double mAngle = 90.0;
  private boolean mMode = true;
  
  private RunningAverage mRunningAverage = new RunningAverage();
  
  private static final NumberFormat sFORMAT = new DecimalFormat("00.0");

  public SensorValue()
  {
  }
  
  public SensorValue(double pValue)
  {
    mAngle = pValue;
    mRunningAverage.update(pValue);
  }
  
  public void setRunningAverageLength(int pLength)
  {
    mRunningAverage.setRunningAverage(pLength);
  }

  public void setTiltAngle(double angle)
  {
    mAngle = angle;
    mRunningAverage.update(angle);
  }
  
  public String getTiltAngle()
  { return mAngle + "";}
  
  public double getLatestTiltAngle()
  {
    return mAngle;
  }
  
  public double getAverageTiltAngle()
  {
    return mRunningAverage.getRunningAverage();
  }

  public void setMode(boolean mode)
  {
    mMode = mode;
  }

  public boolean getMode()
  {
    return mMode;
  }

  public String toString()
  {
    return sFORMAT.format(mRunningAverage.getRunningAverage());
  }

}
