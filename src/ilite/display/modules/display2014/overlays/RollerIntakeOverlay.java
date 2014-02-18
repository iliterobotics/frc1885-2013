package ilite.display.modules.display2014.overlays;

import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.EDrawPriority;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Path2D;

public class RollerIntakeOverlay extends AbstractLayer
{
	private int xStart, yStart;
	private String status;
	public RollerIntakeOverlay(int pXStart, int pYStart, String pStatus)
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
		Path2D forward = new Path2D.Double();
		Path2D reverse = new Path2D.Double();
		
		if(status.equals("main"))
		{
			rect.moveTo(xStart, yStart);
			rect.lineTo(xStart+100, yStart);
			rect.lineTo(xStart+100, yStart+50);
			rect.lineTo(xStart, yStart+50);
			rect.lineTo(xStart, yStart);

			forward.moveTo(xStart+10+35, yStart+10);
			forward.lineTo(xStart+10+35, yStart+40);
			forward.lineTo(xStart+10, yStart+25);
			forward.lineTo(xStart+10+35, yStart+10);

			reverse.moveTo(xStart+55, yStart+10);
			reverse.lineTo(xStart+55, yStart+40);
			reverse.lineTo(xStart+90, yStart+25);
			reverse.lineTo(xStart+55, yStart+10);
		}
		
		else
		{
			rect.moveTo(xStart, yStart);
			rect.lineTo(xStart+175, yStart);
			rect.lineTo(xStart+175, yStart+150);
			rect.lineTo(xStart, yStart+150);
			rect.lineTo(xStart, yStart);
			
			forward.moveTo(xStart+75, yStart+25);
			forward.lineTo(xStart+75, yStart+125);
			forward.lineTo(xStart+25, yStart+75);
			forward.lineTo(xStart+75, yStart+25);

			reverse.moveTo(xStart+100, yStart+25);
			reverse.lineTo(xStart+100, yStart+125);
			reverse.lineTo(xStart+150, yStart+75);
			reverse.lineTo(xStart+100, yStart+25);
		}
		
		g2.setColor(bkgd);
		g2.fill(rect);
		
		g2.setColor(off);
		g2.fill(forward);
		
		g2.setColor(on);
		g2.fill(reverse);
	}

	@Override
	public EDrawPriority getPriority() 
	{
		return EDrawPriority.ANNOTATIONS_MED;
	}
	
	
		
}
