package ilite.display.omi.widgets.dial;

import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.EDrawPriority;

import java.awt.Graphics;

public class DialLabels extends AbstractLayer
{

  @Override
  public void paint(Graphics g)
  {
    
  }

  @Override
  public EDrawPriority getPriority()
  {
    return EDrawPriority.ANNOTATIONS_LOW;
  }
}
