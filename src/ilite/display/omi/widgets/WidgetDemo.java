package ilite.display.omi.widgets;

import static javax.swing.SpringLayout.EAST;
import static javax.swing.SpringLayout.HORIZONTAL_CENTER;
import static javax.swing.SpringLayout.NORTH;
import static javax.swing.SpringLayout.SOUTH;
import static javax.swing.SpringLayout.VERTICAL_CENTER;
import static javax.swing.SpringLayout.WEST;
import ilite.display.omi.widgets.bar.DataBarWidget;
import ilite.display.omi.widgets.bar.EBarOrient;
import ilite.display.omi.widgets.colorscale.ColorScaleBarWidget;
import ilite.display.omi.widgets.colorscale.CompositeColorScale;
import ilite.display.omi.widgets.colorscale.DefaultColorScale;
import ilite.display.omi.widgets.colorscale.ETestFuzzyColorScale;
import ilite.display.omi.widgets.dial.DialWidget;
import ilite.util.gui.GuiFactory;
import ilite.util.lang.Delegator;

import java.awt.Color;
import java.awt.Dimension;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.SpringLayout;
import javax.swing.SwingConstants;

public class WidgetDemo extends JPanel
{
  private Delegator<Double> mDataBarUpdater = new Delegator<Double>();
  private final int inset = 2;
  
