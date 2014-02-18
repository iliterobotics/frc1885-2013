package ilite.display.modules.display2013.overlays;

import ilite.display.modules.display2013.CompetitionDisplay2013Model;
import ilite.display.modules.display2013.data.RobotData2013;
import ilite.util.gui.components.NumberOverlay;
import ilite.util.gui.components.layer.EDrawPriority;

import java.awt.Color;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;

public class WheelOverlays extends NumberOverlay
{

  private CompetitionDisplay2013Model mModel;

  public WheelOverlays(int pWidth, int pHeight,
      CompetitionDisplay2013Model pModel)
  {
    super(pWidth, pHeight);
    mModel = pModel;
  }

  @Override
  public void paint(Graphics g)
  {
    Graphics2D g2 = (Graphics2D) g;
    FontMetrics fm = g2.getFontMetrics();
    g2.setPaint(Color.PINK);
    g2.setFont(mSmallFont);
    g2.drawString("%", mWidth - 25, 50);
    g2.setFont(mFont);
    g2.drawString(mLastValue, mWidth - 110, 50);
    g2.setPaint(Color.GREEN);
    g2.setFont(mSmallFont);
    int ISWidth = (mWidth / 2 - (fm.stringWidth(mInputValue) / 2) + 15);
    int addRPM = (mWidth / 2 + (fm.stringWidth(mInputValue) / 2) + 15);
    g2.drawString("%", addRPM + 1, 50);
    g2.setFont(mFont);
    g2.drawString(mInputValue, ISWidth, 50);
    g2.setPaint(Color.CYAN);
    g2.setFont(mSmallFont);
    g2.drawString("%", fm.stringWidth(mCurrentValue) + 5, 50);
    g2.setFont(mFont);
    g2.drawString(mCurrentValue, 6, 50);
  }

  @Override
  public EDrawPriority getPriority()
  {
    return EDrawPriority.ANNOTATIONS_MED;
  }

  public void update(RobotData2013 pData)
  {
    setLastValue(mModel.getLatestShotWheelSpeed() * 100d);
    setCurrentValue(pData.motors.mLaunchWheelMotorPower * 100d);
    double input = (pData.inputs.mManualSpeedPotValue * 100d - 0.330709)
        / (1 - 0.330709);
    setInputValue(input);
  }

  @Override
  public String getUnit()
  {
    return "";
  }

  @Override
  public String getNumberFormat()
  {
    return "000";
  }
}
