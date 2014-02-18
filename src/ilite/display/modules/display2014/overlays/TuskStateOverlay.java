package ilite.display.modules.display2014.overlays;

import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.EDrawPriority;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Path2D;

public class TuskStateOverlay extends AbstractLayer
{
	private int xStart, yStart;
	private final Font mFontMod = new Font("Arial", Font.BOLD, 40);
	private final Font mFontHUD = new Font("Arial", Font.BOLD, 120);
	private String status;
	
	public TuskStateOverlay(int pXStart, int pYStart, String pStatus)
	{
		xStart = pXStart;
		yStart = pYStart;
		status = pStatus;
	}
	
	@Override
	public void paint(Graphics g)
	{
		Color bkgd = new Color(230, 230, 230);
		Color off = new Color(130, 130, 130);
		Color on = new Color(0, 255, 0);
		
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
			g2.setColor(off);
			g2.setFont(mFontMod);
			g2.drawString("L", xStart+10, yStart+40);

			g2.setColor(off);
			g2.setFont(mFontMod);
			g2.drawString("R", xStart+55, yStart+40);
		}
		else
		{
			g2.setColor(off);
			g2.setFont(mFontHUD);
			g2.drawString("L", xStart+5, yStart+115);

			g2.setColor(off);
			g2.setFont(mFontHUD);
			g2.drawString("R", xStart+80, yStart+115);
		}
	}
			
	@Override
	public EDrawPriority getPriority()
	{
		return EDrawPriority.ANNOTATIONS_MED;
	}
}
