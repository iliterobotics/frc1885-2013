package ilite.display.omi.widgets.colorscale;

import ilite.display.omi.widgets.IColorScale;
import ilite.util.gui.ColoredAwtRectangle;
import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.EDrawPriority;
import ilite.util.lang.IUpdate;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import javax.swing.SwingConstants;

/**
 * @author JesseK
 *
 *  Creates a 'waterfall' bar which uses a color scale to show how data changes
 *  over time (or as each 'update' call is made).  This overlay does NOT handle
 *  tick marks, x/y scale labels, or a color scale legend - it only paints the
 *  colorscale bar itself.
 *
 * @param <T>
 */
public class ColorScaleBarOverlay<T> extends AbstractLayer implements IUpdate<T>
{

  protected LinkedList<Color> mColors = new LinkedList<>();
  private final IColorScale<T> mColorScale;
  private int mNumBins = 100;
  protected int mPixelsPerBin;
  protected final int mWidth;
  protected final int mHeight;
  private List<ColoredAwtRectangle> mDrawBuffer = new ArrayList<>();
  private List<ColoredAwtRectangle> mFillBuffer = new ArrayList<>();
  private int mOrientation = SwingConstants.HORIZONTAL;
  private int mDirection = SwingConstants.LEADING;
  private boolean mDrawOutline = false;
  private static final NumberFormat sFORMAT = new DecimalFormat("000.00");
  private T mLatestValue = null;
  private final int mLabelY;
  private static final int sFONT_SIZE = 32;
  private static final Font sFONT = new Font(Font.MONOSPACED, Font.PLAIN, sFONT_SIZE);
  private final int mX0;
  private final int mY0;

  /**
   * Instantiates a colorscale bar a 0,0 - really for use by the ColorScaleBarWidget
   * class
   * 
   * @param pColorScale
   * @param pWidth
   * @param pHeight
   */
  public ColorScaleBarOverlay(IColorScale<T> pColorScale, int pWidth, int pHeight)
  {
    this(pColorScale, pWidth, pHeight, 0, 0);
  }
  
  /**
   * Preferred constructor
   * 
   * @param pColorScale - converts a <Number> to a color
   * @param pWidth - width of the bar
   * @param pHeight - height of the bar
   * @param pX0 - starting X of the bar (upper left corner)
   * @param pY0 - starting Y of the bar (upper left corner)
   */
  public ColorScaleBarOverlay(IColorScale<T> pColorScale, int pWidth, int pHeight, int pX0, int pY0)
  {
    mColorScale = pColorScale;
    mWidth = pWidth;
    mHeight = pHeight;
    mLabelY = mHeight/2 + sFONT_SIZE/2;
    mPixelsPerBin = mWidth / mNumBins;
    setPreferredSize(new Dimension(pWidth, pHeight));
    mX0 = pX0;
    mY0 = pY0;
  }
  
  /**
   * Set direction to either SwingConstants.TRAILING or SwingConstants.LEADING,
   * which will affect how the data moves across the bar, depending upon the
   * bar's orientation.
   * <p>LEADING will display the newest data on the top or left
   * <p>TRAILING will display the newest data on the bottom or right
   * @param pDirection
   */
  public void setDirection(int pDirection)
  {
    mDirection = pDirection;
  }
  
  public int getDirection()
  {
    return mDirection;
  }
  
  /**
   * Set the bar's orientation to SwingConstants.HORIZONTAL or 
   * SwingConstants.VERTICAL
   * @param pOrientation
   */
  public void setOrientation(int pOrientation)
  {
    mOrientation = pOrientation;
  }
  
  public int getOrientation()
  {
    return mOrientation;
  }
  
  /* (non-Javadoc)
   * @see java.awt.Component#paint(java.awt.Graphics)
   */
  public void paint(Graphics g2)
  {
    Graphics2D g = (Graphics2D)g2;
    mDrawBuffer.clear();
    synchronized(mFillBuffer)
    {
      mDrawBuffer.addAll(mFillBuffer);
    }
    for(ColoredAwtRectangle r : mDrawBuffer)
    {
      g.setColor(r.getColor());
      g.fill(r);
      if(mDrawOutline)
      {
        g.setColor(Color.white);
        g.draw(r); 
      }
    }
    
    if(mLatestValue != null)
    {
      String label = sFORMAT.format(mLatestValue);
      g.setColor(Color.black);
      g.setFont(sFONT);
      g.drawString(label, 2, mLabelY);      
    }
  }
  
  public void setDrawOutline(boolean pDrawOutline)
  {
    mDrawOutline = pDrawOutline;
  }
  
  public boolean getDrawOutline()
  {
    return mDrawOutline;
  }

  public final void update(T pData)
  {
    
    if(mColors.size() > mNumBins)
    {
      mColors.removeLast();
    }
    Color dataColor = mColorScale.toColor(pData);
    mColors.addFirst(dataColor);
    List<ColoredAwtRectangle> fill = updateFillBuffer(mColors.iterator());

    synchronized(mFillBuffer)
    {
      mFillBuffer.clear();
      mFillBuffer.addAll(fill);
    }
    
    mLatestValue = pData;
  }

  /**
   * @param pNumBins maximum number of colored rectangles to display
   * across the total bar.  Each bin will have a size relative to the total
   * size of the bar - i.e. if a horizontal bar is 100 pixels wide and contains
   * 5 bins, each bin rectangle color will be 20 pixels wide
   */
  public void setMaxHistoryBins(int pNumBins)
  {
    mNumBins = pNumBins;
    if(mOrientation == SwingConstants.HORIZONTAL)
    {
      mPixelsPerBin = mWidth / mNumBins;
    }
    else
    {
      mPixelsPerBin = mHeight / mNumBins;
    }
  }
  
  /**
   * @return number of history bins this bar will display
   */
  public int getMaxHistoryBins()
  {
    return mNumBins;
  }

  protected List<ColoredAwtRectangle> updateFillBuffer(Iterator<Color> pColors)
  {
    List<ColoredAwtRectangle> fill = new ArrayList<>();
    Iterator<Color> colorSeq = mColors.iterator();
    int loc = 0;
    for(int p = 0; colorSeq.hasNext(); p+=mPixelsPerBin)
    {
      Color c = colorSeq.next();
      loc = p;
      if(mOrientation == SwingConstants.HORIZONTAL)
      {
        if(mDirection == SwingConstants.TRAILING)
        {
          loc = mWidth - p - mPixelsPerBin;
        }
        fill.add(new ColoredAwtRectangle(c, mX0 + loc, mY0, mPixelsPerBin, mHeight));
      }
      else
      {
        if(mDirection == SwingConstants.TRAILING)
        {
          loc = mHeight - p - mPixelsPerBin;
        }
        fill.add(new ColoredAwtRectangle(c, mX0, loc + mY0, mWidth, mPixelsPerBin));
      }
    }

    return fill;
  }

  @Override
  public EDrawPriority getPriority()
  {
    return EDrawPriority.ANNOTATIONS_MED;
  }

}
