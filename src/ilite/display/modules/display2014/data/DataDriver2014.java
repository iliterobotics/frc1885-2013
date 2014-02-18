package ilite.display.modules.display2014.data;

import ilite.data.driver.AbstractPeriodicDataDriver;

public class DataDriver2014 extends AbstractPeriodicDataDriver<Data2014>
{
  @Override
  public Data2014 generate(float pRelativeTimeSeconds)
  {
    float value = (float)Math.sin(0.2f*pRelativeTimeSeconds);
    Integer iValue = (value < -0.25 ? -1 : value > 0.25 ? 1 : 0);
    Boolean bValue = (value < 0 ? false : true);
    Data2014 result = new Data2014();
    for(EData2014 field : Data2014.scUsedFields.values())
    {
      if(field.getTelemetryType().getFieldClass().equals(Float.class))
      {
        result.set(field, value);
      } else if (field.getTelemetryType().getFieldClass().equals(Boolean.class))
      {
        result.set(field, bValue);
      } else if (field.getTelemetryType().getFieldClass().equals(Integer.class))
      {
        result.set(field, iValue);
      }
    }
    return result;
  }
}
