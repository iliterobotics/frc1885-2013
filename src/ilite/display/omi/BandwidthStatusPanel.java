package ilite.display.omi;

import static javax.swing.SpringLayout.EAST;
import static javax.swing.SpringLayout.NORTH;
import static javax.swing.SpringLayout.SOUTH;
import static javax.swing.SpringLayout.VERTICAL_CENTER;
import static javax.swing.SpringLayout.WEST;
import ilite.display.StateData;
import ilite.display.interfaces.net.Bandwidth;
import ilite.display.interfaces.net.EConnectionState;
import ilite.display.omi.components.IliteExpandingPanel;
import ilite.util.gui.components.EPanelOrientation;
import ilite.util.lang.IUpdate;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

import java.awt.Dimension;
import java.awt.FontMetrics;

import javax.swing.JLabel;
import javax.swing.SpringLayout;

public class BandwidthStatusPanel extends IliteExpandingPanel
{
  private final JLabel mFormattedUpRate = new JLabel("00.0 Mb/s");
  private final JLabel mFormattedDownRate = new JLabel("00.0 Mb/s");
  private final JLabel mFormattedCameraRate = new JLabel("00.0 Mb/s");
  
  private final JLabel mNumUpPackets = new JLabel("000");
  private final JLabel mNumDownPackets = new JLabel("000");

  private JLabel upNumLabel = new JLabel("U:");
  private JLabel downNumLabel = new JLabel("D:");
  private JLabel downRateLabel = new JLabel(" In:");
  private JLabel upRateLabel = new JLabel("Out:");
  private JLabel cameraRateLabel = new JLabel("Camera:");
  
  private final ILog mLog = Logger.createLog(BandwidthStatusPanel.class);

  public BandwidthStatusPanel(StateData pStateData)
  {
    super(EPanelOrientation.UP_LEFT);

    mPanel.setPreferredSize(getPanelPreferredSize());
    mPanel.add(upNumLabel);
    mPanel.add(downNumLabel);
    mPanel.add(mNumUpPackets);
    mPanel.add(mNumDownPackets);

    SpringLayout sl = new SpringLayout();
    mPanel.setLayout(sl);
    sl.putConstraint(WEST, upNumLabel, INSET, WEST, mPanel);
    sl.putConstraint(WEST, mNumUpPackets, 0, EAST, upNumLabel);
    sl.putConstraint(WEST, downNumLabel, INSET * 3, EAST, mNumUpPackets);
    sl.putConstraint(WEST, mNumDownPackets, 0, EAST, downNumLabel);
    sl.putConstraint(VERTICAL_CENTER, upNumLabel, 0, VERTICAL_CENTER, mPanel);
    sl.putConstraint(VERTICAL_CENTER, mNumUpPackets, 0, VERTICAL_CENTER, mPanel);
    sl.putConstraint(VERTICAL_CENTER, downNumLabel, 0, VERTICAL_CENTER, mPanel);
    sl.putConstraint(VERTICAL_CENTER, mNumDownPackets, 0, VERTICAL_CENTER, mPanel);

    mPopupPanel.add(upRateLabel);
    mPopupPanel.add(downRateLabel);
    mPopupPanel.add(mFormattedDownRate);
    mPopupPanel.add(mFormattedUpRate);
    mPopupPanel.add(mFormattedCameraRate);
    mPopupPanel.add(cameraRateLabel);

    SpringLayout psl = new SpringLayout();
    mPopupPanel.setLayout(psl);

    psl.putConstraint(SOUTH, mFormattedDownRate, -INSET,  SOUTH, mPopupPanel);
    psl.putConstraint(SOUTH, downRateLabel, -INSET,  SOUTH, mPopupPanel);
    
    psl.putConstraint(SOUTH, mFormattedUpRate, -INSET,   NORTH, mFormattedDownRate);
    psl.putConstraint(SOUTH, upRateLabel, -INSET,    NORTH, downRateLabel);
    
    psl.putConstraint(SOUTH, mFormattedCameraRate, -INSET , NORTH, mFormattedUpRate);
    psl.putConstraint(SOUTH, cameraRateLabel, -INSET, NORTH, upRateLabel);
    psl.putConstraint(WEST, cameraRateLabel, INSET, WEST, mPopupPanel);
    psl.putConstraint(WEST, mFormattedCameraRate, INSET, EAST, cameraRateLabel);
    
    psl.putConstraint(WEST, downRateLabel, INSET,   WEST, mPopupPanel);
    psl.putConstraint(WEST, upRateLabel, INSET, WEST,   mPopupPanel);
    psl.putConstraint(WEST, mFormattedDownRate, INSET,   EAST, downRateLabel);
    psl.putConstraint(WEST, mFormattedUpRate, INSET,  EAST, upRateLabel);

    pStateData.addIncomingBandwidthListener(new DownBwUpdate());
    pStateData.addCameraBandwidthListener(new CameraBwUpdate());
    pStateData.addOutgoingBandwidthListener(new UpBwUpdate());
  }

  @Override
  protected Dimension getPopupPreferredSize()
  {
    FontMetrics fm = mFormattedDownRate.getFontMetrics(mFormattedDownRate
        .getFont());
    int ht = fm.getHeight();
    int wd = fm.stringWidth(mFormattedDownRate.getText() + "CAMERA:");
    Dimension d = new Dimension(wd + 4 * INSET, ht * 3 + 3 * INSET);
    mLog.debug(d);
    return d;
  }

  @Override
  protected Dimension getPanelPreferredSize()
  {
    FontMetrics metrics = mFormattedDownRate.getFontMetrics(mFormattedDownRate
        .getFont());
    int mMaxTextWidth = metrics.stringWidth(mNumDownPackets.getText()
        + mNumUpPackets.getText() + upNumLabel.getText()
        + downNumLabel.getText());

    return new Dimension(mMaxTextWidth + 5 * INSET, metrics.getHeight() + 2
        * INSET);
  }

  private class DownBwUpdate implements IUpdate<Bandwidth>
  {
    public void update(Bandwidth pBw)
    {
      mFormattedDownRate.setText(pBw.getFormattedBandwidth());
      mNumDownPackets.setText(Integer.toString(pBw.getNumPackets()));
    }
  }

  private class UpBwUpdate implements IUpdate<Bandwidth>
  {
    public void update(Bandwidth pBw)
    {
      mFormattedUpRate.setText(pBw.getFormattedBandwidth());
      mNumUpPackets.setText(Integer.toString(pBw.getNumPackets()));
    }
  }
  
  private class CameraBwUpdate implements IUpdate<Bandwidth>
  {
	  public void update(Bandwidth pBw)
	  {
		  mFormattedCameraRate.setText(pBw.getFormattedBandwidth());
		  mLog.debug("[BandwidthStatusPanel] Camera Framerate: " + pBw.getPacketsPerSecond());
	  }
  }

  private class ConnectionStatusListener implements IUpdate<EConnectionState>
  {
    @Override
    public void update(EConnectionState pObject)
    {
      // Maybe enable/disable this panel? no biggie
    }

  }
}