  public WidgetDemo()
  {
    SpringLayout sl = new SpringLayout();
    setLayout(sl);
    setPreferredSize(new Dimension(1424, 768));
    
    final int min = 0, max = 100, update = 16, bar_width = 100, bar_length = 600;
    Color fore = Color.black, back = Color.white;
    
    final DataBarWidget<Double> right_bar = new DataBarWidget<>(EBarOrient.HORIZONTAL, min, max);
    right_bar.setPreferredSize(new Dimension(bar_length, bar_width));
    right_bar.addCursorSlider(EPos.SOUTH, Color.cyan);
    right_bar.setRunningAverage(50);
    right_bar.addDataLabels(EPos.NORTH, 10);
    right_bar.setShowMaxValue(true);
    right_bar.setShowMinValue(true);
    right_bar.setForeground(fore);
    right_bar.setBackground(back);
    add(right_bar);
    mDataBarUpdater.addListener(right_bar);
    
    final DataBarWidget<Double> up_bar = new DataBarWidget<>(EBarOrient.VERTICAL, min, max);
    up_bar.setPreferredSize(new Dimension(bar_width, bar_length));
    up_bar.addCursorSlider(EPos.WEST, Color.cyan);
    up_bar.setForeground(fore);
    up_bar.setBackground(back);    
    add(up_bar);
    mDataBarUpdater.addListener(up_bar);

    final DataBarWidget<Double> v_bar = new DataBarWidget<>(EBarOrient.VERTICAL, min, max);
    v_bar.setPreferredSize(new Dimension(bar_width, bar_length));
    v_bar.addAveragingAdjustment(EPos.SOUTH);
    v_bar.setShowMaxValue(true);
    v_bar.setShowMinValue(true);
    v_bar.setForeground(fore);
    v_bar.setBackground(back);    
    add(v_bar);
    mDataBarUpdater.addListener(v_bar);

    final DataBarWidget<Double> v2_bar = new DataBarWidget<>(EBarOrient.VERTICAL, min, max);
    v2_bar.setPreferredSize(new Dimension(bar_width, bar_length));
    v2_bar.addDataLabels(EPos.WEST, 10);
    v2_bar.setForeground(fore);
    v2_bar.setBackground(back);
    add(v2_bar);
    mDataBarUpdater.addListener(v2_bar);
    
    final DialWidget<Double> d1 = new DialWidget<>(bar_width*2, 90,-270);
    add(d1);
    mDataBarUpdater.addListener(d1);
    d1.addAveragingAdjustment(EPos.SOUTH);
    
    final DialWidget<Double> d2 = new DialWidget<>(bar_width*2, 180, 0);
    add(d2);
    mDataBarUpdater.addListener(d2);
    
    final DialWidget<Double> d3 = new DialWidget<>(bar_width/2, -90, 90);
    add(d3);
    mDataBarUpdater.addListener(d3);

    int length = 400;
    int bins = length/20;
    int width = 36;
    IColorScale<Double> scale = new DefaultColorScale<>(0d, 100d);
    IColorScale<Double> fuzzy = new CompositeColorScale<>(ETestFuzzyColorScale.values());
    final ColorScaleBarWidget<Double> b1 = new ColorScaleBarWidget<>(scale, length, width);
    b1.setOrientation(SwingConstants.HORIZONTAL);
    b1.setDirection(SwingConstants.LEADING);
    b1.setMaxHistoryBins(bins);
    mDataBarUpdater.addListener(b1);
    add(b1);
    
    final ColorScaleBarWidget<Double> b2 = new ColorScaleBarWidget<>(scale, length, width);
    b2.setOrientation(SwingConstants.HORIZONTAL);
    b2.setDirection(SwingConstants.TRAILING);
    b2.setMaxHistoryBins(bins);
    mDataBarUpdater.addListener(b2);
    add(b2);
    
    
    final ColorScaleBarWidget<Double> b3 = new ColorScaleBarWidget<>(fuzzy, width, length);
    b3.setOrientation(SwingConstants.VERTICAL);
    b3.setDirection(SwingConstants.LEADING);
    b3.setMaxHistoryBins(bins);
    mDataBarUpdater.addListener(b3);
    add(b3);
    
    final ColorScaleBarWidget<Double> b4 = new ColorScaleBarWidget<>(fuzzy, width, length);
    b4.setOrientation(SwingConstants.VERTICAL);
    b4.setDirection(SwingConstants.TRAILING);
    b4.setMaxHistoryBins(bins);
    mDataBarUpdater.addListener(b4);
    add(b4);
   
    sl.putConstraint(NORTH, right_bar, inset, NORTH, this);
    sl.putConstraint(HORIZONTAL_CENTER, right_bar, 0, HORIZONTAL_CENTER, this);
    sl.putConstraint(WEST, up_bar, inset, EAST, right_bar);
    sl.putConstraint(VERTICAL_CENTER, up_bar, 0, VERTICAL_CENTER, this);

    sl.putConstraint(WEST, v_bar, inset, EAST, up_bar);
    sl.putConstraint(VERTICAL_CENTER, v_bar, 0, VERTICAL_CENTER, this);
    sl.putConstraint(WEST, v2_bar, inset, EAST, v_bar);
    sl.putConstraint(VERTICAL_CENTER, v2_bar, 0, VERTICAL_CENTER, this);
    
    // Top Row
    sl.putConstraint(NORTH, d1, inset, SOUTH, right_bar);
    sl.putConstraint(HORIZONTAL_CENTER, d1, 0, HORIZONTAL_CENTER, this);
    sl.putConstraint(NORTH, d2, 0, NORTH, d1);
    sl.putConstraint(EAST, d2, -inset, WEST, d1);    
    sl.putConstraint(NORTH, d3, 0, NORTH, d1);
    sl.putConstraint(WEST, d3, inset, EAST, d1);
    
    sl.putConstraint(NORTH, b1, inset, SOUTH, d1);
    sl.putConstraint(HORIZONTAL_CENTER, b1, 0, HORIZONTAL_CENTER, this);
    sl.putConstraint(NORTH, b2, inset, SOUTH, b1);
    sl.putConstraint(HORIZONTAL_CENTER, b2, 0, HORIZONTAL_CENTER, this);
    
    sl.putConstraint(NORTH, b3, inset, SOUTH, b2);
    sl.putConstraint(NORTH, b4, inset, SOUTH, b2);
    sl.putConstraint(EAST, b3, -(width/2+inset/2), HORIZONTAL_CENTER, this);
    sl.putConstraint(WEST, b4, (width/2+inset/2), HORIZONTAL_CENTER, this);
    
    
    Timer t = new Timer("DATA DRIVER TIMER");
    TimerTask tt = new TimerTask()
    {
      @Override
      public void run()
      {
        mNum++;
        if(mNum > 100) mNum = 0;
        mDataBarUpdater.update(mNum);
//        new RepaintClosure(WidgetDemo.this);
      }
    };
    t.scheduleAtFixedRate(tt, update, update);
    
    
  }
  
  public static double mNum = 0.0;

  public static void main(String[] pArgs)
  {
    JFrame f = GuiFactory.createanddisplayDefaultJFrame("TESTING", new WidgetDemo());
    GuiFactory.INST.createAutoRepaintTimer(60.0, f);
  }
}
