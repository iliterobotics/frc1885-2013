package ilite.display.modules.display2013;

import ilite.display.modules.display2013.data.RobotData2013;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

/**
 * Represents all of the potential data from the robot.
 * 
 * This is analogous to the "GlobalData2012" class
 *
 */
public class CompetitionDisplay2013Model {
  
  private LinkedList<RobotData2013> mRobotDataList = new LinkedList<RobotData2013>();
  
  private double mLastShotTiltAngle = -1d;
  private double mLastShotWheelSpeed = -1d;
  	
	/**
	 * @return the latest data from the robot
	 */
	public RobotData2013 getLatestRobotData()
	{
	  synchronized (mRobotDataList)
    {
	    return mRobotDataList.getLast();
    }
	}
	
	/**
	 * @return a list of all robot data collected thus far
	 */
	public List<RobotData2013> getAllRobotData()
	{
	  return new ArrayList<RobotData2013>(mRobotDataList);
	}

	/**
	 * Adds a piece of robot data to the end of this model's queue.  This method
	 * also checks certain states within the robot to set the 'last xyz' value
	 * for certain things on the display.
	 * 
	 * @param pData the new robot data
	 */
	public void addRobotData(RobotData2013 pData)
	{
	  synchronized (mRobotDataList)
    {
	    mRobotDataList.addLast(pData);
    }
	  
	  if(pData.pneumatics.mLaunchKickerState)
	  {
	    mLastShotTiltAngle = pData.sensors.mTiltPotentiometerValue;
	    mLastShotWheelSpeed = pData.motors.mLaunchWheelMotorPower;
	  }
	}
	
	/**
	 * Gets the tilt angle of the last {@code RobotData} which had a 
	 * {@code launchKickerState} of {@code true}
	 * 
	 * @return tilt angle in degrees
	 */
	public double getLatestShotTiltAngle()
	{
	  return mLastShotTiltAngle;
	}
  
  /**
   * Gets the wheels speed of the last {@code RobotData} which had a 
   * {@code launchKickerState} of {@code true}
   * 
   * @return wheel power as a ratio of the default wheel power, where 1.0 equals
   * the default wheel power, 0 < x < 1.0 represents underpowered, and
   * 1.0 < x < Infinity represents overpowered.  The display must convert that
   * to actual RPM if necessary.
   */
	public double getLatestShotWheelSpeed()
	{
	  return mLastShotWheelSpeed;
	}
}
