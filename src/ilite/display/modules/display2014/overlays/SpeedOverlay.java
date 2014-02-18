package ilite.display.modules.display2014.overlays;

import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.EDrawPriority;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Path2D;

public class SpeedOverlay extends AbstractLayer
{
	private int xStart, yStart;
	private String speed, status;
	private final Font mFontMod = new Font("Arial", Font.BOLD, 20);
	private final Font mFontHUD = new Font("Arial", Font.BOLD, 80);
	
	public SpeedOverlay(int pXStart, int pYStart, String pSpeed, String pStatus)
	{
		xStart = pXStart;
		yStart = pYStart;
		speed = pSpeed;
		status = pStatus;
	}
	
	@Override
	public void paint(Graphics g)
	{
		Color bkgd = new Color(230, 230, 230);
		Color low = new Color(130, 130, 130);
		Color high = new Color(0, 255, 0);
		
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
			g2.setColor(high);
			g2.setFont(mFontMod);
			g2.drawString(speed, xStart+10, yStart+33);
		}
		else
		{
			g2.setColor(high);
			g2.setFont(mFontHUD);
			g2.drawString(speed, xStart+7, yStart+75);
			g2.drawString("mph", xStart+5, yStart+135);
		}
	}
			
	@Override
	public EDrawPriority getPriority()
	{
		return EDrawPriority.ANNOTATIONS_MED;
	}
}
