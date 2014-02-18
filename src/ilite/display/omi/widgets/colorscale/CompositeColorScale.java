package ilite.display.omi.widgets.colorscale;

import ilite.display.omi.widgets.IColorScale;

import java.awt.Color;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * @author JesseK
 *
 *  Iterates over a list of IFuzzyColorLogic<T> interfaces to arrive at the
 *  final color for a piece of data.  This class will stop at the FIRST instance
 *  where IFuzzyColorLogic.predicate() returns <code>true</code>, so it is up
 *  to the creator to ensure this class is constructed with an array or list that
 *  is in the appropriate order.
 * @param <T>
 */
public class CompositeColorScale <T> implements IColorScale<T>
{
  private final List<IFuzzyColorLogic<T>> mColorScales;

  /**
   * Array-based constructor.  Can be used with enumerations that implement
   * IFuzzyColorLogic<T> by calling Enum.values()
   * @param pColorScales
   */
  public CompositeColorScale(IFuzzyColorLogic<T>[] pColorScales)
  {
    mColorScales = Arrays.asList(pColorScales);
  }
  
  /**
   * Copies the List to an ArrayList, so the caller may use an expensive list
   * implementatio to ensure proper order
   * @param pColorScales
   */
  public CompositeColorScale(List<IFuzzyColorLogic<T>> pColorScales)
  {
    mColorScales = new ArrayList<>(pColorScales);
  }

  @Override
  public Color toColor(T pData)
  {
    Color result = null;
    for(IFuzzyColorLogic<T> scale : mColorScales)
    {
      if(scale.predicate(pData))
      {
        result = scale.toColor(pData);
        break;
      }
    }
    return result;
  }

}
