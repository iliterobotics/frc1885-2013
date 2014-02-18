package ilite.display.omi.widgets.bar;

import javax.swing.JSlider;

public enum EBarOrient
{
  VERTICAL(JSlider.VERTICAL),
  HORIZONTAL(JSlider.HORIZONTAL);
  
  public int slider_orientation(){ return mSliderOrient; }
  
  private int mSliderOrient;
  private EBarOrient(int pJSliderOrientation)
  {
    mSliderOrient = pJSliderOrientation;
  }

}
