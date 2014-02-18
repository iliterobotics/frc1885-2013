package ilite.display.omi.widgets.labels;

import ilite.util.gui.RepaintClosure;
import ilite.util.lang.IUpdate;

import java.awt.Color;

import javax.swing.JLabel;

public class BooleanStateLabel extends JLabel implements IUpdate<Boolean>
{
  private final Color[] mStateColors;
  
  /**
   * Set the background color based upon state - 
   * TRUE = pStateColors[0], FALSE = pStateColors[1];
   * @param pStateColors
   */
  public BooleanStateLabel(Color[] pStateColors)
  {
    mStateColors = pStateColors;
    
  }
  
  /**
   * Uses default TRUE = GREEN, FALSE = RED state colors
   */
  public BooleanStateLabel()
  {
    this(new Color[]{Color.green, Color.red});
  }
  
  @Override
  public void update(Boolean pState)
  {
    setBackground(pState ? mStateColors[0] : mStateColors[1]);
    setText(pState.toString().toUpperCase());
    new RepaintClosure(this);
  } 
}
