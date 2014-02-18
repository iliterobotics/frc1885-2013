package ilite.display.omi.widgets.bar;

import ilite.display.omi.widgets.AbstractWidget;
import ilite.display.omi.widgets.EPos;
import ilite.util.gui.RevalidationClosure;
import ilite.util.gui.components.AlignmentCursorOverlay;
import ilite.util.gui.components.layer.LayeredPanel;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;

import javax.swing.JSlider;

public class DataBarWidget <N extends Number> extends AbstractWidget<N>
{
  private final UpdatingDataBar mBar;
  private final LayeredPanel mPanel;
  private final int mMaxValue;
  private final int mMinValue;
  private final EBarOrient mOrientation;
  
  public DataBarWidget(EBarOrient pOrient, int pMin, int pMax)
  {
    mMaxValue = pMax;
    mMinValue = pMin;
    mOrientation = pOrient;
    mPanel = new LayeredPanel();
    switch(pOrient)
    {
    case HORIZONTAL:
      mBar = new HorizontalDataBar(mData);
      break;
    case VERTICAL:
      mBar = new VerticalDataBar(mData);
      break;
    default:
      throw new IllegalArgumentException("Orientation " + pOrient.name() + " is not supported yet");
    }
    
    setLayout(new BorderLayout());
    add(mPanel, BorderLayout.CENTER);
        
    mBar.getRunningAverage().setMax(pMax);
    mBar.getRunningAverage().setMin(pMin);
    mPanel.addDrawLayer(mBar);
    mPanel.setAntiAliasEnabled(true);
  }

  public void addDataLabels(EPos pPosition, int pNumLabelsToDisplay)
  {
    if(pPosition == EPos.VOID) return;

    Dimension d = getPreferredSize();
    mBar.setNumTicks(pNumLabelsToDisplay);
    mBar.setDrawDataLabels(true);
    mBar.setShowTicks(true);
    Component c = mBar.getDataLabelPanel();
    mComponentPositions.put(pPosition, c);
    add(c, pPosition.border_layout());
    setPreferredSize(d);
  }
  
  public void addCursorSlider(EPos pPosition, Color pCursorColor)
  {
    if(pPosition == EPos.VOID) return;
    
    Dimension d = getPreferredSize();
    JSlider slider = new JSlider(
        mOrientation.slider_orientation(), mMinValue, mMaxValue, (mMaxValue-mMinValue)/2);
    mComponentPositions.put(pPosition, slider);
    AlignmentCursorOverlay cursor = new AlignmentCursorOverlay(mPanel, slider);
    cursor.setColor(pCursorColor);
    add(slider, pPosition.border_layout());
    mPanel.addDrawLayer(cursor);
    setPreferredSize(d);
    new RevalidationClosure(this);
  }
    
  public void setBackground(Color c)
  {
    super.setBackground(c);
    if(mBar != null) mBar.setBackground(c);
    if(mPanel != null) mPanel.setBackground(c);
  }
  
  public void setForeground(Color c)
  {
    super.setForeground(c);
    if(mBar != null) mBar.setForeground(c);
    if(mPanel != null) mPanel.setForeground(c);
  }
  
  public void setShowTicks(boolean pShowTicks)
  {
    mBar.setShowTicks(pShowTicks);
  }
  
  public void setShowMaxValue(boolean pShowMaxValue)
  {
    mBar.setShowMaxCursor(pShowMaxValue);
  }
  
  public void setShowMinValue(boolean pShowMinValue)
  {
    mBar.setShowMinCursor(pShowMinValue);
  }
  
  public void setRunningAverage(int pAvg)
  {
    mBar.getRunningAverage().setRunningAverage(pAvg);
  }

  public void setPreferredSize(Dimension d)
  {
    super.setPreferredSize(d);
    int w = d.width;
    int h = d.height;
    for(EPos p : mComponentPositions.keySet())
    {
      switch(p)
      {
      case EAST:
      case WEST:
        w -= mComponentPositions.get(p).getPreferredSize().width; break;
      case NORTH:
      case SOUTH:
        h -= mComponentPositions.get(p).getPreferredSize().height; break;
      case VOID:
      default:
      }
    }
    
    mPanel.setPreferredSize(new Dimension(w, h));
  }
  
}
