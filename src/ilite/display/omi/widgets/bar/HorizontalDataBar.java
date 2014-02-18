package ilite.display.omi.widgets.bar;

import ilite.util.math.RunningAverage;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;

import javax.swing.JPanel;

/*package*/ class HorizontalDataBar extends UpdatingDataBar
{

  /*package*/ HorizontalDataBar(RunningAverage pAvg)
  {
    super(pAvg);
  }
  
  @Override
  public void paint(Graphics g)
  {
    Dimension d = getPreferredSize();
    Graphics2D g2 = (Graphics2D)g;
    
    // Value
    int pv = convertValueToPx(mAvg.getRunningAverage(), EBarOrient.HORIZONTAL);
    g2.setColor(getForeground());
    g2.fillRect(0, 0, (int)pv, d.height);
    g2.setColor(getBackground());
    g2.fillRect((int)pv, 0, d.width-(int)pv, d.height);
    
    
    // Major ticks & Outline
    g2.setStroke(mAnnotationStroke);
    g2.setColor(mAnnotationsColor);
    g2.drawRect(0, 0, d.width, d.height);
    if(mShowTicks)
    {
      int tickstart = d.width / (mNumTicksToDraw);
      for(int i = 1; i < mNumTicksToDraw; i++)
      {
        g2.drawLine(tickstart*i, 0, tickstart*i, d.height/4);
      }
    }
    
    if(mShowMaxCursor)
    {
      pv = convertValueToPx(mAvg.getMaxAvg(), EBarOrient.HORIZONTAL);
      g2.drawLine(pv, 0, pv, d.height);
    }
    
    if(mShowMinCursor)
    {
      pv = convertValueToPx(mAvg.getMinAvg(), EBarOrient.HORIZONTAL);
      g2.drawLine(pv, 0, pv, d.height);
    }
    
    // Data Label
    FontMetrics fm = g2.getFontMetrics();
    int y = d.height/2 - fm.getHeight()/2;
    g2.drawString(mValueFormat.format(mAvg.getRunningAverage()), sTEXT_INSET, y);
    
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
          int xpos = convertValueToPx(nums[i], EBarOrient.HORIZONTAL);
          xpos -= fm.stringWidth(labels[i])/2;
          g.drawString(labels[i], xpos, fm.getHeight());
        }
      }
    };
    p.setPreferredSize(new Dimension(getPreferredSize().width, p.getFontMetrics(p.getFont()).getHeight()));
    return p;
  }
}
