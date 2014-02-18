package ilite.display.modules.display2013.overlays;

import ilite.util.gui.components.layer.AbstractLayer;
import ilite.util.gui.components.layer.EDrawPriority;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;

public class LabelOverlays extends AbstractLayer
{
	private String text = "HELLO";
    private int mHeight;
    private int mWidth;
    
    private final Font mFontMod = new Font("Arial", Font.BOLD, 40);
    
    public LabelOverlays(int pWidth, int pHeight, String imageName)
    {
    	mWidth = pWidth;
    	mHeight = pHeight;
    	text = imageName;
    }
    
    
	@Override
	public void paint(Graphics g)
	{
		Graphics2D g2 = (Graphics2D) g;
		g2.setFont(mFontMod);
		g2.setPaint(Color.black);
		g2.drawString(text, 5, 30);
	}

	@Override
	public EDrawPriority getPriority()
	{
		return EDrawPriority.ANNOTATIONS_LOW;
	}
}
