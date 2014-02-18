package ilite.display.omi;

import ilite.display.Constants;
import ilite.display.IMasterController;
import ilite.display.config.ISystemConfiguration;
import ilite.display.config.UIOptions;
import ilite.util.gui.RevalidationClosure;
import ilite.util.lang.IUpdate;

import java.awt.Dimension;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SpringLayout;

public class UtilityStatusBar
{

  private JPanel mContainer = new JPanel();

  private BandwidthStatusPanel mBandwidthStatus;
  private static JLabel alliance = new JLabel("");
  private static JLabel timeLabel = new JLabel("[TIME]");

  public UtilityStatusBar(IMasterController pMaster)
  {
    mBandwidthStatus = new BandwidthStatusPanel(pMaster.getStateData());

    SpringLayout sl = new SpringLayout();
    mContainer.setLayout(sl);
    JPanel bandwidth = mBandwidthStatus.getPanel();
    mContainer.add(bandwidth);
    mContainer.add(timeLabel);
    bandwidth.setBackground(UIOptions.darkBlob);

    sl.putConstraint(SpringLayout.SOUTH, bandwidth, -1, SpringLayout.SOUTH,
        mContainer);

    sl.putConstraint(SpringLayout.EAST, bandwidth, -1, SpringLayout.EAST,
        mContainer);
    sl.putConstraint(SpringLayout.SOUTH, timeLabel, -1, SpringLayout.SOUTH, mContainer);
    sl.putConstraint(SpringLayout.WEST, timeLabel, 650, SpringLayout.WEST, mContainer);

    int width = pMaster.getStateData().getParentWindowSize().width;
    mContainer.setPreferredSize(new Dimension(width,
        bandwidth.getPreferredSize().height));

    IUpdate<String> fontListener = new IUpdate<String>()
    {
      @Override
      public void update(String pSize)
      {
        new RevalidationClosure(mContainer);
      }
    };
    ISystemConfiguration syscfg = pMaster.getSystemConfiguration();
    syscfg.addPropertyChangeListener(Constants.sLARGE_FONT_SIZE_KEY, fontListener);
    syscfg.addPropertyChangeListener(Constants.sSMALL_FONT_SIZE_KEY, fontListener);
  }
  
  public static void setText(String time)
  {
	  timeLabel.setText(time);
  }
  
//  public static void addComponent(JLabel x)
//  {
//	  timeLabel = x;
//  }

  public JPanel getPanel()
  {
    return mContainer;
  }
}
