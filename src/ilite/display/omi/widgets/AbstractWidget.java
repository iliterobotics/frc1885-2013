package ilite.display.omi.widgets;

import ilite.util.gui.RevalidationClosure;
import ilite.util.gui.components.layer.LayeredPanel;
import ilite.util.lang.IUpdate;
import ilite.util.math.RunningAverage;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Dimension;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public abstract class AbstractWidget<N extends Number> extends JPanel implements IUpdate<N>
{
  protected final RunningAverage<N> mData = new RunningAverage<N>();
  protected final Map<EPos, Component> mComponentPositions = new HashMap<EPos, Component>();
  protected final LayeredPanel mLayeredPanel = new LayeredPanel();
  
  public AbstractWidget()
  {    
    add(mLayeredPanel, BorderLayout.CENTER);
  }
    
  @Override
  public final void update(N pData)
  {
    mData.update(pData);
    updateImpl(pData);
  }
  
  protected void updateImpl(N pData)
  {
    
  }

  public void addAveragingAdjustment(EPos pPosition)
  {
    if(pPosition == EPos.VOID) return;
    
    Dimension d = getPreferredSize();
    final JSpinner spinner = new JSpinner(new SpinnerNumberModel(1, 1, 100, 4));
    mComponentPositions.put(pPosition, spinner);
    add(spinner, pPosition.border_layout());
    spinner.addChangeListener(new ChangeListener()
    {
      @Override
      public void stateChanged(ChangeEvent arg0)
      {
        mData.setRunningAverage((Integer)spinner.getValue());
        mData.resetMinMaxAverages();
      }
    });
    setPreferredSize(d);
    new RevalidationClosure(this);
  }
}
