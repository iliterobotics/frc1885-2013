package ilite.display.camera;

import ilite.util.lang.Delegator;
import ilite.util.lang.IUpdate;

import java.awt.image.BufferedImage;
import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashSet;
import java.util.Set;

import com.sun.image.codec.jpeg.JPEGCodec;
import com.sun.image.codec.jpeg.JPEGImageDecoder;

public class GenericWebCam extends Delegator<BufferedImage> implements Runnable
{
  private Set<IUpdate<BufferedImage>> mListeners = new HashSet<>();
  
  private BufferedImage mLatest = null;
  
  private final String mURL;
  private static final String sCONTENT_LENGTH = "Content-length:";

  public GenericWebCam(String pURL)
  {
    mURL = pURL;
  }
  
  public void run()
  {
    HttpURLConnection con = null;

    try
    {
      InputStream conIn;

      URL url = new URL(mURL);
      con = (HttpURLConnection) url.openConnection();
      //    con.setRequestProperty("Authorization", "Basic " + encodedLogin);
      System.out.println("Camera Stream Connection: " + con.getResponseCode() + " " + con.getResponseMessage());

      conIn = con.getInputStream();
      BufferedInputStream bis = new BufferedInputStream(conIn);

      int c = -1;
      String line = "";
      String length_str = "";
      int img_length =0;
      byte[] img_buf;
      BufferedImage img = null;

      StringBuilder sb = new StringBuilder();

      while(true)
      {
        while((c = bis.read()) != '\n')
        {
          try
          {
            sb.append((char)c);
          } 
          catch (Throwable t)
          {
            System.out.println("[KinectImage] Throwable " + t.getMessage() + 
                " caught when extending StringBuilder passed " + sb.length());
            sb = new StringBuilder();
          }
        }
        line = sb.toString();
        System.out.println(line);
        if(line.contains(sCONTENT_LENGTH))
        {
          length_str = line.substring(sCONTENT_LENGTH.length()).trim();
          img_length = Integer.parseInt(length_str);
          
          byte trash = 0x00;
          do
          {
            trash = readSingleByte(bis);
            System.out.println(trash);
          } while (trash != 0x0d);
          
          System.out.println("Read length = " + img_length);
          img_buf = new byte[img_length];
          img_buf[0] = trash;
          readInput(bis, img_length, img_buf, 1);
//          bis.read(img_buf, 0, img_length);
          ByteArrayInputStream bais = new ByteArrayInputStream(img_buf);
          JPEGImageDecoder decoder = JPEGCodec.createJPEGDecoder(bais);
          img = decoder.decodeAsBufferedImage();
          if(img != null)
          {
            update(img);
            img = null;
          }
          else
          {
            
          }
        }
        sb = new StringBuilder();
        Thread.sleep(10);
      }
    } 
    catch (Exception e)
    {
      e.printStackTrace();
    }
    finally
    {
      if(con != null) con.disconnect();
    }
  }
  
  private byte readSingleByte(InputStream pReader) throws Exception
  {
    byte[] array = new byte[1];
    readInput(pReader, 1, array);
    return array[0];
  }
  private void readInput(InputStream acReader, int size, byte[] dataRegion, int pOffset) throws Exception
  {
    int amountRead = pOffset;
    int currentReadData = 0;
    while(amountRead < size) {
      currentReadData = acReader.read(dataRegion, amountRead, size - amountRead);
      if(currentReadData <= 0) {
        throw new IOException();
      } else {
        amountRead += currentReadData;
      }
    }
  }
  
  private void readInput(InputStream acReader, int size, byte[] dataRegion) throws Exception
  {
    int amountRead = 0;
    int currentReadData = 0;
    while(amountRead < size) {
      currentReadData = acReader.read(dataRegion, amountRead, size - amountRead);
      if(currentReadData <= 0) {
        throw new IOException();
      } else {
        amountRead += currentReadData;
      }
    }
  }
}
