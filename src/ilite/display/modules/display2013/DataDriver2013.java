package ilite.display.modules.display2013;

import ilite.data.driver.AbstractPeriodicDataDriver;
import ilite.display.modules.display2013.data.RobotData2013;
import ilite.util.lang.IUpdate;

public class DataDriver2013 extends AbstractPeriodicDataDriver<RobotData2013>
{
  public DataDriver2013(final CompetitionDisplay2013Control pController)
  {
    super(50l);
    addListener(new IUpdate<RobotData2013>()
    {      
      @Override
      public void update(RobotData2013 pObject)
      {
        pController.addRobotData(pObject);
      }
    });
  }
  
  @Override
  public RobotData2013 generate(float pRelativeTimeSeconds)
  {
    float time = pRelativeTimeSeconds % 10f;
    RobotData2013 result = new RobotData2013();
    result.sensors.mTiltPotentiometerValue = time * 0.5;
    result.motors.mLaunchWheelMotorPower = time * 10f;
    return result;
  }
}
