package ilite.display.modules.display2013;

import ilite.display.config.ISystemConfiguration;

import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JPanel;

public class AnimatedImage extends JPanel
{
	BufferedImage defaultImage;
	int width, height;
	AnimatedImagePanel aip;
	public AnimatedImage(ISystemConfiguration pConfig)
	{
		String filepath = pConfig.getString("2013.images.cad");
		try
		{
			defaultImage = ImageIO.read(new File(filepath));
		} catch (IOException e)
		{
			e.printStackTrace();
			defaultImage = null;
		}
		aip = new AnimatedImagePanel(640, 480, defaultImage);
		aip.getPanel().setPreferredSize(new Dimension(640, 480));
		add(aip.getPanel());
		//System.out.println(aip.getPanel().getWidth() + "x" + aip.getPanel().getHeight() + "  13z");
		//System.out.println("Default size = " + defaultImage.getWidth() + "x" + defaultImage.getHeight());
	}
	
	public AnimatedImagePanel  getAnimatedPanel()
	{
		return aip;
	}
}
