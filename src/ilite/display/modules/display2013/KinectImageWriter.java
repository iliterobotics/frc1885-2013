package ilite.display.modules.display2013;

import ilite.util.lang.Delegator;
import ilite.util.lang.IProvider;
import ilite.util.lang.IUpdate;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

import javax.imageio.ImageIO;

public class KinectImageWriter implements IUpdate<BufferedImage>{
	private final File mPathFile;
	
	public KinectImageWriter(IProvider<BufferedImage> pProvider, String pPath)
	{
		String path;
		if(pPath != null)
		{
			path = pPath;
		}
		else
		{
			path = "matchdata\\images\\";
		}
		
		mPathFile = new File(path);
		if(!mPathFile.exists())
		{
			mPathFile.mkdirs();
		}
		pProvider.addListener(this);
	}

	@Override
	public void update(BufferedImage pObject) 
	{
		long time = System.currentTimeMillis();
		File f = new File(mPathFile, "IMAGE_" + time + ".jpg");
		try {
			ImageIO.write(pObject, "jpg", f);
			System.out.println("[KinectImageWriter] Writing at " + 1000/(time - sLAST) + "fps");
			sLAST = time;
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public static void main(String[] pArgs) throws IOException
	{
		final Delegator<BufferedImage> p = new Delegator<BufferedImage>();
		final KinectImageWriter kiw = new KinectImageWriter(p, null);
		
		File img = new File("images\\deadpanda2.jpg");
		final BufferedImage towrite = ImageIO.read(img);
		
		TimerTask tt = new TimerTask() {			
			@Override
			public void run() {
				
				p.update(towrite);
				
			}
		};
		
		long time = 1000 / 7;
		new Timer().scheduleAtFixedRate(tt, time, time);
	}

	private static long sLAST = -1;
}
