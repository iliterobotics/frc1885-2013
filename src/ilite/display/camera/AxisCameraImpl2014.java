package ilite.display.camera;

import ilite.display.interfaces.net.EConnectionState;
import ilite.util.lang.Delegator;
import ilite.util.lang.INotify;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;
import ilite.util.physics.concepts.Hertz;

import java.awt.image.BufferedImage;
import java.io.BufferedInputStream;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import javax.imageio.ImageIO;

import Vision.Base64Encoder;

public class AxisCameraImpl2014 extends Delegator<BufferedImage>
{
  private ILog mLog = Logger.createLog(AxisCameraImpl2014.class);
  private String mPassword = null;
  private String mUsername = null;
  private String mIpAddress = "10.18.85.21";
  private double mFrameRate = 20;
  private static final String sPRE = "http://";
  private static final String sPOST = "/axis-cgi/jpg/image.cgi";

  private final Base64Encoder base64 = new Base64Encoder();
  private URL mURL = null;
  private HttpURLConnection conn = null;
  private BufferedInputStream httpIn = null;
  
  private final ScheduledExecutorService mImgGrabService = Executors.newScheduledThreadPool(1);
  
  private EConnectionState mConnectionState = EConnectionState.DISCONNECTED;
  
  /**
   * Set the target's IP address
   * @param pIpAddress
   * @throws MalformedURLException
   */
  public void setIpAddress(String pIpAddress) throws MalformedURLException
  {
    mURL = new URL(
        new StringBuilder().append(sPRE).append(pIpAddress).append(sPOST).toString());
    mIpAddress = pIpAddress; // Assign after the exception
  }
  
  /**
   * Sets the frame rate.  Re-starts the image connection thread if it has
   * already been started.
   * @param pRate
   */
  public void setFrameRate(Hertz pRate)
  {
    mFrameRate = pRate.perSecond();
    if(mConnectionState.isConnected())
    {
      disconnect();
      connect();
    }
  }
  
  /**
   * Sets the connection login info
   * @param pUsername
   * @param pPassword
   */
  public void setUserPass(String pUsername, String pPassword)
  {
    mUsername = pUsername;
    mPassword = pPassword;
  }
  
  /**
   * @return the current connection state
   */
  public EConnectionState getConnectionState()
  {
    return mConnectionState;
  }
  
  /**
   * Tests that the connection is available by connecting and establishing
   * an InputStream with it.  Will only perform this test if the URL, 
   * user & password are not null.  If the connection is valid, this method will
   * start a timed reader which pull an image off the socket at the frame rate
   * set (default=20).  Upon reading the image, the super class (Delegator) will
   * notify all subscribers
   */
  public void connect()
  {
    mLog.info("Connecting to", mIpAddress);
    testConnection();
    if(mConnectionState.isConnected())
    {
      mLog.info("Connected to " + mIpAddress);
      mImgGrabService.scheduleAtFixedRate(new Runnable()
      {
        @Override
        public void run()
        {
          try
          {
            update(grabImage()); // sends the image to the Delegator
          } catch (IOException e)
          {
            mLog.exception(e);
            disconnect();
            mConnectionState = EConnectionState.ERROR;
          }
        }
      }, 2000, (long)((1d/mFrameRate)*1000d), TimeUnit.MILLISECONDS);
    }
  }
  
  /**
   * Stops the connection; really stops the timer which connects to grab an image  
   */
  public void disconnect()
  {
    mImgGrabService.shutdown();
    mConnectionState = EConnectionState.DISCONNECTED;
  }
  
  /**
   * Checks to see if the connection is available by connecting and establishing
   * an InputStream with it.  It doesn't actually read any images.  Will only
   * perform this test if the URL, user & password are not null
   */
  private void testConnection() 
  {

    if(mURL != null && mUsername != null && mPassword != null)
    {
      
      mConnectionState = EConnectionState.DISCONNECTED;
      try {
        mConnectionState = EConnectionState.ATTEMPTING;
        conn = (HttpURLConnection)mURL.openConnection();
        conn.setRequestProperty("Authorization", "Basic " + base64.encode(mUsername + ":" + mPassword));
        httpIn = new BufferedInputStream(conn.getInputStream(), 2^13); // 8k
        mConnectionState = EConnectionState.ESTABLISHED;
      } catch (IOException e)
      {
        mConnectionState = EConnectionState.ERROR;
        mLog.exception(e);
      } finally {

        if(conn != null) {
          conn.disconnect();
        }

        if(httpIn != null) {
          try {
            httpIn.close();
          } catch(IOException e) {
            mLog.error("Unable to close httpIn, probably not a problem, just thought you should know");
            mLog.exception(e);
          }
        }
      }
    }
  }

  /**
   * Attempts connection to the camera.  Only does so if the IP, user & password
   * are not null
   * @throws IOException
   */
  public BufferedImage grabImage() throws IOException
  {
    BufferedImage result = null;
    if(mConnectionState.isConnected())
    {
      try {
        conn = (HttpURLConnection)mURL.openConnection();
        conn.setRequestProperty("Authorization", "Basic " + base64.encode(mUsername + ":" + mPassword));
        httpIn = new BufferedInputStream(conn.getInputStream(), 2^13); // 8k

        result = ImageIO.read(httpIn);    
      } finally {

        if(conn != null) {
          conn.disconnect();
        }

        if(httpIn != null) {
          try {
            httpIn.close();
          } catch(IOException e) {
            mLog.error("Unable to close httpIn, probably not a problem, just thought you should know");
            mLog.exception(e);
          }
        }
      }
    }
    return result;
  }
}
