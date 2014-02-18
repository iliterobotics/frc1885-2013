package ilite.display;

import ilite.display.modules.display2013.data.RobotCommand;
import ilite.util.lang.IProvider;

import java.awt.image.BufferedImage;
import java.util.HashMap;
import java.util.Map;

public class DataProviders
{

  public static final DataProviders INST = new DataProviders();
  
  private IProvider<BufferedImage> mRobotCameraImageProvider = null;
  
  @SuppressWarnings("rawtypes")
private Map<Class, IProvider> mProviders = new HashMap<Class, IProvider>();
  
  private DataProviders()
  {
	  
  }
  
  public void setCameraImageProvider(IProvider<BufferedImage> pProvider)
  {
    mRobotCameraImageProvider = pProvider;
  }
  
  public IProvider<BufferedImage> getCameraImageProvider()
  {
    return mRobotCameraImageProvider;
  }
  
  public void registerRobotCommandProvider(IProvider<RobotCommand> pProvider)
  {
	  mProviders.put(RobotCommand.class, pProvider);
  }
  
  @SuppressWarnings("unchecked")
public <T> IProvider<T> getProvider(Class<T> pDataType)
  {
	  return mProviders.get(pDataType);
  }
  
}
