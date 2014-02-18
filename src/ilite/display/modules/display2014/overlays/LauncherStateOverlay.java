package ilite.display.modules.display2014.overlays;

import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.EDrawPriority;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Path2D;

public class LauncherStateOverlay extends AbstractLayer
{
  Color off = new Color(130, 130, 130);
  Color on = new Color(0, 255, 255);
  
  Path2D rect = new Path2D.Double();
  Path2D mDownTri = new Path2D.Double();
  Path2D mUpTri = new Path2D.Double();
	
	public LauncherStateOverlay(int pX, int pY, int pWidth, int pHeight, int pInset)
	{
    rect.moveTo(pX, pY);
    rect.lineTo(pX+pWidth, pY);
    rect.lineTo(pX+pWidth, pY+pHeight);
    rect.lineTo(pX, pY+pHeight);
    rect.lineTo(pX, pY);

    mDownTri.moveTo(pX+pInset, pY+pInset);
    mDownTri.lineTo(pX+pWidth/2, pY+pInset);
    mDownTri.lineTo(pX+pWidth/4+pInset/2, pY+pHeight-pInset);
    mDownTri.lineTo(pX+pInset, pY+pInset);

    mUpTri.moveTo(pX+pWidth/2, pY+pHeight-pInset);
    mUpTri.lineTo(pX+pWidth-pInset, pY+pHeight-pInset);
    mUpTri.lineTo(pX+pWidth*3/4-pInset/2, pY+pInset);
    mUpTri.lineTo(pX+pWidth/2, pY+pHeight-pInset);
	}
	
	@Override
	public void paint(Graphics g)
	{
    Graphics2D g2 = (Graphics2D) g;
			
		g2.setColor(Color.black);
		g2.fill(rect);
		
		g2.setColor(off);
		g2.fill(mDownTri);
		
		g2.setColor(on);
		g2.fill(mUpTri);
	}

	@Override
	public EDrawPriority getPriority()
	{
		return EDrawPriority.ANNOTATIONS_MED;
	}

}
