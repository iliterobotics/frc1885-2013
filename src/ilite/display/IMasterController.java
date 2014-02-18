package ilite.display;

import ilite.display.config.ISystemConfiguration;

public interface IMasterController
{
  public ISystemConfiguration getSystemConfiguration();
  
  public DataProviders getDataProviders();
  
  public StateData getStateData();
    
  public void startConnections();
  
  public void stopConnections();
}
