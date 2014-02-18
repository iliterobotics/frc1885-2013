package ilite.display.omi.widgets.bar;

import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.EDrawPriority;
import ilite.util.lang.IUpdate;

import java.awt.Color;
import java.awt.Graphics;

/*pacakge*/ abstract class BarCursor extends AbstractLayer implements IUpdate<Double>
{
  protected double mValue = 50.0;
  private Color mColor = Color.green;
  
  /*package*/ BarCursor()
  {
    
  }
  
  public void setColor(Color pColor)
  {
    mColor = pColor;
  }

  @Override
  public void paint(Graphics g)
  {
    // TODO
  }

  @Override
  public EDrawPriority getPriority()
  {
    return EDrawPriority.CURSORS;
  }

}
