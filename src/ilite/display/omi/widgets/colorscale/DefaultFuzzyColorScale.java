package ilite.display.omi.widgets.colorscale;

import java.awt.Color;

/**
 * @author JesseK
 *
 *  If a number is between min & max, the resulting color is returned
 *
 * @param <N>
 */
public class DefaultFuzzyColorScale<N extends Number> implements IFuzzyColorLogic<N>
{
  private final double mMin;
  private final double mMax;
  private final Color mColor;
  
  public DefaultFuzzyColorScale(N pMin, N pMax, Color pColor)
  {
    mMin = pMin.doubleValue();
    mMax = pMax.doubleValue();
    mColor = pColor;
  }

  @Override
  public Color toColor(N pData)
  {
    return mColor;
  }

  @Override
  public boolean predicate(N pData)
  {
    double d = pData.doubleValue();
    return (d >= mMin && d <= mMax);
  }

}
