package ilite.display.omi.widgets.bar;

import ilite.util.math.RunningAverage;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;

import javax.swing.JPanel;

public class VerticalDataBar extends UpdatingDataBar
{
  /*package*/ VerticalDataBar(RunningAverage pAvg)
  {
    super(pAvg);
  }
  
  public void paint(Graphics g)
  {
    Dimension d = getPreferredSize();
    Graphics2D g2 = (Graphics2D)g;
    
    // Value
    int pv = convertValueToPx(mAvg.getRunningAverage(), EBarOrient.VERTICAL);
    g2.setColor(getForeground());
    g2.fillRect(0, pv, d.width, d.height - pv);
    g2.setColor(getBackground());
    g2.fillRect(0, 0, d.width, pv);
    
    
    // Major ticks & Outline
    g2.setStroke(mAnnotationStroke);
    g2.setColor(mAnnotationsColor);
    g2.drawRect(0, 0, d.width, d.height);
    if(mShowTicks)
    {
      int tickstart = d.height / (mNumTicksToDraw);
      for(int i = 1; i < mNumTicksToDraw; i++)
      {
        g2.drawLine(0, tickstart*i, d.width/4, tickstart*i);
      }
    }
    
    if(mShowMaxCursor)
    {
      pv = convertValueToPx(mAvg.getMaxAvg(), EBarOrient.VERTICAL);
      g2.drawLine(0, pv, d.width, pv);
    }
    
    if(mShowMinCursor)
    {
      pv = convertValueToPx(mAvg.getMinAvg(), EBarOrient.VERTICAL);
      g2.drawLine(0, pv, d.width, pv);
    }
    
    // Data Label
    FontMetrics fm = g2.getFontMetrics();
    String s = mValueFormat.format(mAvg.getRunningAverage());
    int x = d.width/2 - fm.stringWidth(s)/2;
    int y = sTEXT_INSET + fm.getHeight();
    g2.drawString(s, x, y);
    
  }
  
  /*package*/ Component getDataLabelPanel()
  {
    final String[] labels = new String[mNumTicksToDraw + 1];
    final double[] nums = new double[mNumTicksToDraw + 1];
    for(int i = 0; i < labels.length; i++)
    {
      nums[i] = (double)i*(mMaxValue-mMinValue)/(double)mNumTicksToDraw;
      labels[i] = mValueFormat.format(nums[i]);
    }
    JPanel p = new JPanel()
    {
      @Override public void paint(Graphics g)
      {
        FontMetrics fm = g.getFontMetrics();
        for(int i = 0; i < labels.length; i++)
        {
          int ypos = convertValueToPx(nums[i], EBarOrient.VERTICAL);
          ypos = Math.max(ypos, fm.getHeight());
          g.drawString(labels[i], 0, ypos);
        }
      }
    };
    int mMaxLabelWidth = 0;
    for(int i = 0; i < labels.length; i++)
    {
      FontMetrics fm = p.getFontMetrics(p.getFont());
      mMaxLabelWidth = Math.max(fm.stringWidth(labels[i]), mMaxLabelWidth);
    }
    p.setPreferredSize(new Dimension(mMaxLabelWidth, getPreferredSize().height));
    return p;
  }
}
