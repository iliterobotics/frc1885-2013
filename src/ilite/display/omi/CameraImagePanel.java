package ilite.display.omi;

import ilite.util.gui.RevalidationClosure;
import ilite.util.gui.components.AlignmentCursorOverlay;
import ilite.util.gui.components.UpdatingImageComponent;
import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.LayeredPanel;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.SpringLayout;

public class CameraImagePanel 
{
//  private enum Orientation {VERTICAL, HORIZONTAL};
  
	private UpdatingImageComponent mCameraImage;
	private LayeredPanel mImageLayers;
	private JPanel mContainer;
	private ILog mLog = Logger.createLog(CameraImagePanel.class);
	private Map<String, JSlider> mVerticalSliders = new HashMap<String, JSlider>();
	private Map<String, JSlider> mHorizontalSliders = new HashMap<String, JSlider>();
	private int mImageHeight;
	private int mImageWidth;
	private SpringLayout mLayout = new SpringLayout();  
	private String mLastAdded = null;

	
	public CameraImagePanel(int pImageWidth, int pImageHeight)
	{
		this(pImageWidth, pImageHeight, null);
	}
	public CameraImagePanel(int pImageWidth, int pImageHeight, BufferedImage pDefaultImage)
	{
		mImageWidth = pImageWidth;
		mImageHeight = pImageHeight;

		mImageLayers = new LayeredPanel();
		mCameraImage = new UpdatingImageComponent(mImageLayers, pImageWidth, pImageHeight);

		mImageLayers.setPreferredSize(new Dimension(pImageWidth, pImageHeight));
		mImageLayers.addDrawLayer(mCameraImage);

		mContainer = new JPanel(mLayout);
		mContainer.add(mImageLayers);
		mLayout.putConstraint(SpringLayout.NORTH, mImageLayers, 0, SpringLayout.NORTH, mContainer);
		mLayout.putConstraint(SpringLayout.WEST, mImageLayers, 0, SpringLayout.WEST, mContainer);

		mContainer.setPreferredSize(getPreferredSize());
		mLog.debug(mContainer.getPreferredSize());
		
		mCameraImage.update(pDefaultImage);
	}
	
	public final JSlider addVerticalCursor(String pName, Color pColor)
	{
		JSlider slider = new JSlider(JSlider.HORIZONTAL, 1, mImageWidth, mImageWidth/2);
		slider.setPreferredSize(new Dimension(mImageWidth, slider.getPreferredSize().height));
		AlignmentCursorOverlay cursor = new AlignmentCursorOverlay(mImageLayers, slider);
		cursor.setColor(pColor);
		mImageLayers.addDrawLayer(cursor);
		mContainer.add(slider);
		JComponent ref = mImageLayers;
		if(!mHorizontalSliders.isEmpty())
		{
			ref = mHorizontalSliders.get(mLastAdded);
		}
		mLog.debug(ref);

		mLayout.putConstraint(SpringLayout.NORTH, slider, 0, SpringLayout.SOUTH, ref);
		mLayout.putConstraint(SpringLayout.WEST, slider, 0, SpringLayout.WEST, mContainer);

		mHorizontalSliders.put(pName, slider);
		mLastAdded = pName;

		mContainer.setPreferredSize(getPreferredSize());
		new RevalidationClosure(mContainer);
		return slider;
	} 
	
	public final JSlider addHorizontalCursor(String pName, Color pColor)
	{
		JSlider slider = new JSlider(JSlider.VERTICAL, 1, mImageHeight, mImageHeight/2);
		slider.setPreferredSize(new Dimension(slider.getPreferredSize().width, mImageHeight));
		AlignmentCursorOverlay cursor = new AlignmentCursorOverlay(mImageLayers, slider);
		cursor.setColor(pColor);
		mImageLayers.addDrawLayer(cursor);
		mContainer.add(slider);
		JComponent ref = mImageLayers;
		if(!mVerticalSliders.isEmpty())
		{
			ref = mVerticalSliders.get(mLastAdded);
		}
		mLog.debug(ref);

		mLayout.putConstraint(SpringLayout.NORTH, slider, 0, SpringLayout.NORTH, mContainer);
		mLayout.putConstraint(SpringLayout.WEST, slider, 0, SpringLayout.EAST, ref);

		mVerticalSliders.put(pName, slider);
		mLastAdded = pName;

		mContainer.setPreferredSize(getPreferredSize());
		new RevalidationClosure(mContainer);
		return slider;
	}

	public Dimension getPreferredSize()
	{
		Dimension d = mImageLayers.getPreferredSize();

		int w = d.width;
		for(JSlider s : mVerticalSliders.values())
		{
			w += s.getPreferredSize().width;
		}
		int h = d.height;
		for(JSlider s : mHorizontalSliders.values())
		{
			h += s.getPreferredSize().height;
		}
		return new Dimension(w, h);
	}
	
	public UpdatingImageComponent getUpdatingImage()
	{
	  return mCameraImage;
	}

	public JPanel getPanel()
	{
		return mContainer;
	}

	public void addOverlay(AbstractLayer pLayer)
	{
		mImageLayers.addDrawLayer(pLayer);
	}
}
