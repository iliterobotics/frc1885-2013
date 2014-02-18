package ilite.display.modules.display2013.overlays;

import ilite.display.modules.display2013.CompetitionDisplay2013Model;
import ilite.display.modules.display2013.data.RobotData2013;
import ilite.util.gui.components.NumberOverlay;
import ilite.util.gui.components.layer.EDrawPriority;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

import java.awt.Color;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;

public class PlatformAngles extends NumberOverlay
{
  private CompetitionDisplay2013Model mModel;
  private final double POT_MIN = 0.330709;
  private final ILog mLog = Logger.createLog(PlatformAngles.class);

  public PlatformAngles(int pWidth, int pHeight,
      CompetitionDisplay2013Model pModel)
  {
    super(pWidth, pHeight);
    mModel = pModel;
  }

  @Override
  public void paint(Graphics g)
  {
    Graphics2D g2 = (Graphics2D) g;
    g2.setFont(mFont);
    FontMetrics fm = g2.getFontMetrics();
    int y = (mHeight - 25);
    int xLSFA = (mWidth - (fm.stringWidth(mLastValue)) - 6);
    g2.setPaint(Color.CYAN);
    g2.drawString(mCurrentValue, 6, y);
    g2.setFont(mSmallFont);
    g2.drawString("CURRENT", 12, mHeight - 5);
    g2.setPaint(Color.GREEN);
    g2.setFont(mFontMod);
    int xCDIA = (mWidth / 2 - (fm.stringWidth(mCurrentValue) / 2));
    g2.drawString(mCurrentValue, xCDIA, y);
    g2.setFont(mSmallFont);
    g2.drawString("INPUT", mWidth / 2 - 10, mHeight - 5);
    g2.setPaint(Color.PINK);
    g2.setFont(mSmallFont);
    g2.drawString("LAST",
        (mWidth - fm.stringWidth("LAST") + (fm.stringWidth(mLastValue)) / 3),
        mHeight - 5);
    g2.setFont(mFont);
    g2.drawString(mLastValue, xLSFA, y);
  }

  @Override
  public EDrawPriority getPriority()
  {
    return EDrawPriority.ANNOTATIONS_MED;
  }

  public void update(RobotData2013 pData)
  {
    double last = (8.391d * mModel.getLatestShotTiltAngle() + 0.7784d);
    setLastValue(last);
    double current = (8.3981d * pData.sensors.mTiltPotentiometerValue) + 0.7784d;
    setCurrentValue(current);
    double input = (40d / (1 - POT_MIN))
        * (pData.inputs.mManualAnglePotValue - POT_MIN);
    setInputValue(input);
  }

  @Override
  public String getUnit()
  {
    return "\u00B0";
  }

  @Override
  public String getNumberFormat()
  {
    return "0.00";
  }
}
