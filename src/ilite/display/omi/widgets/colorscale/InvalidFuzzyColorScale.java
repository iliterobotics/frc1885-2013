package ilite.display.omi.widgets.colorscale;

import java.awt.Color;

public class InvalidFuzzyColorScale<T> implements IFuzzyColorLogic<T>
{
  private final Color mColor;
  
  public InvalidFuzzyColorScale(Color pColor)
  {
    mColor = pColor;
  }

  @Override
  public Color toColor(T pData)
  {
    return mColor;
  }

  @Override
  public boolean predicate(T pData)
  {
    return true;
  }

}
