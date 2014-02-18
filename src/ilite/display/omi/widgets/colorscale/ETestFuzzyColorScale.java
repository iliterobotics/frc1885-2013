package ilite.display.omi.widgets.colorscale;

import java.awt.Color;

public enum ETestFuzzyColorScale
implements IFuzzyColorLogic<Double>
{
  TARGET(new DefaultFuzzyColorScale<Double>(55d, 60d, Color.blue)),
  HIGH(new DefaultFuzzyColorScale<Double>(75d, 100d, Color.red)),
  MID(new DefaultFuzzyColorScale<Double>(25d, 75d, Color.yellow)),
  LOW(new DefaultFuzzyColorScale<Double>(10d, 25d, Color.green)),
  INVALID(new InvalidFuzzyColorScale<Double>(Color.gray));

  @Override
  public Color toColor(Double pData)
  {
    return mScale.toColor(pData);
  }

  @Override
  public boolean predicate(Double pData)
  {
    return mScale.predicate(pData);
  }
  
  private IFuzzyColorLogic<Double> mScale;
  private ETestFuzzyColorScale(IFuzzyColorLogic<Double> pScale)
  {
    mScale = pScale;
  }
}
