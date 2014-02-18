package ilite.display.modules.display2013;

import ilite.display.Constants;
import ilite.display.config.ISystemConfiguration;
import ilite.display.modules.display2013.data.RobotData2013;
import ilite.display.modules.display2013.overlays.PlatformAngles;
import ilite.display.modules.display2013.overlays.WheelOverlays;
import ilite.display.omi.CameraImagePanel;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class KinectImagePanel extends JPanel {
	public final static String sHORIZONTAL_CURSOR = "2012.ui.horizontalCameraCursor";
	public final static String sVERTICAL_CURSOR = "2012.ui.verticalCameraCursor";
	private final ISystemConfiguration mSysCfg;
	private BufferedImage mNoImagePicture;
	private PlatformAngles mPlatformAngles;
	private WheelOverlays mWheelOverlays;
	private final CameraImagePanel mCameraImagePanel;

	public KinectImagePanel(CompetitionDisplay2013Model pModel, ISystemConfiguration pConfig) {
		try {
			File f = new File("images/deadPanda2.jpg");
			mNoImagePicture = ImageIO.read(f);
		} catch (IOException e) {
			e.printStackTrace();
			mNoImagePicture = null;
		}

		// change this and die
		mSysCfg = pConfig;
		Dimension d = Constants.getImageSize();
		mCameraImagePanel = new CameraImagePanel(d.width,d.height,mNoImagePicture);
		final JSlider horz = mCameraImagePanel.addHorizontalCursor(sHORIZONTAL_CURSOR,
				Color.RED);
		int h_value = mSysCfg.getInt(sHORIZONTAL_CURSOR);
		horz.setValue(h_value);

		final JSlider vert = mCameraImagePanel.addVerticalCursor(sHORIZONTAL_CURSOR,
				Color.RED);
		int v_value = mSysCfg.getInt(sVERTICAL_CURSOR);
		vert.setValue(v_value);
		mPlatformAngles = new PlatformAngles(d.width, d.height, pModel);
		mWheelOverlays = new WheelOverlays(d.width, d.height, pModel);
		mCameraImagePanel.addOverlay(mPlatformAngles);
		mCameraImagePanel.addOverlay(mWheelOverlays);
		add(mCameraImagePanel.getPanel());

		horz.addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent e) {
				mSysCfg.setConfigItem(sHORIZONTAL_CURSOR,
						Integer.toString(horz.getValue()));
			}
		});

		vert.addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent e) {
				mSysCfg.setConfigItem(sVERTICAL_CURSOR,
						Integer.toString(vert.getValue()));
			}
		});

	}
	
	public void update(RobotData2013 pData) {
		mPlatformAngles.update(pData);
		mWheelOverlays.update(pData);
	}

  /*package*/ CameraImagePanel getUpdatingImagePanel()
  {
    return mCameraImagePanel;
  }
}