package ilite.display.omi.widgets.colorscale;

import ilite.display.omi.widgets.AbstractWidget;
import ilite.display.omi.widgets.IColorScale;

import java.awt.Dimension;

/**
 * @author JesseK
 *
 * @param <T> any number class (Float, Double, etc)
 */
public class ColorScaleBarWidget<T extends Number> extends AbstractWidget<T>
{  
  private final ColorScaleBarOverlay<T> mOverlay;

  /**
   * @param pColorScale
   * @param pWidth
   * @param pHeight
   */
  public ColorScaleBarWidget(IColorScale<T> pColorScale, int pWidth, int pHeight)
  {
    super();
    mOverlay = new ColorScaleBarOverlay<>(pColorScale, pWidth, pHeight);
    Dimension d = new Dimension(pWidth, pHeight);
    setPreferredSize(d);
    mLayeredPanel.add(mOverlay);
    mLayeredPanel.setPreferredSize(d);
  }
  
  /**
   * Set direction to either SwingConstants.TRAILING or SwingConstants.LEADING,
   * which will affect how the data moves across the bar, depending upon the
   * bar's orientation.
   * <p>LEADING will display the newest data on the top or left
   * <p>TRAILING will display the newest data on the bottom or right
   * @param pDirection
   */
  public void setDirection(int pDirection)
  {
    mOverlay.setDirection(pDirection);
  }
  
  public int getDirection()
  {
    return mOverlay.getDirection();
  }
  
  /**
   * Set the bar's orientation to SwingConstants.HORIZONTAL or 
   * SwingConstants.VERTICAL
   * @param pOrientation
   */
  public void setOrientation(int pOrientation)
  {
    mOverlay.setOrientation(pOrientation);
  }
  
  public int getOrientation()
  {
    return mOverlay.getOrientation();
  }
  
  public void setDrawOutline(boolean pDrawOutline)
  {
    mOverlay.setDrawOutline(pDrawOutline);
  }
  
  public boolean getDrawOutline()
  {
    return mOverlay.getDrawOutline();
  }

  public final void updateImpl(T pData)
  {
    mOverlay.update(pData);
  }

  /**
   * @param pNumBins maximum number of colored rectangles to display
   * across the total bar.  Each bin will have a size relative to the total
   * size of the bar - i.e. if a horizontal bar is 100 pixels wide and contains
   * 5 bins, each bin rectangle color will be 20 pixels wide
   */
  public void setMaxHistoryBins(int pNumBins)
  {
    mOverlay.setMaxHistoryBins(pNumBins);
  }
  
  /**
   * @return number of history bins this bar will display
   */
  public int getMaxHistoryBins()
  {
    return mOverlay.getMaxHistoryBins();
  }
}
