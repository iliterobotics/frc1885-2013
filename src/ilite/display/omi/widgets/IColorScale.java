package ilite.display.omi.widgets;

import java.awt.Color;

public interface IColorScale<T>
{
  public Color toColor(T pData);
}
