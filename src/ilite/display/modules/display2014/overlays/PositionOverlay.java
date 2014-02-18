package ilite.display.modules.display2014.overlays;

import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.EDrawPriority;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Path2D;

public class PositionOverlay extends AbstractLayer
{
	private int xStart, yStart;
	private String pos, status;
	private final Font mFontMod = new Font("Arial", Font.BOLD, 23);
	private final Font mFontHUD = new Font("Arial", Font.BOLD, 80);
	 
	public PositionOverlay(int pXStart, int pYStart, String pPos, String pStatus)
	{
		xStart = pXStart;
		yStart = pYStart;
		pos = pPos;
		status = pStatus;
	}
	
	@Override
	public void paint(Graphics g)
	{
		Color bkgd = new Color(230, 230, 230);
		Color out = new Color(130, 130, 130);
		Color in = new Color(0, 255, 0);
		Color critical = new Color(255, 0, 0);
		
		Graphics2D g2 = (Graphics2D) g;
		Path2D rect = new Path2D.Double();
		
		if(status.equals("main"))
		{
			rect.moveTo(xStart, yStart);
			rect.lineTo(xStart+100, yStart);
			rect.lineTo(xStart+100, yStart+50);
			rect.lineTo(xStart, yStart+50);
			rect.lineTo(xStart, yStart);
		}
		else
		{
			rect.moveTo(xStart, yStart);
			rect.lineTo(xStart+175, yStart);
			rect.lineTo(xStart+175, yStart+150);
			rect.lineTo(xStart, yStart+150);
			rect.lineTo(xStart, yStart);
		}
		
		g2.setColor(bkgd);
		g2.fill(rect);
		
		if(status.equals("main"))
		{
			g2.setColor(critical);
			g2.setFont(mFontMod);
			g2.drawString(pos, xStart+10, yStart+35);
		}
		else
		{
			g2.setColor(critical);
			g2.setFont(mFontHUD);
			g2.drawString(pos, xStart+7, yStart+75);
			g2.drawString("ft", xStart+60, yStart+140);
		}
	}
			
	@Override
	public EDrawPriority getPriority()
	{
		return EDrawPriority.ANNOTATIONS_MED;
	}
	
}
