package ilite.display.modules.display2013.overlays;

import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.EDrawPriority;

import java.awt.Color;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;

public class ConnectionStatusOverlay extends AbstractLayer
{
	private final int mWidth, mHeight;
	private Font mFont = new Font("Arial", Font.BOLD, 50);
	
	public ConnectionStatusOverlay(int pWidth, int pHeight)
	{
		mWidth = pWidth;
		mHeight = pHeight;
	}
	
	@Override
	public void paint(Graphics g)
	{
		Graphics2D g2 = (Graphics2D) g;
		FontMetrics fm = g2.getFontMetrics();
		g2.setFont(mFont);
		g2.setPaint(Color.YELLOW);
		g2.drawString("CONNECTION LOST", mWidth/2-(fm.stringWidth("CONNECTION LOST"))/2, fm.getHeight()+5);
	}
	@Override
	public EDrawPriority getPriority()
	{
		return EDrawPriority.ANNOTATION_HIGH;
	}
}
