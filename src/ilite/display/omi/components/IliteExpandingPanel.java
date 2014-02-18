package ilite.display.omi.components;

import ilite.util.gui.components.EPanelOrientation;
import ilite.util.gui.components.ExpandingPanel;

import java.awt.Dimension;

public abstract class IliteExpandingPanel extends ExpandingPanel
{

  public IliteExpandingPanel(EPanelOrientation  pOrientation)
  {
    super(pOrientation);
  }

  protected abstract Dimension getPanelPreferredSize();
}
