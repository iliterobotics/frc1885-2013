package ilite.display.modules.display2014;

import ilite.data.models.RobotDebugMessage;
import ilite.display.DataProviders;
import ilite.display.interfaces.fileio.CsvDataLogger;
import ilite.display.interfaces.net.DefaultIliteClient;
import ilite.display.interfaces.net.StringMsgDecoder;
import ilite.display.modules.display2014.data.Data2014;
import ilite.display.modules.display2014.data.DataDriver2014;
import ilite.display.modules.display2014.data.TelemetryDeocoder2014;
import ilite.util.lang.IProvider;
import ilite.util.lang.IUpdate;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;
import ilite.util.physics.concepts.Hertz;
import ilite.util.platform.PlatformUtils;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.imageio.ImageIO;

public class DataManager2014 extends DefaultIliteClient
{
//  public static final int sDSIN_MSG_BYTE_TAG = 0xBADA55;

  // "Debug Feed"
  public static final int sDSIN_DEBUG_MSG_TAG = 0xD3B6FEED;
  
  private DataDriver2014 mDriver = null;
    
  private ILog mLog = Logger.createLog(DataManager2014.class);

  private TelemetryDeocoder2014 mTelemetryDecoder = new TelemetryDeocoder2014();  
  private StringMsgDecoder mDebugMessageDecoder = new StringMsgDecoder(sDSIN_DEBUG_MSG_TAG);
  private final String mRootDataDirectory;
  private String mCurrentDataDirectory;
  private String mImageDir;
  
  private CsvDataLogger<Data2014> mTelemetryLogger;
  
  public static final SimpleDateFormat sTIMED_DIRECTORY_FORMAT = 
      new SimpleDateFormat("yyyyMMMdd-kkmm");

  /**
   * @return the root folder where ALL match data is stored.  Platform-dependent.
   * On Windows it is the User's Desktop
   */
  public String getRootDataFolder()
  {
    return mRootDataDirectory;
  }
  
  /**
   * @return the sub folder of the root directory where the current session's
   * data is stored
   */
  public String getCurrentDataDirectory()
  {
    return mCurrentDataDirectory;
  }
  
  /**
   * Creates a practice match folder under the input event name.
   * @param pEventName - the event name.  If NULL, will be 'TEST'
   * @return the resulting output folder
   */
  public String setToPractice(String pEventName)
  {
    String event = (pEventName == null ? "TEST" : pEventName);
    String time = sTIMED_DIRECTORY_FORMAT.format(new Date());
    return setEventAndMatch(event, time);
  }

  /**
   * @param pEvent - includes name and year
   * @param pMatchNumber - includes prefix ('Q','QF',etc) and number
   */
  public String setEventAndMatch(String pEvent, String pMatchNumber)
  {
    String file = pMatchNumber+"-"+System.currentTimeMillis();
    if(mTelemetryLogger != null)
    {
      mTelemetryLogger.close();
      mTelemetryDecoder.removeListener(mTelemetryLogger);
    }
    mTelemetryLogger = new CsvDataLogger<>(mRootDataDirectory + pEvent + File.separator +
        pMatchNumber+ File.separator, file);
    mTelemetryDecoder.addListener(mTelemetryLogger);
    mCurrentDataDirectory = mTelemetryLogger.getOutputPath();
    mImageDir = mCurrentDataDirectory + File.separator + "Images";
    new File(mImageDir).mkdirs();
    DataProviders.INST.getCameraImageProvider().addListener(new ImageWriter());
    return mTelemetryLogger.getOutputPath();
  }
  
  private class ImageWriter implements IUpdate<BufferedImage>
  {

    @Override
    public void update(BufferedImage pObject)
    {
      if(pObject != null)
      {
        long time = System.currentTimeMillis();
        File f = new File(mImageDir, "IMAGE_" + time + ".jpg");
        try {
          ImageIO.write(pObject, "jpg", f);
        } catch (IOException e) {
          e.printStackTrace();
        }
        
      }
    }
    
    
  }

  /**
   * Sets the data rate for the data driver.  Instantiates the driver if it is
   * null
   * @param pRate
   */
  public void setDataDriverRate(Hertz pRate)
  {
    if(mDriver == null)
    {
      mDriver = new DataDriver2014();
      mDriver.addListener(mTelemetryDecoder);      
    }
    mDriver.setDataRate(pRate);
  }
  
  /**
   * Starts the data driver
   */
  public void setDriverEnabled(boolean pEnabled)
  {
    mLog.info("Starting Data Driver: " + pEnabled);
    if(mDriver == null)
    {
      mDriver = new DataDriver2014();
      mDriver.addListener(mTelemetryDecoder);      
    }
    
    if(pEnabled)
    {
      postConnectionInit();
      mDriver.start();      
    }
    else
    {
      postConnectionClose();
      mDriver.cancel();
    }
  }


  /**
   * Override this to receive notification that the socket has connected
   */
  @Override
  protected void postConnectionInit()
  {
    if(mTelemetryLogger != null)
    {
      try
      {
        mLog.debug("Starting Telemetry logger");
        mTelemetryLogger.init();
        mLog.debug("Telemetry logging started");
      } catch (IOException e)
      {
        mLog.exception(e);
      }
    }
  }

  /**
   * Override this to receive notification that the socket has closed
   */
  @Override
  protected void postConnectionClose()
  {
    if(mTelemetryLogger != null)
    {
      mLog.debug("Closing telemetry logger");
      mTelemetryLogger.close();
    }
  }

  /**
   * @return
   */
  public IProvider<Data2014> getTelemetryProvider()
  {
    return mTelemetryDecoder;
  }
  
  /**
   * @return
   */
  public IProvider<RobotDebugMessage> getDebugMsgProvider()
  {
    return mDebugMessageDecoder;
  }

  // *** Lazy Singleton.  Really lazy.

  public synchronized static DataManager2014 getInstance()
  {
    return Holder.scDataManager;
  }
  
  private static class Holder
  {
    private static final DataManager2014 scDataManager;
    
    static
    {
      scDataManager = new DataManager2014();
    }
  }
  
  /**
   * Private constructor
   */
  private DataManager2014()
  {
    addParser(mTelemetryDecoder);
    addParser(mDebugMessageDecoder);
    mRootDataDirectory =
        PlatformUtils.getDesktopLocation() + 
        File.separator + "RobotData" + File.separator;
    mCurrentDataDirectory = mRootDataDirectory;
    mLog.info("Robot data storage: " + mRootDataDirectory);
  }
}
