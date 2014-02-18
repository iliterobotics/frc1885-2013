package ilite.display.modules.display2013;

import ilite.util.gui.RepaintClosure;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.Rectangle2D;

import javax.swing.JPanel;

@SuppressWarnings("serial")
public class goalPanel extends JPanel
{
	Graphics2D g2;
	Rectangle2D goalHigh;
	Color goalHighColor;
	public goalPanel()
	{
		//initializing a panel
		setVisible(true);
		goalHighColor = Color.gray;
		//adding a border
	}
	@Override
	public void paint(Graphics g)
	{
		Dimension d = this.getSize();
//		System.out.println(d);
		Graphics2D g2 = (Graphics2D) g;
		Rectangle2D rect = new Rectangle2D.Double(0, 0, d.width, d.height);
		goalHigh = new Rectangle2D.Double(rect.getWidth()/2-100, rect.getHeight()/2-50, 200, 100);
		g2.setPaint(Color.WHITE);
		g2.fill(rect);
		g2.draw(rect);
		g2.setPaint(goalHighColor);
		g2.setStroke(new BasicStroke(3));
		g2.draw(goalHigh);
	}
	

	public void setHighGoalColor(Color c)
	{
		goalHighColor = c;
		new RepaintClosure(this);
	}
}
