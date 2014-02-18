package ilite.display.camera;

import ilite.display.StateData;
import ilite.display.omi.CameraImagePanel;
import ilite.util.gui.GuiFactory;
import ilite.util.lang.Delegator;
import ilite.util.lang.IProvider;
import ilite.util.lang.IUpdate;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Timer;
import java.util.TimerTask;

import javax.imageio.ImageIO;
import javax.swing.JFrame;

import com.sun.image.codec.jpeg.JPEGCodec;
import com.sun.image.codec.jpeg.JPEGImageDecoder;

public class KinectImage implements IProvider<BufferedImage>
{
	//Warning: If you change this for no reason (especially if you don't know what you're doing), your death is imminent.
	//Thank you for your attention to this notice.
	private static ILog mLog = Logger.createLog(KinectImage.class); 
	private static final String test_ip ="http://10.18.85.12:9090/stream_1";
	private final URL mURL;
	private static final String sCONTENT_LENGTH = "Content-Length: ";

	public static final int width = 640;
	public static final int height = 480;
	public int check = 0;
	
	private final StateData mStateData;

	private static final int sDEFAULT_IMG_BUFFER_LENGTH = (int)Math.pow(2, 20);
	private final int mImageBufferLength;

	private ImageRetriever mRetriever;

	private int mTotalBytesReceived = 0;
	private int mLastReadByteValue = 0;
	private static int mSecondsSinceStart = 0;

	private final Delegator<BufferedImage> mDelegator = new Delegator<BufferedImage>();
	private BufferedImage mNoImagePicture = null;
	private final static String sDEFAULT_IMAGE_PATH = "images\\deadpanda.png";

	public static void main(String[] pArgs) throws MalformedURLException
	{    
	  StateData data = new StateData();
		final KinectImage ki = new KinectImage(new URL(test_ip), data);
		final NumberFormat f = new DecimalFormat("0.00");

		CameraImagePanel cip = new CameraImagePanel(width, height);
		ki.addListener(cip.getUpdatingImage());

		JFrame aFrame = GuiFactory.createanddisplayDefaultJFrame("CAMERA", cip.getPanel());
		aFrame.setSize(new Dimension(width, height));

		Timer t = new Timer("BandwidthCounter");
		t.schedule(new TimerTask()
		{

			@Override
			public void run()
			{
				mSecondsSinceStart++;
				int oldbytes = ki.mLastReadByteValue;
				int newbytes = ki.mTotalBytesReceived;
				double bandwidth = (double)(newbytes - oldbytes) * 8d / 1024d / 1024d ; //Mbits per second
				double total = (double)newbytes / 1024d / 1024d;
				mLog.debug(f.format(bandwidth) + "Mbits/s\tTotal=" + f.format(total) + "MB\t" + mSecondsSinceStart + "s");
				ki.mLastReadByteValue = newbytes;
			}
		}, 1000, 1000);

		ki.start();
	}
	
	public KinectImage(URL pIP, StateData pData)
	{
		this(pIP, sDEFAULT_IMG_BUFFER_LENGTH, sDEFAULT_IMAGE_PATH, pData);
	}
	
	public KinectImage(URL pIP, int pImageBufferLength, StateData pStateData)
	{
		this(pIP, pImageBufferLength, sDEFAULT_IMAGE_PATH, pStateData);
	}
	
	public KinectImage(URL pIP, int pImageBufferLength, String pDefaultLoc, StateData pStateData)
	{
		mURL = pIP;
		mStateData = pStateData;
		mImageBufferLength = pImageBufferLength;
		mLog.info("Initialized KinectImage class with a ",mImageBufferLength," byte image buffer.");
		String filepath = pDefaultLoc;
		if(filepath != null)
		{
			try{ mNoImagePicture = ImageIO.read(new File(filepath)); }
			catch(IOException e){ e.printStackTrace(); mNoImagePicture = null; }
		}
	}
	
	public void start()
	{
		mRetriever = new ImageRetriever();
		new Thread(mRetriever).start();
		mDelegator.update(mNoImagePicture);
	}
	
	public void cancel()
	{
		mRetriever.stop();
	}
	
	@Override
	public void addListener(IUpdate<BufferedImage> pListener)
	{
		mDelegator.addListener(pListener);
	}
	
	@Override
	public void removeListener(IUpdate<BufferedImage> pListener)
	{
		mDelegator.removeListener(pListener);
	}
	
	@Override
	public BufferedImage getLatest()
	{
		return mDelegator.getLatest();
	}
	
	private class ImageRetriever implements Runnable
	{
		boolean mContinueProcessing = true;

		public void stop()
		{
			mContinueProcessing = false;
		}
		@Override
		public void run()
		{
			HttpURLConnection con = null;

			try
			{
				InputStream conIn;

				con = (HttpURLConnection) mURL.openConnection();
				//    con.setRequestProperty("Authorization", "Basic " + encodedLogin);
				mLog.info("Camera Stream Connection: " + con.getResponseCode() + " " + con.getResponseMessage());

				conIn = con.getInputStream();
				BufferedInputStream bis = new BufferedInputStream(conIn);

				int c = -1;
				String line = "";
				String length_str = "";
				int img_length =0;
				byte[] img_buf;
				BufferedImage img = null;

				StringBuilder sb = new StringBuilder();

				while(mContinueProcessing)
				{
					while((c = bis.read()) != '\n')
					{
						mTotalBytesReceived++;
						try
						{
							sb.append((char)c);
						} 
						catch (Throwable t)
						{
							mLog.error("[KinectImage] Throwable " + t.getMessage() + 
									" caught when extending StringBuilder passed " + sb.length());
							sb = new StringBuilder();
						}
					}
					line = sb.toString();
					if(line.contains(sCONTENT_LENGTH))
					{
						length_str = line.substring(sCONTENT_LENGTH.length()).trim();
						img_length = Integer.parseInt(length_str);
						mTotalBytesReceived += img_length;
						for(int i = 0; i < 2; i++)
							bis.read();
						mTotalBytesReceived += 2;
						mLog.debug("[KinectImage] Read length = " + img_length);
						img_buf = new byte[img_length];
						readInput(bis, img_length, img_buf);
						mTotalBytesReceived += img_length;
						mStateData.addCameraData(mTotalBytesReceived);
						mTotalBytesReceived = 0;
//						bis.read(img_buf, 0, img_length);
						ByteArrayInputStream bais = new ByteArrayInputStream(img_buf);
						JPEGImageDecoder decoder = JPEGCodec.createJPEGDecoder(bais);
						img = decoder.decodeAsBufferedImage();
						if(img != null)
						{
							mDelegator.update(img);
							img = null;
						}
						else
						{
							mDelegator.update(mNoImagePicture);
						}
					}
					sb = new StringBuilder();
					Thread.sleep(10);
				}
			} 
			catch (Exception e)
			{

				mLog.exception(e);

			}
			finally
			{
				if(con != null) con.disconnect();
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
