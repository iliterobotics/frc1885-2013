package ilite.display.omi.widgets.labels;

import ilite.display.omi.widgets.IColorScale;
import ilite.util.lang.IUpdate;

import java.awt.Color;
import java.text.DecimalFormat;
import java.text.NumberFormat;

import javax.swing.JLabel;

public class NumberRangeStateLabel <N extends Number>  extends JLabel 
implements IUpdate<N>
{
  private final IColorScale<N> mColorScale;
  private final NumberFormat mNumberFormat;
  
  public NumberRangeStateLabel(IColorScale<N> pColorScale, NumberFormat pFormat)
  {
    mColorScale = pColorScale;
    mNumberFormat = pFormat;
  }
  
  public NumberRangeStateLabel()
  {
    this(new IColorScale<N>()
    {
      public Color toColor(N pData)
      {
        if(pData.intValue() > 0) return Color.green;
        if(pData.intValue() < 0) return Color.red;
        return Color.black;
      }
    }, new DecimalFormat("00.0"));
  }

  @Override
  public void update(N pData)
  {
    setBackground(mColorScale.toColor(pData));
    setText(mNumberFormat.format(pData));
  }
}
