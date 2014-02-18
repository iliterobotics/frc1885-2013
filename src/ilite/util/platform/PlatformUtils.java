package ilite.util.platform;

import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

import java.util.Properties;

public class PlatformUtils
{
  private static final ILog mLog = Logger.createLog(PlatformUtils.class);
  private static ESupportedPlatform mCurrentPlatform = ESupportedPlatform.UNSUPPORTED;

  public static String getDesktopLocation()
  {
    switch(mCurrentPlatform)
    {
    case WINDOWS:
      return WindowsUtils.getCurrentUserDesktopPath();
    }
    return "";
  }
  
  public static ESupportedPlatform getCurrentPlatform()
  {
    return mCurrentPlatform;
  }
  
  public static void printEnvironment()
  {
    for(String key : System.getenv().keySet())
    {
      mLog.debug(key + "= " + System.getenv(key));
    }
  }
  
  public static void printProperties()
  {

    Properties p = System.getProperties();
    for(Object o : p.keySet())
    {
      mLog.debug(o.toString() + "=" + p.get(o));
    }
  }
  
  static
  {
    if(System.getProperty("os.name").contains("Windows"))
    {
      mCurrentPlatform = ESupportedPlatform.WINDOWS;
    }
  }

  public static void main(String[] pArgs)
  {
    printEnvironment();
    printProperties();
  }
}
