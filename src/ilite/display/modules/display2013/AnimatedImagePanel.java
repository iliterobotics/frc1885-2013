package ilite.display.modules.display2013;

import ilite.display.modules.display2013.overlays.LabelOverlays;
import ilite.display.omi.CameraImagePanel;
import ilite.util.gui.components.UpdatingImageComponent;
import ilite.util.gui.components.layer.LayeredPanel;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JPanel;
import javax.swing.SpringLayout;

public class AnimatedImagePanel
{
	private UpdatingImageComponent mAnimatedImage;
	private LayeredPanel mImageLayers;
	private JPanel mContainer;
	private ILog mLog = Logger.createLog(CameraImagePanel.class);
	private int mImageHeight;
	private int mImageWidth;
	private SpringLayout mLayout = new SpringLayout();
	private String filepath = "2013.images.launching";
	private String startfile = "2013.images.";
	private String name;
	private final BufferedImage mDefaultImage;

	public AnimatedImagePanel(int pImageWidth, int pImageHeight, BufferedImage pDefaultImage)
	{
		mDefaultImage = pDefaultImage;
		mImageWidth = pImageWidth;
		mImageHeight = pImageHeight;

		mImageLayers = new LayeredPanel();
		mAnimatedImage = new UpdatingImageComponent(mImageLayers, pImageWidth, pImageHeight);

		mImageLayers.setPreferredSize(new Dimension(pImageWidth, pImageHeight));
		mImageLayers.addDrawLayer(mAnimatedImage);

		mContainer = new JPanel(mLayout);
		mContainer.add(mImageLayers);
		mLayout.putConstraint(SpringLayout.NORTH, mImageLayers, 0, SpringLayout.NORTH, mContainer);
		mLayout.putConstraint(SpringLayout.WEST, mImageLayers, 5, SpringLayout.WEST, mContainer);
		mLayout.putConstraint(SpringLayout.EAST, mImageLayers, 0, SpringLayout.EAST, mContainer);
		mLayout.putConstraint(SpringLayout.SOUTH, mImageLayers, 0, SpringLayout.SOUTH, mContainer);

		mAnimatedImage.update(pDefaultImage);
		mImageLayers.addDrawLayer(new LabelOverlays(640, 480, getImageFile()));
	}
	public JPanel getPanel()
	{
		return mContainer;
	}
	
	public void startUpdate(String path)
	{
		filepath = path;
		BufferedImage img = null;
		try{
			img = ImageIO.read(new File(path));
		}
		catch(IOException e){
			e.printStackTrace();
			img = mDefaultImage;
		}
		mAnimatedImage.update(img);
	}
	public String getImageFile()
	{
		if(filepath.equals("images/HangingCropped.jpg"))
		{
			name = "READY TO HANG";
			return name;
		}
		else if(filepath.equals("images/LoadingFromFloorCropped.jpg"))
		{
			name = "READY TO LOAD";
			return name;
		}
		else if(filepath.equals("images/DefaultPositionCropped.jpg"))
		{
			name = "READY";
			return name;
		}
		else if(filepath.equals("images/ReadyToLaunchCropped.jpg"))
		{
			name = "READY TO LAUNCH";
			return name;
		}
		else
		{
			name = "default";
			return name;
		}
	}
}
