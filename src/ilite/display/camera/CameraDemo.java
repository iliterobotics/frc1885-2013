package ilite.display.camera;

import ilite.util.gui.GuiFactory;
import ilite.util.gui.components.UpdatingImageComponent;
import ilite.util.gui.components.layer.LayeredPanel;
import ilite.util.image.ImageDecoder;
import ilite.util.lang.IUpdate;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import javax.imageio.ImageIO;
import javax.imageio.stream.ImageInputStream;
import javax.swing.JFrame;

import com.sun.image.codec.jpeg.JPEGCodec;
import com.sun.image.codec.jpeg.JPEGImageDecoder;

public class CameraDemo
{
  private static ILog mLog = Logger.createLog(CameraDemo.class); 
  private static final String ip ="http://169.254.84.4:9090/stream_1";
  private static final String sCONTENT_LENGTH = "Content-Length: ";
  
  private static final long rate = 1000;
  public static final int width = 640;
  public static final int height = 480;

  public static void main(String[] pArgs)
  {    
    LayeredPanel p = new LayeredPanel();
    UpdatingImageComponent uic = new UpdatingImageComponent(p, width, height);
    
//    ImageDecoder id = new ImageDecoder();
//    id.addListener(uic);
    JFrame aFrame = GuiFactory.createanddisplayDefaultJFrame("CAMERA", p);
    aFrame.setSize(new Dimension(width, height));


    //Timer t = new Timer("IMAGE RETRIEVAL");
    //t.scheduleAtFixedRate(new ImageRetriever(id), rate, rate);
    new Thread(new ImageRetriever(null, uic)).start();
  }
  
  static class ImageRetriever implements Runnable//TimerTask
  {
 //   private final ImageDecoder mDecoder;
    private final AxixCameraImpl mCamera;
    private final IUpdate<BufferedImage> mImagePanel;
    ImageRetriever(ImageDecoder pDecoder, IUpdate<BufferedImage> pListener)
    {
//      mDecoder = pDecoder;
    	mImagePanel = pListener;
      mCamera = new AxixCameraImpl();
    }
    
    @Override
    public void run()
    {
      HttpURLConnection con = null;

      try
      {
//       URL url = new URL(ip + "/axis-cgi/jpg/image.cgi?resolution=" + width + "x" + height);
         URL url = new URL(ip);
//          img = ImageIO.read();
//       URL url = new URL(ip + "/mjpg/video.mjpg");
//       URL url = new URL(ip + mCamera.getConnectionString());
        InputStream conIn;
        DataOutputStream conOut;
        
        // Basic authentication expects base-64 encoded "user$@#$@#$@#".
        // This is "root$@#$@#$@#" or whatever the default is.
        //    String encodedLogin = "cm9vdDphZG1pbg==";

        con = (HttpURLConnection) url.openConnection();
        //    con.setRequestProperty("Authorization", "Basic " + encodedLogin);

        mLog.info(con.getResponseCode() + " " + con.getResponseMessage());

        conIn = con.getInputStream();
        BufferedInputStream bis = new BufferedInputStream(conIn);
        ByteArrayOutputStream byteArrayOut = new ByteArrayOutputStream();
        
        int c = -1;
        String line = "";
        String length_str = "";
        int img_length =0;
        byte[] img_buf = new byte[8092];
        BufferedImage img = null;
        
        StringBuilder sb = new StringBuilder();
        boolean cont = true;
        
        ImageInputStream iis = ImageIO.createImageInputStream(conIn);
        while(cont)
        {
        	while((c = bis.read()) != '\n')
        	{
        		sb.append((char)c);
        	}
        	line = sb.toString();
        	if(line.contains(sCONTENT_LENGTH))
        	{
        		length_str = line.substring(sCONTENT_LENGTH.length()).trim();
        		img_length = Integer.parseInt(length_str);
        		ByteArrayOutputStream baos = new ByteArrayOutputStream();
        		for(int i = 0; i < 2; i++)
        			bis.read();
        		bis.read(img_buf, 0, img_length);
        		ByteArrayInputStream bais = new ByteArrayInputStream(img_buf);
        		JPEGImageDecoder decoder = JPEGCodec.createJPEGDecoder(bais);
        		img = decoder.decodeAsBufferedImage();
        		baos.flush();
        		//System.out.println(img);
        		if(img != null)
        		{
        			mImagePanel.update(img);
        			img = null;
        		}
        	}
    		sb = new StringBuilder();
        }
        System.out.println("done " + byteArrayOut.size());
///        while((c = input.read()) != -1)
//        {
//        	//System.out.println(c);
//          byteArrayOut.write(c);
//        }
//        System.out.println("done " + byteArrayOut.size());


//        mDecoder.update(byteArrayOut.toByteArray());
           // conOut = con.openDataOutputStream();
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
  }
}
