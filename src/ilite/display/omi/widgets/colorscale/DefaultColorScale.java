package ilite.display.omi.widgets.colorscale;

import ilite.display.omi.widgets.IColorScale;

import java.awt.Color;
import java.util.Hashtable;
import java.util.Map;

public class DefaultColorScale<N extends Number> implements IColorScale<N>
{
  
  private Map<N, Color> mColorCache = new Hashtable<>();
  private final double mMaxValue;
  private final double mMinValue;
  private final float[] mMinColor;
  private final float[] mMidColor;
  private final float[] mMaxColor;
  private Color mInvalidColor = Color.black;
  
  public DefaultColorScale(N pMinValue, N pMaxValue, Color pMinColor, Color pMidColor, Color pMaxColor)
  {
    mMinValue = pMinValue.doubleValue();
    mMaxValue = pMaxValue.doubleValue();
    mMinColor = pMinColor.getColorComponents(null);
    mMidColor = pMidColor.getColorComponents(null);
    mMaxColor = pMaxColor.getColorComponents(null);
  }
  
  public DefaultColorScale(N pMinValue, N pMaxValue)
  {
    this(pMinValue, pMaxValue, Color.green, Color.yellow, Color.red);
  }

  public Color toColor(N pData) {
    Color result = mInvalidColor;
    double d = pData.doubleValue();
    if(d >= mMinValue && d <= mMaxValue)
    {
      result = mColorCache.get(pData);
      if(result == null)
      {
        float pct = (float)(d/(mMaxValue-mMinValue));        
        if(pct < 0.5f)
        {          
          pct *= 2f;
          float r = mMinColor[0] * pct + mMidColor[0] * (1 - pct);
          float g = mMinColor[1] * pct + mMidColor[1] * (1 - pct);
          float b = mMinColor[2] * pct + mMidColor[2] * (1 - pct);
          result = new Color(r, g, b);
        }
        else
        {
          pct -= 0.5f;
          pct *= 2f;
          float r = mMidColor[0] * pct + mMaxColor[0] * (1 - pct);
          float g = mMidColor[1] * pct + mMaxColor[1] * (1 - pct);
          float b = mMidColor[2] * pct + mMaxColor[2] * (1 - pct);
          result = new Color(r, g, b);
        }
        mColorCache.put(pData, result);
      }
    }
    return result;
  }

}
